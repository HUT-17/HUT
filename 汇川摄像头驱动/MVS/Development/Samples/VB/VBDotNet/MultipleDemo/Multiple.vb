Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Public Class Multiple
    Dim dev(4) As MyCamera

    ' ch:用于保存图像的缓存 | en:Buffer to save
    Dim m_nBufSizeForSaveImage As UInt32 = 3072 * 2048 * 3 * 3 + 2048
    Dim m_pBufForSaveImage(m_nBufSizeForSaveImage) As Byte

    Dim m_nByteImageBuffer As UInt32 = 3072 * 2048 * 3
    Dim m_byteImageBuffer(m_nByteImageBuffer) As Byte

    Dim m_stDeviceInfoList As MyCamera.MV_CC_DEVICE_INFO_LIST = New MyCamera.MV_CC_DEVICE_INFO_LIST
    Dim m_stDeviceInfo(4) As MyCamera.MV_CC_DEVICE_INFO
    Dim m_stMatchInfo As MyCamera.MV_ALL_MATCH_INFO = New MyCamera.MV_ALL_MATCH_INFO

    Dim m_nCanOpenDeviceNum As UInt32
    Dim m_PictureBoxCamera(4) As PictureBox
    Dim m_TextBoxCameraFrameGet(4) As TextBox
    Dim m_TextBoxCameraFrameLose(4) As TextBox

    Dim m_bIsOpen As Boolean
    Dim m_bIsGrabbing As Boolean
    Dim m_nFrameCount(4) As UInteger
    Dim m_bIsSaveImage(4) As Boolean
    Dim m_myCallback As MyCamera.cbOutputExdelegate = New MyCamera.cbOutputExdelegate(AddressOf cbOutputdelegateFunc)

    Private Sub cbOutputdelegateFunc(ByVal pData As IntPtr, ByRef pFrameInfo As MyCamera.MV_FRAME_OUT_INFO_EX, ByVal pUser As IntPtr)
        Dim nIndex As UInt32
        nIndex = pUser
        m_nFrameCount(nIndex) = pFrameInfo.nFrameNum

        If (m_bIsSaveImage(nIndex) = True) Then
            m_bIsSaveImage(nIndex) = False
            If 3 * pFrameInfo.nFrameLen + 2048 > m_nBufSizeForSaveImage Then
                m_nBufSizeForSaveImage = 3 * pFrameInfo.nFrameLen + 2048
                ReDim m_pBufForSaveImage(m_nBufSizeForSaveImage)
            End If

            Dim nRet As Int32
            Dim pImage As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForSaveImage, 0)
            Dim stSaveImageParam As MyCamera.MV_SAVE_IMAGE_PARAM = New MyCamera.MV_SAVE_IMAGE_PARAM()
            stSaveImageParam.pData = pData
            stSaveImageParam.nDataLen = pFrameInfo.nFrameLen
            stSaveImageParam.enPixelType = pFrameInfo.enPixelType
            stSaveImageParam.nWidth = pFrameInfo.nWidth
            stSaveImageParam.nHeight = pFrameInfo.nHeight
            stSaveImageParam.enImageType = MyCamera.MV_SAVE_IAMGE_TYPE.MV_Image_Bmp
            stSaveImageParam.nBufferSize = m_nBufSizeForSaveImage
            stSaveImageParam.pImageBuffer = pImage

            nRet = dev(nIndex).MV_CC_SaveImage_NET(stSaveImageParam)
            If (MyCamera.MV_OK <> nRet) Then
                MsgBox("Image format convert fail")
                Return
            End If

            ' ch:写文件 | en:Write file
            ReDim m_byteImageBuffer(stSaveImageParam.nImageLen)
            Marshal.Copy(pImage, m_byteImageBuffer, 0, stSaveImageParam.nImageLen)
            My.Computer.FileSystem.WriteAllBytes("Image" + nIndex.ToString() + ".bmp", m_byteImageBuffer, False)
        End If

        Dim stDisplayInfo As MyCamera.MV_DISPLAY_FRAME_INFO = New MyCamera.MV_DISPLAY_FRAME_INFO()
        stDisplayInfo.hWnd = m_PictureBoxCamera([nIndex]).Handle
        stDisplayInfo.pData = pData
        stDisplayInfo.nDataLen = pFrameInfo.nFrameLen
        stDisplayInfo.nWidth = pFrameInfo.nWidth
        stDisplayInfo.nHeight = pFrameInfo.nHeight
        stDisplayInfo.enPixelType = pFrameInfo.enPixelType

        dev(nIndex).MV_CC_DisplayOneFrame_NET(stDisplayInfo)

    End Sub

    Private Sub Multiple_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim i As UInt32 = 0
        For i = 0 To 3
            dev(i) = New MyCamera
            m_stDeviceInfo(i) = New MyCamera.MV_CC_DEVICE_INFO
        Next

        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = MyCamera.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE Or MyCamera.MV_USB_DEVICE), m_stDeviceInfoList)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Enum device failed" + Convert.ToString(nRet))
            Return
        End If

        Control.CheckForIllegalCrossThreadCalls = False

        TextBoxEnumCameraNum.Text = m_stDeviceInfoList.nDeviceNum.ToString()
        TextBoxCameraUsingNum.Text = m_stDeviceInfoList.nDeviceNum.ToString()

        m_PictureBoxCamera(0) = PictureBoxCamera0
        m_PictureBoxCamera(1) = PictureBoxCamera1
        m_PictureBoxCamera(2) = PictureBoxCamera2
        m_PictureBoxCamera(3) = PictureBoxCamera3

        m_nFrameCount(0) = 0
        m_nFrameCount(1) = 0
        m_nFrameCount(2) = 0
        m_nFrameCount(3) = 0

        m_bIsSaveImage(0) = False
        m_bIsSaveImage(1) = False
        m_bIsSaveImage(2) = False
        m_bIsSaveImage(3) = False

        m_TextBoxCameraFrameGet(0) = TextBoxCamera0FrameGet
        m_TextBoxCameraFrameGet(1) = TextBoxCamera1FrameGet
        m_TextBoxCameraFrameGet(2) = TextBoxCamera2FrameGet
        m_TextBoxCameraFrameGet(3) = TextBoxCamera3FrameGet

        m_TextBoxCameraFrameLose(0) = TextBoxCamera0FrameLose
        m_TextBoxCameraFrameLose(1) = TextBoxCamera1FrameLose
        m_TextBoxCameraFrameLose(2) = TextBoxCamera2FrameLose
        m_TextBoxCameraFrameLose(3) = TextBoxCamera3FrameLose

        ' ch:初始化一些控件可视化参数 | en:Initialize parameter visibility of controls
        TextBoxExposureTime.Enabled = False
        TextBoxGain.Enabled = False
        ButtonSetParam.Enabled = False
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = False
        ButtonSaveImage.Enabled = False
        CheckBoxSoftware.Enabled = False
        ButtonSoftwareOnce.Enabled = False
        ButtonClose.Enabled = False

    End Sub

    Private Sub ButtonInitCamera_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonInitCamera.Click
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = MyCamera.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE Or MyCamera.MV_USB_DEVICE), m_stDeviceInfoList)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to enumerate devices" + Convert.ToString(nRet))
            Return
        End If

        TextBoxEnumCameraNum.Text = m_stDeviceInfoList.nDeviceNum.ToString()

        Dim nCameraUsingNum As UInt32 = 0
        Try
            nCameraUsingNum = Convert.ToUInt32(TextBoxCameraUsingNum.Text)
        Catch
            MsgBox("Format of the used camera number is not correct")
            Return
        Finally
        End Try

        ' ch:参数检测 | en:Check parameters
        If (nCameraUsingNum <= 0) Then
            nCameraUsingNum = 1
        End If
        If (nCameraUsingNum > 4) Then
            nCameraUsingNum = 4
        End If
        If (nCameraUsingNum > m_stDeviceInfoList.nDeviceNum) Then
            nCameraUsingNum = m_stDeviceInfoList.nDeviceNum
        End If

        m_nCanOpenDeviceNum = 0
        Dim i As UInt32 = 0
        Dim j As UInt32 = 0
        For i = 0 To m_stDeviceInfoList.nDeviceNum - 1
            m_stDeviceInfo(j) = CType(Marshal.PtrToStructure(m_stDeviceInfoList.pDeviceInfo(i), GetType(MyCamera.MV_CC_DEVICE_INFO)), MyCamera.MV_CC_DEVICE_INFO)
            nRet = dev(j).MV_CC_CreateDevice_NET(m_stDeviceInfo(j))
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Create handle fail")
                Continue For
            End If

            ' ch:打开设备 | en:Open device
            nRet = dev(j).MV_CC_OpenDevice_NET()
            If MyCamera.MV_OK <> nRet Then
                dev(j).MV_CC_DestroyDevice_NET()
            Else
                ' ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
                If m_stDeviceInfo(j).nTLayerType = MyCamera.MV_GIGE_DEVICE Then
                    Dim nPacketSize As Int32
                    nPacketSize = dev(j).MV_CC_GetOptimalPacketSize_NET()
                    If nPacketSize > 0 Then
                        nRet = dev(j).MV_CC_SetIntValue_NET("GevSCPSPacketSize", nPacketSize)
                        If 0 <> nRet Then
                            Console.WriteLine("Warning: Set Packet Size failed:{0:x8}", nRet)
                        End If
                    Else
                        Console.WriteLine("Warning: Get Packet Size failed:{0:x8}", nPacketSize)
                    End If
                End If
                dev(j).MV_CC_RegisterImageCallBackEx_NET(m_myCallback, j)
                dev(j).MV_CC_SetEnumValue_NET("TriggerMode", 0)
                m_nCanOpenDeviceNum = m_nCanOpenDeviceNum + 1
                If (m_nCanOpenDeviceNum = nCameraUsingNum) Then
                    Exit For
                End If
                j = j + 1
            End If
        Next i

        m_bIsOpen = True
        TextBoxCameraUsingNum.Text = m_nCanOpenDeviceNum.ToString()

        If 0 = m_nCanOpenDeviceNum Then
            MsgBox("No camera available")
            Return
        End If

        ButtonInitCamera.Enabled = False
        TextBoxCameraUsingNum.Enabled = False
        TextBoxExposureTime.Enabled = True
        TextBoxGain.Enabled = True
        ButtonSetParam.Enabled = True
        ButtonStartGrabbing.Enabled = True
        ButtonStopGrabbing.Enabled = False
        ButtonSaveImage.Enabled = False
        CheckBoxSoftware.Enabled = True
        ButtonSoftwareOnce.Enabled = False
        ButtonClose.Enabled = True
    End Sub

    Private Sub ButtonSetParam_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSetParam.Click
        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            ' ch:设置曝光时间 | en:Set exposure time
            Dim fExposureTime As Single = 0

            Try
                fExposureTime = Convert.ToSingle(TextBoxExposureTime.Text)
            Catch
                MsgBox("Format of exposure time is not correct")
                Return
            Finally
            End Try

            Dim nRet As Int32
            nRet = dev(i).MV_CC_SetEnumValue_NET("ExposureMode", 0)
            nRet = dev(i).MV_CC_SetFloatValue_NET("ExposureTime", fExposureTime)
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to set exposure time")
            End If
            ' ch:设置增益 | en:Set gain
            Dim fGain As Single = 0

            Try
                fGain = Convert.ToSingle(TextBoxGain.Text)
            Catch
                MsgBox("Format of gain is not correct")
                Return
            Finally
            End Try

            nRet = dev(i).MV_CC_SetEnumValue_NET("GainAuto", 0)
            nRet = dev(i).MV_CC_SetFloatValue_NET("Gain", fGain)
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to set gain")
            End If
        Next
    End Sub

    Private Sub ButtonStartGrabbing_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonStartGrabbing.Click
        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            ' ch:开启采集 | en:Start Grabbing
            Dim nRet As Int32
            nRet = dev(i).MV_CC_StartGrabbing_NET()
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to start grabbing")
            End If
            m_bIsGrabbing = True
        Next
        TimerCameraControl.Enabled = True

        CheckBoxSoftware.Enabled = True
        If (CheckBoxSoftware.Checked) Then
            ButtonSoftwareOnce.Enabled = True
        Else
            ButtonSoftwareOnce.Enabled = False
        End If
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = True
        ButtonSaveImage.Enabled = True

    End Sub

    Private Sub ButtonStopGrabbing_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonStopGrabbing.Click
        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            ' ch:停止采集 | en:Stop grabbing
            Dim nRet As Int32
            nRet = dev(i).MV_CC_StopGrabbing_NET()
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to stop grabbing")
            End If
        Next

        m_bIsGrabbing = False
        TimerCameraControl.Enabled = False

        ButtonStartGrabbing.Enabled = True
        ButtonStopGrabbing.Enabled = False
        ButtonSaveImage.Enabled = False
        CheckBoxSoftware.Enabled = False
        ButtonSoftwareOnce.Enabled = False

    End Sub

    Private Sub ButtonSaveImage_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSaveImage.Click
        For i = 0 To m_nCanOpenDeviceNum - 1
            m_bIsSaveImage(i) = True
        Next i
    End Sub

    Private Sub CheckBoxSoftware_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBoxSoftware.CheckedChanged

        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            If (CheckBoxSoftware.Checked) Then
                ' ch:开启触发 | en:Open trigger mode
                Dim nRet As Int32 = MyCamera.MV_OK
                nRet = dev(i).MV_CC_SetEnumValue_NET("TriggerMode", 1)
                If MyCamera.MV_OK <> nRet Then
                    MsgBox("Fail to close trigger mode")
                End If
                ' ch:设置软触发 | en:Set software trigger
                nRet = dev(i).MV_CC_SetEnumValue_NET("TriggerSource", 7)
                If MyCamera.MV_OK <> nRet Then
                    MsgBox("Fail to set software trigger")
                End If

                ButtonSoftwareOnce.Enabled = True
                ButtonSaveImage.Enabled = False

            Else
                ' ch:关闭触发 | en:Close trigger mode
                Dim nRet As Int32 = MyCamera.MV_OK
                nRet = dev(i).MV_CC_SetEnumValue_NET("TriggerMode", 0)
                If MyCamera.MV_OK <> nRet Then
                    MsgBox("Fail to close trigger mode")
                End If
                ' ch:设置硬触发 | en:Set hardware trigger
                nRet = dev(i).MV_CC_SetEnumValue_NET("TriggerSource", 0)
                If MyCamera.MV_OK <> nRet Then
                    MsgBox("Fail to set hardware trigger")
                End If
                ButtonSoftwareOnce.Enabled = False
                ButtonSaveImage.Enabled = True

            End If
        Next
    End Sub

    Private Sub ButtonSoftwareOnce_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSoftwareOnce.Click

        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            ' ch:软触发一次 | en:Software trigger once
            Dim nRet As Int32
            nRet = dev(i).MV_CC_SetCommandValue_NET("TriggerSoftware")
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to software trigger once")
            End If
        Next
    End Sub

    Private Sub ButtonClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonClose.Click
        If 0 = m_nCanOpenDeviceNum Then
            Return
        End If

        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            ' ch:关闭设备 | en:Close device
            Dim nRet As Int32 = MyCamera.MV_OK
            nRet = dev(i).MV_CC_CloseDevice_NET()
            nRet = dev(i).MV_CC_DestroyDevice_NET()
        Next

        m_bIsOpen = False

        ButtonInitCamera.Enabled = True
        TextBoxCameraUsingNum.Enabled = True
        TextBoxExposureTime.Enabled = False
        TextBoxGain.Enabled = False
        ButtonSetParam.Enabled = False
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = False
        ButtonSaveImage.Enabled = False
        CheckBoxSoftware.Enabled = False
        ButtonSoftwareOnce.Enabled = False
        ButtonClose.Enabled = False

    End Sub

    Private Sub TimerCameraControl_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TimerCameraControl.Tick
        If 0 = m_nCanOpenDeviceNum Then
            Return
        End If

        Dim i As UInt32 = 0
        Dim nRet As Int32 = MyCamera.MV_OK
        For i = 0 To m_nCanOpenDeviceNum - 1
            If m_stDeviceInfo(i).nTLayerType = MyCamera.MV_GIGE_DEVICE Then
                Dim stMatchInfoNetDetect As MyCamera.MV_MATCH_INFO_NET_DETECT = New MyCamera.MV_MATCH_INFO_NET_DETECT
                m_stMatchInfo.nInfoSize = System.Runtime.InteropServices.Marshal.SizeOf(GetType(MyCamera.MV_MATCH_INFO_NET_DETECT))
                m_stMatchInfo.nType = MyCamera.MV_MATCH_TYPE_NET_DETECT
                Dim size As Int32 = Marshal.SizeOf(stMatchInfoNetDetect)
                m_stMatchInfo.pInfo = Marshal.AllocHGlobal(size)
                Marshal.StructureToPtr(stMatchInfoNetDetect, m_stMatchInfo.pInfo, False)

                nRet = dev(i).MV_CC_GetAllMatchInfo_NET(m_stMatchInfo)

                If MyCamera.MV_OK = nRet Then
                    m_TextBoxCameraFrameLose(i).Text = stMatchInfoNetDetect.nLostFrameCount
                End If
                m_TextBoxCameraFrameGet(i).Text = m_nFrameCount(i).ToString()
            ElseIf m_stDeviceInfo(i).nTLayerType = MyCamera.MV_USB_DEVICE Then
                Dim stMatchInfoUSBDetect As MyCamera.MV_MATCH_INFO_USB_DETECT = New MyCamera.MV_MATCH_INFO_USB_DETECT
                m_stMatchInfo.nInfoSize = System.Runtime.InteropServices.Marshal.SizeOf(GetType(MyCamera.MV_MATCH_INFO_USB_DETECT))
                m_stMatchInfo.nType = MyCamera.MV_MATCH_TYPE_USB_DETECT
                Dim size As Int32 = Marshal.SizeOf(stMatchInfoUSBDetect)
                m_stMatchInfo.pInfo = Marshal.AllocHGlobal(size)
                Marshal.StructureToPtr(stMatchInfoUSBDetect, m_stMatchInfo.pInfo, False)

                nRet = dev(i).MV_CC_GetAllMatchInfo_NET(m_stMatchInfo)

                If MyCamera.MV_OK = nRet Then
                    m_TextBoxCameraFrameLose(i).Text = stMatchInfoUSBDetect.nErrorFrameCount
                End If
                m_TextBoxCameraFrameGet(i).Text = m_nFrameCount(i).ToString()
            End If

        Next i
    End Sub

    Private Sub Multiple_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        If 0 = m_nCanOpenDeviceNum Then
            Return
        End If

        Dim i As UInt32 = 0
        For i = 0 To m_nCanOpenDeviceNum - 1
            ' ch:关闭设备 | en:Close Device
            Dim nRet As Int32 = MyCamera.MV_OK
            nRet = dev(i).MV_CC_CloseDevice_NET()
            nRet = dev(i).MV_CC_DestroyDevice_NET()
        Next
    End Sub
End Class

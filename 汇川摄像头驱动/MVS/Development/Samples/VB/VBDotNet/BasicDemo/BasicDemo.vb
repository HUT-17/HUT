Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports System.IO
Imports MvCamCtrl.NET


Public Class BasicDemo
    Dim dev As MyCamera = New MyCamera

    ' ch:用于从驱动获取图像的缓存 | en:Buffer to get image from driver
    Dim m_nBufSizeForDriver As UInt32 = 1000 * 1000 * 3
    Dim m_pBufForDriver(m_nBufSizeForDriver) As Byte

    ' ch:用于保存图像的缓存 | en:Buffer to save image
    Dim m_nBufSizeForSaveImage As UInt32 = 1000 * 1000 * 3 * 3 + 2048
    Dim m_pBufForSaveImage(m_nBufSizeForSaveImage) As Byte

    ' ch:成员变量，用于控制相机 | en:Member variable to control camera
    Dim m_stDeviceInfoList As MyCamera.MV_CC_DEVICE_INFO_LIST = New MyCamera.MV_CC_DEVICE_INFO_LIST
    Dim m_nDeviceIndex As UInt32
    Dim m_bIsGrabbing As Boolean = False
    Dim m_hGrabHandle As System.Threading.Thread
    Dim m_stFrameInfo As MyCamera.MV_FRAME_OUT_INFO_EX = New MyCamera.MV_FRAME_OUT_INFO_EX()
    Dim m_ReadWriteLock As System.Threading.ReaderWriterLock

    ' ch:枚举设备按钮操作 | en:Button operation for device enum
    Private Sub ButtonEnumDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonEnumDevice.Click
        '清空设备列表 | en:Clear up device list
        ComboBoxDeviceList.Items.Clear()
        ComboBoxDeviceList.SelectedIndex = -1

        Dim Info As String
        Dim nRet As Int32 = MyCamera.MV_OK
        ' ch:枚举设备 | en:Enumerate devices
        nRet = MyCamera.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE Or MyCamera.MV_USB_DEVICE), m_stDeviceInfoList)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to enumerate devices" + Convert.ToString(nRet))
            Return
        End If

        If (0 = m_stDeviceInfoList.nDeviceNum) Then
            MsgBox("No Find Device !")
            Return
        End If

        ' ch:将设备信息放到设备列表中 | en:Put device information in the device list
        Dim i As Int32
        For i = 0 To m_stDeviceInfoList.nDeviceNum - 1
            Dim stDeviceInfo As MyCamera.MV_CC_DEVICE_INFO = New MyCamera.MV_CC_DEVICE_INFO
            stDeviceInfo = CType(Marshal.PtrToStructure(m_stDeviceInfoList.pDeviceInfo(i), GetType(MyCamera.MV_CC_DEVICE_INFO)), MyCamera.MV_CC_DEVICE_INFO)
            If (MyCamera.MV_GIGE_DEVICE = stDeviceInfo.nTLayerType) Then
                Dim stGigeInfoPtr As IntPtr = Marshal.AllocHGlobal(216)
                Marshal.Copy(stDeviceInfo.SpecialInfo.stGigEInfo, 0, stGigeInfoPtr, 216)
                Dim stGigeInfo As MyCamera.MV_GIGE_DEVICE_INFO
                stGigeInfo = CType(Marshal.PtrToStructure(stGigeInfoPtr, GetType(MyCamera.MV_GIGE_DEVICE_INFO)), MyCamera.MV_GIGE_DEVICE_INFO)
                Dim nIpByte1 As UInt32 = (stGigeInfo.nCurrentIp And &HFF000000) >> 24
                Dim nIpByte2 As UInt32 = (stGigeInfo.nCurrentIp And &HFF0000) >> 16
                Dim nIpByte3 As UInt32 = (stGigeInfo.nCurrentIp And &HFF00) >> 8
                Dim nIpByte4 As UInt32 = (stGigeInfo.nCurrentIp And &HFF)

                If (stGigeInfo.chUserDefinedName = "") Then
                    Info = "DEV:[" + Convert.ToString(i) + "] " + stGigeInfo.chModelName + "(" + nIpByte1.ToString() + "." + nIpByte2.ToString() + "." + nIpByte3.ToString() + "." + nIpByte4.ToString() + ")"
                Else
                    Info = "DEV:[" + Convert.ToString(i) + "] " + stGigeInfo.chUserDefinedName + "(" + nIpByte1.ToString() + "." + nIpByte2.ToString() + "." + nIpByte3.ToString() + "." + nIpByte4.ToString() + ")"
                End If

                ComboBoxDeviceList.Items.Add(Info)
            Else
                Dim stUsbInfoPtr As IntPtr = Marshal.AllocHGlobal(540)
                Marshal.Copy(stDeviceInfo.SpecialInfo.stUsb3VInfo, 0, stUsbInfoPtr, 540)
                Dim stUsbInfo As MyCamera.MV_USB3_DEVICE_INFO
                stUsbInfo = CType(Marshal.PtrToStructure(stUsbInfoPtr, GetType(MyCamera.MV_USB3_DEVICE_INFO)), MyCamera.MV_USB3_DEVICE_INFO)

                If (stUsbInfo.chUserDefinedName = "") Then
                    Info = "U3V:[" + Convert.ToString(i) + "] " + stUsbInfo.chModelName + "(" + stUsbInfo.chSerialNumber + ")"
                Else
                    Info = "U3V:[" + Convert.ToString(i) + "] " + stUsbInfo.chUserDefinedName + "(" + stUsbInfo.chSerialNumber + ")"
                End If

                ComboBoxDeviceList.Items.Add(Info)
            End If
        Next i

        ' ch:设置各个控件的可见度 | en:Set visibility of controls
        GroupBoxInit.Enabled = True
        GroupBoxDeviceControl.Enabled = False
        GroupBoxGrabImage.Enabled = False
        GroupBoxImageSave.Enabled = False
        GroupBoxParam.Enabled = False

        ComboBoxDeviceList.Enabled = True
        ButtonEnumDevice.Enabled = True
        ButtonOpenDevice.Enabled = False
        ButtonCloseDevice.Enabled = False
        RadioButtonTriggerOff.Enabled = False
        RadioButtonTriggerOn.Enabled = False
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = False
        CheckBoxSoftware.Enabled = False
        ButtonSoftwareOnce.Enabled = False
        ButtonSaveBmp.Enabled = False
        ButtonSaveJpg.Enabled = False
        ButtonSaveTiff.Enabled = False
        ButtonSavePng.Enabled = False
        TextBoxExposureTime.Enabled = False
        TextBoxGain.Enabled = False
        TextBoxFrameRate.Enabled = False
        ButtonParamGet.Enabled = False
        ButtonParamSet.Enabled = False
    End Sub

    ' ch:打开设备按钮操作 | en:Button operation for opening device
    Private Sub ButtonOpenDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonOpenDevice.Click
        ' ch:创建句柄 | en:Create handle
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stDeviceInfo As MyCamera.MV_CC_DEVICE_INFO
        stDeviceInfo = CType(Marshal.PtrToStructure(m_stDeviceInfoList.pDeviceInfo(m_nDeviceIndex), GetType(MyCamera.MV_CC_DEVICE_INFO)), MyCamera.MV_CC_DEVICE_INFO)
        nRet = dev.MV_CC_CreateDevice_NET(stDeviceInfo)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to create handle")
            Return
        End If

        ' ch:打开设备 | en:Open device
        nRet = dev.MV_CC_OpenDevice_NET()
        If MyCamera.MV_OK <> nRet Then
            dev.MV_CC_DestroyDevice_NET()
            MsgBox("Open device failed")
            Return
        End If

        ' ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
        If stDeviceInfo.nTLayerType = MyCamera.MV_GIGE_DEVICE Then
            Dim nPacketSize As Int32
            nPacketSize = dev.MV_CC_GetOptimalPacketSize_NET()
            If nPacketSize > 0 Then
                nRet = dev.MV_CC_SetIntValue_NET("GevSCPSPacketSize", nPacketSize)
                If 0 <> nRet Then
                    MsgBox("Set Packet Size failed")
                End If
            Else
                MsgBox("Get Packet Size failed")
            End If
        End If

        ' ch:获取触发模式 | en:Acquire trigger mode
        Dim stTriggerMode As MyCamera.MVCC_ENUMVALUE = New MyCamera.MVCC_ENUMVALUE
        nRet = dev.MV_CC_GetEnumValue_NET("TriggerMode", stTriggerMode)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to acquire trigger mode")
            Return
        End If
        If 0 = stTriggerMode.nCurValue Then
            RadioButtonTriggerOff.Checked = True
            RadioButtonTriggerOn.Checked = False
        Else
            RadioButtonTriggerOff.Checked = False
            RadioButtonTriggerOn.Checked = True
        End If

        ' ch:获取触发源 | en:Acquire trigger source
        Dim stTriggerSource As MyCamera.MVCC_ENUMVALUE = New MyCamera.MVCC_ENUMVALUE
        nRet = dev.MV_CC_GetEnumValue_NET("TriggerSource", stTriggerSource)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to acquire trigger source")
            Return
        End If
        If MyCamera.MV_CAM_TRIGGER_SOURCE.MV_TRIGGER_SOURCE_SOFTWARE = stTriggerSource.nCurValue Then
            CheckBoxSoftware.Checked = True
        Else
            CheckBoxSoftware.Checked = False
        End If

        ' ch:获取曝光时间 | en:Acquire exposure time 
        Dim stExposureTime As MyCamera.MVCC_FLOATVALUE = New MyCamera.MVCC_FLOATVALUE
        nRet = dev.MV_CC_GetFloatValue_NET("ExposureTime", stExposureTime)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to acquire exposure time")
        End If
        TextBoxExposureTime.Text = Convert.ToString(stExposureTime.fCurValue)
        ' ch:获取增益 | en:Acquire gain
        Dim stGain As MyCamera.MVCC_FLOATVALUE = New MyCamera.MVCC_FLOATVALUE
        nRet = dev.MV_CC_GetFloatValue_NET("Gain", stGain)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to acquire gain")
        End If
        TextBoxGain.Text = Convert.ToString(stGain.fCurValue)
        ' ch:获取帧率 | en:Acquire frame rate
        Dim stFrameRate As MyCamera.MVCC_FLOATVALUE = New MyCamera.MVCC_FLOATVALUE
        nRet = dev.MV_CC_GetFloatValue_NET("ResultingFrameRate", stFrameRate)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to acquire frame rate")
        End If
        TextBoxFrameRate.Text = Convert.ToString(stFrameRate.fCurValue)

        ' ch:模式控制 | en:Mode control
        If RadioButtonTriggerOn.Checked Then
            RadioButtonTriggerOn.Enabled = False
            RadioButtonTriggerOff.Enabled = True
        Else
            RadioButtonTriggerOn.Enabled = True
            RadioButtonTriggerOff.Enabled = False
        End If

        If (RadioButtonTriggerOn.Checked) Then
            CheckBoxSoftware.Enabled = True
        Else
            CheckBoxSoftware.Enabled = False
        End If

        If (RadioButtonTriggerOn.Checked And CheckBoxSoftware.Checked) Then
            ButtonSoftwareOnce.Enabled = True
        Else
            ButtonSoftwareOnce.Enabled = False
        End If

        ' ch:设置各个控件的可见性 | en:Set visibility of controls
        GroupBoxDeviceControl.Enabled = True
        GroupBoxGrabImage.Enabled = True
        GroupBoxImageSave.Enabled = True
        GroupBoxParam.Enabled = True

        ComboBoxDeviceList.Enabled = False
        ButtonOpenDevice.Enabled = False
        ButtonCloseDevice.Enabled = True
        ButtonStartGrabbing.Enabled = True
        ButtonStopGrabbing.Enabled = False
        ButtonSaveBmp.Enabled = False
        ButtonSaveJpg.Enabled = False
        ButtonSaveTiff.Enabled = False
        ButtonSavePng.Enabled = False
        TextBoxExposureTime.Enabled = True
        TextBoxGain.Enabled = True
        TextBoxFrameRate.Enabled = True
        ButtonParamGet.Enabled = True
        ButtonParamSet.Enabled = True

    End Sub

    ' ch:关闭设备 | en:Close device
    Private Sub ButtonCloseDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCloseDevice.Click
        If m_bIsGrabbing Then
            m_bIsGrabbing = False
            m_hGrabHandle.Join()
        End If

        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_CloseDevice_NET()
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to close device")
            Return
        End If

        nRet = dev.MV_CC_DestroyDevice_NET()
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to destroy handle")
            Return
        End If

        TextBoxExposureTime.Text = "0"
        TextBoxGain.Text = "0"
        TextBoxFrameRate.Text = "0"

        ' ch:设置各个控件的可见性 | en:Set visibility of controls
        ComboBoxDeviceList.Enabled = True
        ButtonOpenDevice.Enabled = True
        ButtonCloseDevice.Enabled = False

        GroupBoxGrabImage.Enabled = False
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = False
        ButtonSoftwareOnce.Enabled = False

        GroupBoxImageSave.Enabled = False
        GroupBoxParam.Enabled = False

    End Sub

    ' ch:刚加载时的初始化 | en:Initialization for loading
    Private Sub Thread_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Control.CheckForIllegalCrossThreadCalls = False
        ' ch:设置各个控件的可见性 | en:Set visibility of controls
        GroupBoxInit.Enabled = True
        GroupBoxDeviceControl.Enabled = False
        GroupBoxGrabImage.Enabled = False
        GroupBoxImageSave.Enabled = False
        GroupBoxParam.Enabled = False

        ComboBoxDeviceList.Enabled = False
        ButtonEnumDevice.Enabled = True
        ButtonOpenDevice.Enabled = False
        ButtonCloseDevice.Enabled = False
        RadioButtonTriggerOff.Enabled = False
        RadioButtonTriggerOn.Enabled = False
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = False
        CheckBoxSoftware.Enabled = False
        ButtonSoftwareOnce.Enabled = False
        ButtonSaveBmp.Enabled = False
        ButtonSaveJpg.Enabled = False
        ButtonSaveTiff.Enabled = False
        ButtonSavePng.Enabled = False
        TextBoxExposureTime.Enabled = False
        TextBoxGain.Enabled = False
        TextBoxFrameRate.Enabled = False
        ButtonParamGet.Enabled = False
        ButtonParamSet.Enabled = False
    End Sub

    ' ch:设置连续采集 | en:Set continuous acquisition
    Private Sub RadioButtonTriggerOff_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButtonTriggerOff.CheckedChanged
        ' ch:关闭触发 | en:Close trigger mode
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_SetEnumValue_NET("TriggerMode", MyCamera.MV_CAM_TRIGGER_MODE.MV_TRIGGER_MODE_OFF)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to close trigger mode")
        End If

        RadioButtonTriggerOff.Enabled = False
        RadioButtonTriggerOn.Enabled = True
        If (RadioButtonTriggerOn.Checked) Then
            CheckBoxSoftware.Enabled = True
        Else
            CheckBoxSoftware.Enabled = False
        End If

        If (RadioButtonTriggerOn.Checked And CheckBoxSoftware.Checked) Then
            ButtonSoftwareOnce.Enabled = True
        Else
            ButtonSoftwareOnce.Enabled = False
        End If

    End Sub

    ' ch:设置触发模式 | en:Set trigger mode
    Private Sub RadioButtonTriggerOn_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RadioButtonTriggerOn.CheckedChanged
        ' ch:开启触发 | en:Open trigger mode
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_SetEnumValue_NET("TriggerMode", MyCamera.MV_CAM_TRIGGER_MODE.MV_TRIGGER_MODE_ON)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to close trigger mode")
        End If

        RadioButtonTriggerOff.Enabled = True
        RadioButtonTriggerOn.Enabled = False
        If (RadioButtonTriggerOn.Checked) Then
            CheckBoxSoftware.Enabled = True
        Else
            CheckBoxSoftware.Enabled = False
        End If

        If (RadioButtonTriggerOn.Checked And CheckBoxSoftware.Checked) Then
            ButtonSoftwareOnce.Enabled = True
        Else
            ButtonSoftwareOnce.Enabled = False
        End If

    End Sub

    Sub ReceiveThreadProcess()
        Dim nRet As Int32
        Dim stIntValue As MyCamera.MVCC_INTVALUE = New MyCamera.MVCC_INTVALUE
        nRet = dev.MV_CC_GetIntValue_NET("PayloadSize", stIntValue)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Get PayloadSize failed!")
        End If

        If stIntValue.nCurValue > m_nBufSizeForDriver Then
            m_nBufSizeForDriver = stIntValue.nCurValue
            ReDim m_pBufForDriver(m_nBufSizeForDriver)
        End If

        Dim pData As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForDriver, 0)

        Dim stFrameInfo As MyCamera.MV_FRAME_OUT_INFO_EX = New MyCamera.MV_FRAME_OUT_INFO_EX()
        Dim stDisplayInfo As MyCamera.MV_DISPLAY_FRAME_INFO = New MyCamera.MV_DISPLAY_FRAME_INFO()

        While (m_bIsGrabbing)
            m_ReadWriteLock.AcquireWriterLock(System.Threading.Timeout.Infinite)
            nRet = dev.MV_CC_GetOneFrameTimeout_NET(pData, m_nBufSizeForDriver, stFrameInfo, 1000)
            If MyCamera.MV_OK = nRet Then
                m_stFrameInfo = stFrameInfo
            End If
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定

            If MyCamera.MV_OK = nRet Then
                If True = RemoveCustomPixelFormats(m_stFrameInfo.enPixelType) Then
                    Continue While
                End If
                stDisplayInfo.hWnd = PictureBoxDisplay.Handle
                stDisplayInfo.pData = pData
                stDisplayInfo.nDataLen = stFrameInfo.nFrameLen
                stDisplayInfo.nWidth = stFrameInfo.nWidth
                stDisplayInfo.nHeight = stFrameInfo.nHeight
                stDisplayInfo.enPixelType = stFrameInfo.enPixelType
                dev.MV_CC_DisplayOneFrame_NET(stDisplayInfo)
            Else
                If RadioButtonTriggerOn.Checked Then
                    Threading.Thread.Sleep(5)
                End If
            End If
        End While
        
    End Sub

    ' ch:开启采集 | en:Start grabbing
    Private Sub ButtonStartGrabbing_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonStartGrabbing.Click
        m_bIsGrabbing = True
        m_ReadWriteLock = New Threading.ReaderWriterLock()
        m_hGrabHandle = New Threading.Thread(AddressOf ReceiveThreadProcess)
        m_hGrabHandle.Start()

        m_stFrameInfo.nFrameLen = 0 '//取流之前先清除帧长度
        Dim nRet As Int32
        nRet = dev.MV_CC_StartGrabbing_NET()
        If MyCamera.MV_OK <> nRet Then
            m_bIsGrabbing = False
            m_hGrabHandle.Join()
            MsgBox("Fail to start grabbing")
        End If

        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = True
        ButtonSaveBmp.Enabled = True
        ButtonSaveJpg.Enabled = True
        ButtonSaveTiff.Enabled = True
        ButtonSavePng.Enabled = True

    End Sub

    ' ch:停止采集 | en:Stop grabbing
    Private Sub ButtonStopGrabbing_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonStopGrabbing.Click
        m_bIsGrabbing = False
        m_hGrabHandle.Join()

        Dim nRet As Int32
        nRet = dev.MV_CC_StopGrabbing_NET()
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to stop grabbing")
        End If
        ButtonStartGrabbing.Enabled = True
        ButtonStopGrabbing.Enabled = False
        ButtonSaveBmp.Enabled = False
        ButtonSaveJpg.Enabled = False
        ButtonSaveTiff.Enabled = False
        ButtonSavePng.Enabled = False

    End Sub

    ' ch:软触发模式 | en:Software trigger mode
    Private Sub CheckBoxSoftware_CheckedChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CheckBoxSoftware.CheckedChanged
        If (CheckBoxSoftware.Checked) Then
            ' ch:设置软触发 | en:Set software trigger
            Dim nRet As Int32
            nRet = dev.MV_CC_SetEnumValue_NET("TriggerSource", MyCamera.MV_CAM_TRIGGER_SOURCE.MV_TRIGGER_SOURCE_SOFTWARE)
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to set software trigger")
            End If
        Else
            ' ch:设置硬触发 | en:Set hardware trigger
            Dim nRet As Int32
            nRet = dev.MV_CC_SetEnumValue_NET("TriggerSource", MyCamera.MV_CAM_TRIGGER_SOURCE.MV_TRIGGER_SOURCE_LINE0)
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to set hardware trigger")
            End If
        End If

        CheckBoxSoftware.Enabled = True
        If (RadioButtonTriggerOn.Checked And CheckBoxSoftware.Checked) Then
            ButtonSoftwareOnce.Enabled = True
        Else
            ButtonSoftwareOnce.Enabled = False
        End If

    End Sub

    ' ch:软触发一次 | en:Software trigger once
    Private Sub ButtonSoftwareOnce_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSoftwareOnce.Click
        Dim nRet As Int32
        nRet = dev.MV_CC_SetCommandValue_NET("TriggerSoftware")
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to software trigger once")
        End If
    End Sub

    ' ch:保存bmp图片 | en:Save bmp image
    Private Sub ButtonSaveBmp_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSaveBmp.Click
        If False = m_bIsGrabbing Then
            MsgBox("Not Start Grabbing!")
            Return
        End If

        If True = RemoveCustomPixelFormats(m_stFrameInfo.enPixelType) Then
            MsgBox("Not Support!")
        End If

        Dim nSaveImageNeedSize As UInt32 = m_nBufSizeForDriver * 3 + 2048
        If nSaveImageNeedSize > m_nBufSizeForSaveImage Then
            m_nBufSizeForSaveImage = nSaveImageNeedSize
            ReDim m_pBufForSaveImage(m_nBufSizeForSaveImage)
        End If

        Dim stSaveImageParam As MyCamera.MV_SAVE_IMAGE_PARAM_EX = New MyCamera.MV_SAVE_IMAGE_PARAM_EX()

        m_ReadWriteLock.AcquireWriterLock(System.Threading.Timeout.Infinite)
        If m_stFrameInfo.nFrameLen = 0 Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Bmp Fail!")
            Return
        End If

        Dim nRet As Int32
        Dim pData As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForDriver, 0)
        Dim pImage As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForSaveImage, 0)
        stSaveImageParam.pData = pData
        stSaveImageParam.nDataLen = m_stFrameInfo.nFrameLen
        stSaveImageParam.enPixelType = m_stFrameInfo.enPixelType
        stSaveImageParam.nWidth = m_stFrameInfo.nWidth
        stSaveImageParam.nHeight = m_stFrameInfo.nHeight
        stSaveImageParam.enImageType = MyCamera.MV_SAVE_IAMGE_TYPE.MV_Image_Bmp
        stSaveImageParam.nBufferSize = m_nBufSizeForSaveImage
        stSaveImageParam.pImageBuffer = pImage
        nRet = dev.MV_CC_SaveImageEx_NET(stSaveImageParam)
        If (MyCamera.MV_OK <> nRet) Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Image fail!")
            Return
        End If
        m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定

        ' ch:写文件 | en:Write file
        Dim file_name As String = Application.StartupPath & "\Image.bmp"
        Dim file_stream As New FileStream(file_name, FileMode.Create)
        file_stream.Write(m_pBufForSaveImage, 0, stSaveImageParam.nImageLen)
        file_stream.Close()

        MsgBox("Save BMP succeed")
        Return

    End Sub

    ' ch:保存jpg图片 | en:Save jpg image
    Private Sub ButtonSaveJpg_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSaveJpg.Click
        If False = m_bIsGrabbing Then
            MsgBox("Not Start Grabbing!")
            Return
        End If

        If True = RemoveCustomPixelFormats(m_stFrameInfo.enPixelType) Then
            MsgBox("Not Support!")
        End If

        Dim nSaveImageNeedSize As UInt32 = m_nBufSizeForDriver * 3 + 2048
        If nSaveImageNeedSize > m_nBufSizeForSaveImage Then
            m_nBufSizeForSaveImage = nSaveImageNeedSize
            ReDim m_pBufForSaveImage(m_nBufSizeForSaveImage)
        End If

        Dim stSaveImageParam As MyCamera.MV_SAVE_IMAGE_PARAM_EX = New MyCamera.MV_SAVE_IMAGE_PARAM_EX()

        m_ReadWriteLock.AcquireWriterLock(System.Threading.Timeout.Infinite)
        If m_stFrameInfo.nFrameLen = 0 Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Jpeg Fail!")
            Return
        End If

        Dim nRet As Int32
        Dim pData As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForDriver, 0)
        Dim pImage As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForSaveImage, 0)
        stSaveImageParam.pData = pData
        stSaveImageParam.nDataLen = m_stFrameInfo.nFrameLen
        stSaveImageParam.enPixelType = m_stFrameInfo.enPixelType
        stSaveImageParam.nWidth = m_stFrameInfo.nWidth
        stSaveImageParam.nHeight = m_stFrameInfo.nHeight
        stSaveImageParam.enImageType = MyCamera.MV_SAVE_IAMGE_TYPE.MV_Image_Jpeg
        stSaveImageParam.nBufferSize = m_nBufSizeForSaveImage
        stSaveImageParam.pImageBuffer = pImage
        stSaveImageParam.nJpgQuality = 80
        nRet = dev.MV_CC_SaveImageEx_NET(stSaveImageParam)
        If (MyCamera.MV_OK <> nRet) Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Image fail!")
            Return
        End If
        m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定

        ' ch:写文件 | en:Write file
        Dim file_name As String = Application.StartupPath & "\Image.jpg"
        Dim file_stream As New FileStream(file_name, FileMode.Create)
        file_stream.Write(m_pBufForSaveImage, 0, stSaveImageParam.nImageLen)
        file_stream.Close()

        MsgBox("Save Jpeg succeed")
        Return
    End Sub

    ' ch:获取参数 | en:Get parameters
    Private Sub ButtonParamGet_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonParamGet.Click
        ' ch:获取曝光时间 | en:Get exposure time
        Dim nRet As Int32
        Dim stExposureTime As MyCamera.MVCC_FLOATVALUE = New MyCamera.MVCC_FLOATVALUE
        nRet = dev.MV_CC_GetFloatValue_NET("ExposureTime", stExposureTime)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to get exposure time")
        End If
        TextBoxExposureTime.Text = Convert.ToString(stExposureTime.fCurValue)
        ' ch:获取增益 | en:Get gain
        Dim stGain As MyCamera.MVCC_FLOATVALUE = New MyCamera.MVCC_FLOATVALUE
        nRet = dev.MV_CC_GetFloatValue_NET("Gain", stGain)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to get gain")
        End If
        TextBoxGain.Text = Convert.ToString(stGain.fCurValue)
        ' ch:获取帧率 | en:Get frame rate
        Dim stFrameRate As MyCamera.MVCC_FLOATVALUE = New MyCamera.MVCC_FLOATVALUE
        nRet = dev.MV_CC_GetFloatValue_NET("ResultingFrameRate", stFrameRate)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to get frame rate")
        End If
        TextBoxFrameRate.Text = Convert.ToString(stFrameRate.fCurValue)

    End Sub

    ' ch:设置参数 | en:Set Parameters
    Private Sub ButtonParamSet_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonParamSet.Click
        Dim fExposureTime As Single = 0
        Dim fGain As Single = 0
        Dim fFrameRate As Single = 0
        Try
            fExposureTime = Convert.ToSingle(TextBoxExposureTime.Text)
            fGain = Convert.ToSingle(TextBoxGain.Text)
            fFrameRate = Convert.ToSingle(TextBoxFrameRate.Text)
        Catch
            MsgBox("Incorrect parameter format")
            Return
        Finally

        End Try

        Dim nRet As Int32
        ' ch:将自动曝光和自动增益关闭 | en:Close auto-exposure and auto-gain
        If RadioButtonTriggerOff.Checked Then
            nRet = dev.MV_CC_SetEnumValue_NET("ExposureAuto", MyCamera.MV_CAM_EXPOSURE_AUTO_MODE.MV_EXPOSURE_AUTO_MODE_OFF)
            If MyCamera.MV_OK <> nRet Then
                MsgBox("Fail to close auto-exposure")
            End If
        End If

        nRet = dev.MV_CC_SetEnumValue_NET("GainAuto", MyCamera.MV_CAM_GAIN_MODE.MV_GAIN_MODE_OFF)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to close auto-gain")
        End If

        ' ch:设置曝光时间 | en:Set exposure time
        nRet = dev.MV_CC_SetFloatValue_NET("ExposureTime", fExposureTime)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to set exposure time")
        End If

        ' ch:设置增益 | en:Set gain
        nRet = dev.MV_CC_SetFloatValue_NET("Gain", fGain)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to set gain")
        End If

        ' ch:设置帧率 | en:Set frame rate
        nRet = dev.MV_CC_SetBoolValue_NET("AcquisitionFrameRateEnable", True)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Set frame rate enable fail")
        End If
        nRet = dev.MV_CC_SetFloatValue_NET("AcquisitionFrameRate", fFrameRate)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("Fail to set frame rate")
        End If

    End Sub

    ' ch:选择不同相机 | en:Select different cameras
    Private Sub ComboBoxDeviceList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBoxDeviceList.SelectedIndexChanged
        m_nDeviceIndex = ComboBoxDeviceList.SelectedIndex

        ' ch:设置各个控件的可见性 | en:Set visibility of controls
        GroupBoxInit.Enabled = True
        GroupBoxDeviceControl.Enabled = True
        GroupBoxGrabImage.Enabled = False
        GroupBoxImageSave.Enabled = False
        GroupBoxParam.Enabled = False

        ComboBoxDeviceList.Enabled = True
        ButtonEnumDevice.Enabled = True
        ButtonOpenDevice.Enabled = True
        ButtonCloseDevice.Enabled = False
        RadioButtonTriggerOff.Enabled = False
        RadioButtonTriggerOn.Enabled = False
        ButtonStartGrabbing.Enabled = False
        ButtonStopGrabbing.Enabled = False
        CheckBoxSoftware.Enabled = False
        ButtonSoftwareOnce.Enabled = False
        ButtonSaveBmp.Enabled = False
        ButtonSaveJpg.Enabled = False
        ButtonSaveTiff.Enabled = False
        ButtonSavePng.Enabled = False
        TextBoxExposureTime.Enabled = False
        TextBoxGain.Enabled = False
        TextBoxFrameRate.Enabled = False
        ButtonParamGet.Enabled = False
        ButtonParamSet.Enabled = False
    End Sub

    Private Sub Thread_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        If m_bIsGrabbing Then
            m_bIsGrabbing = False
            m_hGrabHandle.Join()
        End If

        ' ch:关闭设备 | en:Close device
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_CloseDevice_NET()
        ' ch:销毁句柄 | en:Destroy handle
        dev.MV_CC_DestroyDevice_NET()
    End Sub

    Private Sub ButtonSaveTiff_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSaveTiff.Click
        If False = m_bIsGrabbing Then
            MsgBox("Not Start Grabbing!")
            Return
        End If

        If True = RemoveCustomPixelFormats(m_stFrameInfo.enPixelType) Then
            MsgBox("Not Support!")
        End If

        Dim nSaveImageNeedSize As UInt32 = m_nBufSizeForDriver * 3 + 2048
        If nSaveImageNeedSize > m_nBufSizeForSaveImage Then
            m_nBufSizeForSaveImage = nSaveImageNeedSize
            ReDim m_pBufForSaveImage(m_nBufSizeForSaveImage)
        End If

        Dim stSaveImageParam As MyCamera.MV_SAVE_IMG_TO_FILE_PARAM = New MyCamera.MV_SAVE_IMG_TO_FILE_PARAM()

        m_ReadWriteLock.AcquireWriterLock(System.Threading.Timeout.Infinite)
        If m_stFrameInfo.nFrameLen = 0 Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Tiff Fail!")
            Return
        End If

        Dim nRet As Int32
        Dim pData As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForDriver, 0)
        Dim pImage As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForSaveImage, 0)
        stSaveImageParam.pData = pData
        stSaveImageParam.nDataLen = m_stFrameInfo.nFrameLen
        stSaveImageParam.enPixelType = m_stFrameInfo.enPixelType
        stSaveImageParam.nWidth = m_stFrameInfo.nWidth
        stSaveImageParam.nHeight = m_stFrameInfo.nHeight
        stSaveImageParam.enImageType = MyCamera.MV_SAVE_IAMGE_TYPE.MV_Image_Tif
        stSaveImageParam.iMethodValue = 0
        stSaveImageParam.pImagePath = "Image_w" + stSaveImageParam.nWidth.ToString() + "_h" + stSaveImageParam.nHeight.ToString() + "_fn" + m_stFrameInfo.nFrameNum.ToString() + ".tif"
        nRet = dev.MV_CC_SaveImageToFile_NET(stSaveImageParam)
        If (MyCamera.MV_OK <> nRet) Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Image fail!")
            Return
        End If
        m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定

        MsgBox("Save Tiff succeed")
        Return
    End Sub

    Private Sub ButtonSavePng_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSavePng.Click
        If False = m_bIsGrabbing Then
            MsgBox("Not Start Grabbing!")
            Return
        End If

        If True = RemoveCustomPixelFormats(m_stFrameInfo.enPixelType) Then
            MsgBox("Not Support!")
        End If

        Dim nSaveImageNeedSize As UInt32 = m_nBufSizeForDriver * 3 + 2048
        If nSaveImageNeedSize > m_nBufSizeForSaveImage Then
            m_nBufSizeForSaveImage = nSaveImageNeedSize
            ReDim m_pBufForSaveImage(m_nBufSizeForSaveImage)
        End If

        Dim stSaveImageParam As MyCamera.MV_SAVE_IMG_TO_FILE_PARAM = New MyCamera.MV_SAVE_IMG_TO_FILE_PARAM()

        m_ReadWriteLock.AcquireWriterLock(System.Threading.Timeout.Infinite)
        If m_stFrameInfo.nFrameLen = 0 Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Png Fail!")
            Return
        End If

        Dim nRet As Int32
        Dim pData As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForDriver, 0)
        Dim pImage As IntPtr = Marshal.UnsafeAddrOfPinnedArrayElement(m_pBufForSaveImage, 0)
        stSaveImageParam.pData = pData
        stSaveImageParam.nDataLen = m_stFrameInfo.nFrameLen
        stSaveImageParam.enPixelType = m_stFrameInfo.enPixelType
        stSaveImageParam.nWidth = m_stFrameInfo.nWidth
        stSaveImageParam.nHeight = m_stFrameInfo.nHeight
        stSaveImageParam.enImageType = MyCamera.MV_SAVE_IAMGE_TYPE.MV_Image_Png
        stSaveImageParam.nQuality = 8
        stSaveImageParam.iMethodValue = 0
        stSaveImageParam.pImagePath = "Image_w" + stSaveImageParam.nWidth.ToString() + "_h" + stSaveImageParam.nHeight.ToString() + "_fn" + m_stFrameInfo.nFrameNum.ToString() + ".png"
        nRet = dev.MV_CC_SaveImageToFile_NET(stSaveImageParam)
        If (MyCamera.MV_OK <> nRet) Then
            m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定
            MsgBox("Save Image fail!")
            Return
        End If
        m_ReadWriteLock.ReleaseWriterLock() ' 释放写入锁定

        MsgBox("Save Png succeed")
        Return
    End Sub
    ' ch:去除自定义的像素格式 | en:Remove custom pixel formats
    Function RemoveCustomPixelFormats(ByVal enPixelFormat As MyCamera.MvGvspPixelType) As Boolean
        Dim nResult As Int32
        nResult = enPixelFormat And (&H80000000)
        If &H80000000 <> nResult Then
            Return False
        Else
            Return True
        End If
    End Function
End Class
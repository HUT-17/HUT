Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Public Class SetIO
    Dim dev As MyCamera = New MyCamera

    Dim m_stDeviceInfoList As MyCamera.MV_CC_DEVICE_INFO_LIST = New MyCamera.MV_CC_DEVICE_INFO_LIST
    Dim m_stDeviceInfo As MyCamera.MV_CC_DEVICE_INFO = New MyCamera.MV_CC_DEVICE_INFO
    Dim m_handle As IntPtr
    Dim m_nDeviceIndex As Int32 = -1
    Dim m_bIsOpen As Boolean
    Dim m_bIsGrabbing As Boolean
    Dim m_stFrameOutInfo As MyCamera.MV_FRAME_OUT_INFO = New MyCamera.MV_FRAME_OUT_INFO()
    Dim m_pData As IntPtr = Marshal.AllocHGlobal(1024 * 1024 * 12)
    Dim m_pImageBuffer As IntPtr = Marshal.AllocHGlobal(1024 * 1024 * 12)
    Dim m_byteImageBuffer(1024 * 1024 * 12) As Byte


    Private Sub ButtonEnumDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonEnumDevice.Click
        ComboBoxDeviceList.Items.Clear()
        ComboBoxDeviceList.SelectedIndex = -1

        Dim Info As String
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE Or MyCamera.MV_USB_DEVICE), m_stDeviceInfoList)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("枚举设备失败!" + Convert.ToString(nRet))
            Return
        End If


        If (0 = m_stDeviceInfoList.nDeviceNum) Then
            MsgBox("No Find Gige | Usb Device !")
            Return
        End If

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

                Info = "DEV[" + Convert.ToString(i) + "] NAME[" + stGigeInfo.chUserDefinedName + "]IP[" + nIpByte1.ToString() + "." + nIpByte2.ToString() + "." + nIpByte3.ToString() + "." + nIpByte4.ToString() + "]"
                ComboBoxDeviceList.Items.Add(Info)
            Else
                Dim stUsbInfoPtr As IntPtr = Marshal.AllocHGlobal(540)
                Marshal.Copy(stDeviceInfo.SpecialInfo.stUsb3VInfo, 0, stUsbInfoPtr, 540)
                Dim stUsbInfo As MyCamera.MV_USB3_DEVICE_INFO
                stUsbInfo = CType(Marshal.PtrToStructure(stUsbInfoPtr, GetType(MyCamera.MV_USB3_DEVICE_INFO)), MyCamera.MV_USB3_DEVICE_INFO)
                Info = "DEV[" + Convert.ToString(i) + "] NAME[" + stUsbInfo.chUserDefinedName + "]Model[" + stUsbInfo.chSerialNumber + "]"
                ComboBoxDeviceList.Items.Add(Info)
            End If
        Next i

        ButtonOpenDevice.Enabled = True
    End Sub

    Private Sub ButtonOpenDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonOpenDevice.Click
        ' ch:创建句柄 | en:Create handle
        If m_nDeviceIndex = -1 Then
            MsgBox("请先选择设备!")
            Return
        End If

        Dim nRet As Int32 = MyCamera.MV_OK
        m_stDeviceInfo = CType(Marshal.PtrToStructure(m_stDeviceInfoList.pDeviceInfo(m_nDeviceIndex), GetType(MyCamera.MV_CC_DEVICE_INFO)), MyCamera.MV_CC_DEVICE_INFO)
        nRet = dev.MV_CC_CreateDevice_NET(m_stDeviceInfo)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("创建句柄失败!")
        End If

        ' ch:打开设备 | en:Open device
        nRet = dev.MV_CC_OpenDevice_NET()
        If MyCamera.MV_OK <> nRet Then
            dev.MV_CC_DestroyDevice_NET()
            MsgBox("Open device failed")
            Return
        End If

        ButtonOpenDevice.Enabled = False
        ButtonCloseDevice.Enabled = True
        ButtonSetMode.Enabled = True
        ButtonGetMode.Enabled = True
        ButtonSetSelector.Enabled = True
        ButtonGetSelector.Enabled = True
    End Sub

    Private Sub ButtonCloseDevice_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonCloseDevice.Click

        ' ch:关闭设备 | en:Close device
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_CloseDevice_NET()
        If MyCamera.MV_OK <> nRet Then
            MsgBox("关闭设备失败!")
        End If

        nRet = dev.MV_CC_DestroyDevice_NET()
        If MyCamera.MV_OK <> nRet Then
            MsgBox("销毁句柄失败!")
        End If

        ButtonOpenDevice.Enabled = True
        ButtonCloseDevice.Enabled = False
        ButtonSetMode.Enabled = False
        ButtonGetMode.Enabled = False
        ButtonSetSelector.Enabled = False
        ButtonGetSelector.Enabled = False
    End Sub

    Private Sub ComboBoxDeviceList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBoxDeviceList.SelectedIndexChanged
        m_nDeviceIndex = ComboBoxDeviceList.SelectedIndex
    End Sub

    Private Sub SetIO_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        ButtonEnumDevice.Enabled = True
        ButtonOpenDevice.Enabled = False
        ButtonCloseDevice.Enabled = False
        ButtonSetMode.Enabled = False
        ButtonGetMode.Enabled = False
        ButtonSetSelector.Enabled = False
        ButtonGetSelector.Enabled = False
    End Sub

    Private Sub ComboBoxLineMode_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBoxLineMode.SelectedIndexChanged

    End Sub

    Private Sub ButtonGetSelector_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonGetSelector.Click
        ' ch:获取LineSelector | en:Get LineSelector
        Dim nRet As Int32
        Dim stLineSelector As MyCamera.MVCC_ENUMVALUE = New MyCamera.MVCC_ENUMVALUE
        nRet = dev.MV_CC_GetEnumValue_NET("LineSelector", stLineSelector)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("获取LineSelector失败!")
        End If

        ComboBoxLineSelector.Items.Clear()
        Dim i As UInt32 = 0
        For i = 0 To stLineSelector.nSupportedNum - 1
            ComboBoxLineSelector.Items.Add("LineSelector" + stLineSelector.nSupportValue(i).ToString())
            If (stLineSelector.nSupportValue(i) = stLineSelector.nCurValue) Then
                ComboBoxLineSelector.SelectedIndex = i
            End If
        Next
    End Sub

    Private Sub ButtonSetSelector_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSetSelector.Click
        If ComboBoxLineSelector.SelectedIndex < 0 Then
            MsgBox("I/O选择不正确!")
            Return
        End If
        ' ch:获取LineSelector | en:Get LineSelector
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stLineSelector As MyCamera.MVCC_ENUMVALUE = New MyCamera.MVCC_ENUMVALUE
        nRet = dev.MV_CC_GetEnumValue_NET("LineSelector", stLineSelector)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("获取LineSelector失败!")
        End If
        ' ch:设置LineSelector | en:Set LineSelector
        nRet = dev.MV_CC_SetEnumValue_NET("LineSelector", stLineSelector.nSupportValue(ComboBoxLineSelector.SelectedIndex))
        If MyCamera.MV_OK <> nRet Then
            MsgBox("设置LineSelector失败!")
        End If
    End Sub

    Private Sub ButtonGetMode_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonGetMode.Click
        ' ch:获取LineMode | en:Get LineMode
        Dim nRet As Int32
        Dim stLineMode As MyCamera.MVCC_ENUMVALUE = New MyCamera.MVCC_ENUMVALUE
        nRet = dev.MV_CC_GetEnumValue_NET("LineMode", stLineMode)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("获取LineMode失败!")
        End If
        ComboBoxLineMode.Items.Clear()
        Dim j As UInt32 = 0
        For j = 0 To stLineMode.nSupportedNum - 1
            ComboBoxLineMode.Items.Add("LineMode" + stLineMode.nSupportValue(j).ToString())
            If (stLineMode.nSupportValue(j) = stLineMode.nCurValue) Then
                ComboBoxLineMode.SelectedIndex = j
            End If
        Next
    End Sub

    Private Sub ButtonSetMode_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ButtonSetMode.Click
        If ComboBoxLineMode.SelectedIndex < 0 Then
            MsgBox("I/O模式不正确!")
            Return
        End If
        ' ch:获取LineMode | en:Get LineMode
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stLineMode As MyCamera.MVCC_ENUMVALUE = New MyCamera.MVCC_ENUMVALUE
        nRet = dev.MV_CC_GetEnumValue_NET("LineMode", stLineMode)
        If MyCamera.MV_OK <> nRet Then
            MsgBox("获取LineMode失败!")
        End If
        ' ch:设置LineMode | en:Set LineMode
        nRet = dev.MV_CC_SetEnumValue_NET("LineMode", stLineMode.nSupportValue(ComboBoxLineMode.SelectedIndex))
        If MyCamera.MV_OK <> nRet Then
            MsgBox("设置LineMode失败!")
        End If
    End Sub

    Private Sub SetIO_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        ' ch:关闭设备 | en:Close device
        Dim nRet As Int32 = MyCamera.MV_OK
        nRet = dev.MV_CC_CloseDevice_NET()
        ' ch:销毁句柄 | en:Destroy handle
        dev.MV_CC_DestroyDevice_NET()
    End Sub
End Class
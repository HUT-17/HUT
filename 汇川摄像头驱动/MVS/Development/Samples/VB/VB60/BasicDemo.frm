VERSION 5.00
Begin VB.Form BasicDemo 
   Caption         =   "BasicDemo_VB6"
   ClientHeight    =   9480
   ClientLeft      =   10065
   ClientTop       =   6330
   ClientWidth     =   11790
   LinkTopic       =   "Form1"
   ScaleHeight     =   9480
   ScaleWidth      =   11790
   Begin VB.ComboBox DEVICE_COMBO 
      Height          =   300
      Left            =   480
      TabIndex        =   24
      Top             =   600
      Width           =   6375
   End
   Begin VB.PictureBox PICTURE_DISPLAY 
      Height          =   7575
      Left            =   240
      ScaleHeight     =   7515
      ScaleWidth      =   6915
      TabIndex        =   4
      Top             =   1440
      Width           =   6975
   End
   Begin VB.Frame Frame4 
      Caption         =   "参数"
      Height          =   2655
      Left            =   7680
      TabIndex        =   3
      Top             =   6480
      Width           =   3855
      Begin VB.CommandButton SET_PARAMETER_BUTTON 
         Caption         =   "参数设置"
         Height          =   495
         Left            =   2160
         TabIndex        =   23
         Top             =   1920
         Width           =   1335
      End
      Begin VB.CommandButton GET_PARAMETER_BUTTON 
         Caption         =   "参数获取"
         Height          =   495
         Left            =   480
         TabIndex        =   22
         Top             =   1920
         Width           =   1215
      End
      Begin VB.TextBox FRAME_RATE_EDIT 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   2052
            SubFormatType   =   1
         EndProperty
         Height          =   375
         Left            =   1680
         TabIndex        =   18
         Top             =   1320
         Width           =   1575
      End
      Begin VB.TextBox GAIN_EDIT 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   2052
            SubFormatType   =   1
         EndProperty
         Height          =   375
         Left            =   1680
         TabIndex        =   17
         Top             =   720
         Width           =   1575
      End
      Begin VB.TextBox EXPOSURE_EDIT 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0.0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   2052
            SubFormatType   =   1
         EndProperty
         Height          =   375
         Left            =   1680
         TabIndex        =   16
         Top             =   240
         Width           =   1575
      End
      Begin VB.Label Label3 
         Caption         =   "帧率"
         Height          =   375
         Left            =   720
         TabIndex        =   21
         Top             =   1440
         Width           =   735
      End
      Begin VB.Label Label2 
         Caption         =   "增益"
         Height          =   255
         Left            =   720
         TabIndex        =   20
         Top             =   840
         Width           =   735
      End
      Begin VB.Label Label1 
         Caption         =   "曝光"
         Height          =   375
         Left            =   720
         TabIndex        =   19
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "图片保存"
      Height          =   1215
      Left            =   7680
      TabIndex        =   2
      Top             =   4920
      Width           =   3855
      Begin VB.CommandButton SAVE_JPG_BUTTON 
         Caption         =   "保存JPG"
         Height          =   495
         Left            =   2280
         TabIndex        =   15
         Top             =   480
         Width           =   1335
      End
      Begin VB.CommandButton SAVE_BMP_BUTTON 
         Caption         =   "保存BMP"
         Height          =   495
         Left            =   480
         TabIndex        =   14
         Top             =   480
         Width           =   1335
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "图像采集"
      Height          =   2175
      Left            =   7680
      TabIndex        =   1
      Top             =   2520
      Width           =   3855
      Begin VB.CommandButton SOFTWARE_ONCE_BUTTON 
         Caption         =   "软触发一次"
         Height          =   375
         Left            =   2280
         TabIndex        =   13
         Top             =   1680
         Width           =   1215
      End
      Begin VB.CheckBox SOFTWARE_TRIGGER_CHECK 
         Caption         =   "软触发"
         Height          =   375
         Left            =   600
         TabIndex        =   12
         Top             =   1680
         Width           =   1095
      End
      Begin VB.CommandButton STOP_GRABBING_BUTTON 
         Caption         =   "停止采集"
         Height          =   375
         Left            =   2160
         TabIndex        =   11
         Top             =   1080
         Width           =   1335
      End
      Begin VB.CommandButton START_GRABBING_BUTTON 
         Caption         =   "开始采集"
         Height          =   375
         Left            =   480
         TabIndex        =   10
         Top             =   1080
         Width           =   1095
      End
      Begin VB.OptionButton TRIGGER_MODE_RADIO 
         Caption         =   "触发模式"
         Height          =   255
         Left            =   2280
         TabIndex        =   9
         Top             =   480
         Width           =   1335
      End
      Begin VB.OptionButton CONTINUS_MODE_RADIO 
         Caption         =   "连续模式"
         Height          =   255
         Left            =   480
         TabIndex        =   8
         Top             =   480
         Width           =   1575
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "初始化"
      Height          =   1935
      Left            =   7680
      TabIndex        =   0
      Top             =   360
      Width           =   3855
      Begin VB.CommandButton CLOSE_BUTTON 
         Caption         =   "关闭设备"
         Height          =   495
         Left            =   2280
         TabIndex        =   7
         Top             =   1080
         Width           =   1455
      End
      Begin VB.CommandButton ENUM_BUTTON 
         Caption         =   "查找设备"
         Default         =   -1  'True
         Height          =   495
         Left            =   720
         TabIndex        =   5
         Top             =   360
         Width           =   2655
      End
      Begin VB.CommandButton OPEN_BUTTON 
         Caption         =   "打开设备"
         Height          =   495
         Left            =   480
         TabIndex        =   6
         Top             =   1080
         Width           =   1575
      End
   End
End
Attribute VB_Name = "BasicDemo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' 全局变量定义
Dim m_bCreateDevice As Boolean
Dim m_bOpenDevice As Boolean
Dim m_bStartGrabbing As Boolean
Dim m_nTriggerMode As Long
Dim m_nTriggerSource As Long
Dim m_nSaveImageType As MV_SAVE_IAMGE_TYPE
Dim m_stDevList As MV_CC_DEVICE_INFO_LIST
Dim m_handle As Long
Dim m_pHandle As Long

' 关闭设备时的窗口显示
Private Sub EnableWindowWhenClose()
    OPEN_BUTTON.Enabled = False
    CLOSE_BUTTON.Enabled = False
    START_GRABBING_BUTTON.Enabled = False
    STOP_GRABBING_BUTTON.Enabled = False
    SOFTWARE_TRIGGER_CHECK.Enabled = False
    SOFTWARE_ONCE_BUTTON.Enabled = False
    SAVE_BMP_BUTTON.Enabled = False
    SAVE_JPG_BUTTON.Enabled = False
    EXPOSURE_EDIT.Enabled = False
    GAIN_EDIT.Enabled = False
    FRAME_RATE_EDIT.Enabled = False
    GET_PARAMETER_BUTTON.Enabled = False
    SET_PARAMETER_BUTTON.Enabled = False
End Sub

' 打开设备但不开始抓图
Private Sub EnableWindowWhenOpenNotStart()
    OPEN_BUTTON.Enabled = False
    CLOSE_BUTTON.Enabled = True
    START_GRABBING_BUTTON.Enabled = True
    SOFTWARE_TRIGGER_CHECK.Enabled = True
    EXPOSURE_EDIT.Enabled = True
    GAIN_EDIT.Enabled = True
    FRAME_RATE_EDIT.Enabled = True
    GET_PARAMETER_BUTTON.Enabled = True
    SET_PARAMETER_BUTTON.Enabled = True
    CONTINUS_MODE_RADIO.Enabled = True
    TRIGGER_MODE_RADIO.Enabled = True
End Sub

' 按下开始采集按钮时的按钮颜色
Private Sub EnableWindowWhenStart()
    STOP_GRABBING_BUTTON.Enabled = True
    START_GRABBING_BUTTON.Enabled = False
    SAVE_BMP_BUTTON.Enabled = True
    SAVE_JPG_BUTTON.Enabled = True
    SOFTWARE_ONCE_BUTTON.Enabled = True
End Sub

' 按下结束采集按钮时的按钮颜色
Private Sub EnableWindowWhenStop()
    STOP_GRABBING_BUTTON.Enabled = False
    START_GRABBING_BUTTON.Enabled = True
    SAVE_BMP_BUTTON.Enabled = False
    SAVE_JPG_BUTTON.Enabled = False
    SOFTWARE_ONCE_BUTTON.Enabled = False
End Sub

' 用来获取函数地址
Private Function GetAddr(ByVal addr As Long) As Long
     GetAddr = addr
End Function

Private Sub Reset()
    m_bCreateDevice = False
    m_bOpenDevice = False
    m_bStartGrabbing = False
    m_nTriggerMode = -1
    m_nTriggerSource = -1
End Sub

Private Sub Form_Load()
    Call EnableWindowWhenClose
    Call Reset
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    'If MsgBox("真的退出吗?", vbQuestion + vbYesNo) = vbYes Then
       'Cancel = False
    'Else
       'Cancel = True
    'End If
    'Call CLOSE_BUTTON_Click
    If False = m_bCreateDevice Or False = m_bOpenDevice Then
        Exit Sub
    End If
    Dim nRet As Long
    nRet = MV_CC_DestroyHandle(m_pHandle)
 End Sub
 
' 关闭相机
Private Sub CLOSE_BUTTON_Click()
    If False = m_bCreateDevice Or False = m_bOpenDevice Then
        Exit Sub
    End If
    Dim nRet As Long
    nRet = MV_CC_StopGrabbing(m_pHandle)
    nRet = MV_CC_CloseDevice(m_pHandle)
    nRet = MV_CC_DestroyHandle(m_pHandle)
    m_bOpenDevice = False
    m_bStartGrabbing = False
    m_bCreateDevice = False
    Call EnableWindowWhenClose
    OPEN_BUTTON.Enabled = True
End Sub
' 设置连续模式
Private Sub CONTINUS_MODE_RADIO_Click()
    Dim nRet As Long
    nRet = MV_CC_SetEnumValue(m_pHandle, "TriggerMode", MV_TRIGGER_MODE_OFF)
    If MV_OK <> nRet Then
        MsgBox ("CONTINUS_MODE_RADIO_Click error")
    End If
    SOFTWARE_ONCE_BUTTON.Enabled = False
End Sub

' 按下查找设备按钮:枚举
Private Sub ENUM_BUTTON_Click()
    Dim nRet As Long
    DEVICE_COMBO.Clear  ' 清除设备列表框中的信息
    Dim stDeviceInfo As MV_CC_DEVICE_INFO
    Dim pStDeviceInfo As Long
    pStDeviceInfo = VarPtr(stDeviceInfo)
    
    nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE Or MV_USB_DEVICE, m_stDevList)
    If MV_OK <> nRet Then
        MsgBox ("MV_CC_EnumDevices error")
    End If
    
    
    For i = 0 To m_stDevList.nDeviceNum - 1
        CopyMemory ByVal pStDeviceInfo, ByVal m_stDevList.pDeviceInfo(i), LenB(stDeviceInfo)
        If MV_GIGE_DEVICE = stDeviceInfo.nTLayerType Then
            Dim stGigEInfo As MV_GIGE_DEVICE_INFO
            Dim pstGigEInfo As Long
            Dim nIp1, nIp2, nIp3, nIp4 As Integer

            pstGigEInfo = VarPtr(stGigEInfo)
            CopyMemory ByVal pstGigEInfo, stDeviceInfo.SpecialInfo(0), LenB(stGigEInfo)
            Dim chGigEName As Long
            chGigEName = VarPtr(stGigEInfo.chUserDefinedName(0))
            Dim pchGigEName As String * INFO_MAX_BUFFER_SIZE
            CopyMemory ByVal pchGigEName, ByVal chGigEName, INFO_MAX_BUFFER_SIZE
            Dim str As String
            If 0 < stGigEInfo.nCurrentIp Then
                nIp4 = stGigEInfo.nCurrentIp Mod 256
                nIp3 = stGigEInfo.nCurrentIp \ 256 Mod 256
                nIp2 = stGigEInfo.nCurrentIp \ 256 \ 256 Mod 256
                nIp1 = stGigEInfo.nCurrentIp \ 256 \ 256 \ 256 Mod 1000
                str = "GigE:" + "[" + CStr(nIp1) + "." + CStr(nIp2) + "." + CStr(nIp3) + "." + CStr(nIp4) + "]" + " " + pchGigEName
            ElseIf 0 > stGigEInfo.nCurrentIp Then
                 nIp4 = (stGigEInfo.nCurrentIp And &H7F000000) \ &H1000000 Or (((stGigEInfo.nCurrentIp And &H80000000) <> 0) And &H80)
                 nIp3 = (stGigEInfo.nCurrentIp And &H7F0000) \ &H10000 Or (((stGigEInfo.nCurrentIp And &H800000) <> 0) And &H80)
                 nIp2 = (stGigEInfo.nCurrentIp And &H7F00) \ &H100 Or (((stGigEInfo.nCurrentIp And &H8000) <> 0) And &H80) - 128
                 nIp1 = (stGigEInfo.nCurrentIp And &H7F) \ &H1 Or (((stGigEInfo.nCurrentIp And &H80) <> 0) And &H80)
                 str = "GigE:" + "[" + CStr(nIp4) + "." + CStr(nIp3) + "." + CStr(nIp2) + "." + CStr(nIp1) + "]" + " " + pchGigEName
            End If
            DEVICE_COMBO.AddItem (str)
        ElseIf MV_USB_DEVICE = stDeviceInfo.nTLayerType Then
            Dim stUSBInfo As MV_USB3_DEVICE_INFO
            Dim pstUSBInfo As Long
            pstUSBInfo = VarPtr(stUSBInfo)
            CopyMemory ByVal pstUSBInfo, stDeviceInfo.SpecialInfo(0), LenB(stUSBInfo)

            Dim chName As Long
            chName = VarPtr(stUSBInfo.chModelName(0))
            Dim pchName As String * INFO_MAX_BUFFER_SIZE
            CopyMemory ByVal pchName, ByVal chName, INFO_MAX_BUFFER_SIZE
            DEVICE_COMBO.AddItem ("USB:" + pchName)
        End If
    Next i
    If m_stDevList.nDeviceNum > 0 Then
        DEVICE_COMBO.ListIndex = 0
        OPEN_BUTTON.Enabled = True
    End If
End Sub

' 获取参数
Private Sub GET_PARAMETER_BUTTON_Click()
    Dim nRet As Long
    Dim stFloatValue As MVCC_FLOATVALUE
    nRet = MV_CC_GetFloatValue(m_pHandle, "ExposureTime", stFloatValue)
    If MV_OK <> nRet Then
        MsgBox ("Get ExposureTime error")
    End If
    EXPOSURE_EDIT.Text = stFloatValue.fCurValue
    
    nRet = MV_CC_GetFloatValue(m_pHandle, "Gain", stFloatValue)
    If MV_OK <> nRet Then
        MsgBox ("Get Gain error")
    End If
    GAIN_EDIT.Text = stFloatValue.fCurValue
    
    nRet = MV_CC_GetFloatValue(m_pHandle, "ResultingFrameRate", stFloatValue)
    If MV_OK <> nRet Then
        MsgBox ("Get ResultingFrameRate error")
    End If
    FRAME_RATE_EDIT.Text = stFloatValue.fCurValue
    
End Sub
' 打开相机
Private Sub OPEN_BUTTON_Click()
    If True = m_bOpenDevice Then
        Exit Sub
    End If
    Dim stDeviceInfo As MV_CC_DEVICE_INFO
    Dim pStDeviceInfo As Long
    Dim nRet As Long
    
    
    pStDeviceInfo = VarPtr(stDeviceInfo)
    CopyMemory ByVal pStDeviceInfo, ByVal m_stDevList.pDeviceInfo(DEVICE_COMBO.ListIndex), LenB(stDeviceInfo)
    
    m_pHandle = VarPtr(m_handle)
    nRet = MV_CC_CreateHandle(m_pHandle, pStDeviceInfo)
    If MV_OK <> nRet Then
        MsgBox ("MV_CC_CreateHandle error")
        Exit Sub
    End If
    m_bCreateDevice = True
    nRet = MV_CC_OpenDevice(m_pHandle, 1, 0)
    If MV_OK <> nRet Then
        MV_CC_DestroyHandle (m_pHandle)
        MsgBox ("MV_CC_OpenDevice error")
    Exit Sub
    End If
    m_bOpenDevice = True
    

    ' 获取参数------------------------
    Call EnableWindowWhenOpenNotStart
End Sub
' 保存BMP
Private Sub SAVE_BMP_BUTTON_Click()
    
    If False = m_bStartGrabbing Then
        Exit Sub
    End If
    Dim fid As Integer
    fid = FreeFile()  '获取一个可用的文件号
    Open "Data.bmp" For Binary As #fid

    Dim pData(MAX_BUF) As Byte
    Dim ppData As Long
    ppData = VarPtr(pData(0))
    Dim nTestNum As Long
    Dim stFrameInfo As MV_FRAME_OUT_INFO_EX
    nTestNum = 500
    

    For i = 0 To nTestNum - 1
        nRet = MV_CC_GetOneFrameTimeout(m_pHandle, ppData, MAX_BUF, stFrameInfo, 1000)
        If MV_OK = nRet Then
            '保存图片
            Dim pImageBuf(MAX_BUF) As Byte
            Dim ppImageBuf As Long
            ppImageBuf = VarPtr(pImageBuf(0))
            Dim stParam As MV_SAVE_IMAGE_PARAM_EX
            stParam.enImageType = MV_Image_Bmp  '需要保存的图像类型
            stParam.enPixelType = stFrameInfo.enPixelType   '相机对应的像素格式
            stParam.nBufferSize = MAX_BUF  '存储节点的大小
            stParam.nWidth = stFrameInfo.nWidth         '相机对应的宽
            stParam.nHeight = stFrameInfo.nHeight       '相机对应的高
            stParam.nDataLen = stFrameInfo.nFrameLen
            stParam.pData = ppData
            stParam.pImageBuffer = ppImageBuf
            
            nRet = MV_CC_SaveImageEx2(m_pHandle, stParam)
            If MV_OK <> nRet Then
                MsgBox ("MV_CC_SaveImage error")
                Exit Sub
            End If
            
            For j = 0 To stParam.nImageLen - 1 Step 1
                Put #fid, , pImageBuf(j)
            Next j
            
            Close #fid
            Set ts = Nothing
            MsgBox ("Save Bmp succeed!")
            Exit Sub
        Else
            Sleep (4)
        End If
    Next i
    
    MsgBox ("No Data!")
    Close #fid
    Set ts = Nothing
End Sub
' 保存JPG
Private Sub SAVE_JPG_BUTTON_Click()
    If False = m_bStartGrabbing Then
        Exit Sub
    End If
    Dim fid As Integer
    fid = FreeFile()  '获取一个可用的文件号
    Open "Data.jpg" For Binary As #fid

    Dim pData(MAX_BUF) As Byte
    Dim ppData As Long
    ppData = VarPtr(pData(0))
    Dim nTestNum As Long
    Dim stFrameInfo As MV_FRAME_OUT_INFO_EX
    nTestNum = 500
    

    For i = 0 To nTestNum - 1
        nRet = MV_CC_GetOneFrameTimeout(m_pHandle, ppData, MAX_BUF, stFrameInfo, 1000)
        If MV_OK = nRet Then
            '保存图片
            Dim pImageBuf(MAX_BUF) As Byte
            Dim ppImageBuf As Long
            ppImageBuf = VarPtr(pImageBuf(0))
            Dim stParam As MV_SAVE_IMAGE_PARAM_EX
            stParam.enImageType = MV_Image_Jpeg  '需要保存的图像类型
            stParam.enPixelType = stFrameInfo.enPixelType   '相机对应的像素格式
            stParam.nBufferSize = MAX_BUF  '存储节点的大小
            stParam.nWidth = stFrameInfo.nWidth         '相机对应的宽
            stParam.nHeight = stFrameInfo.nHeight       '相机对应的高
            stParam.nDataLen = stFrameInfo.nFrameLen
            stParam.pData = ppData
            stParam.nJpgQuality = 80
            stParam.pImageBuffer = ppImageBuf
            
            nRet = MV_CC_SaveImageEx2(m_pHandle, stParam)
            If MV_OK <> nRet Then
                MsgBox ("MV_CC_SaveImage error")
                Exit Sub
            End If
            
            For j = 0 To stParam.nImageLen - 1 Step 1
                Put #fid, , pImageBuf(j)
            Next j
            
            Close #fid
            Set ts = Nothing
            MsgBox ("Save Jpg succeed!")
            Exit Sub
        Else
            Sleep (4)
        End If
    Next i
    
    MsgBox ("No Data!")
    Close #fid
    Set ts = Nothing
End Sub
' 设置参数
Private Sub SET_PARAMETER_BUTTON_Click()
    Dim nRet As Long
    nRet = MV_CC_SetEnumValue(m_pHandle, "ExposureMode", MV_EXPOSURE_MODE_TIMED)
    nRet = MV_CC_SetEnumValue(m_pHandle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF)
    nRet = MV_CC_SetFloatValue(m_pHandle, "ExposureTime", EXPOSURE_EDIT.Text)
    If MV_OK <> nRet Then
        MsgBox ("Set ExposureTime error")
    End If
    
    nRet = MV_CC_SetEnumValue(m_pHandle, "GainAuto", 0)
    nRet = MV_CC_SetFloatValue(m_pHandle, "Gain", GAIN_EDIT.Text)
    If MV_OK <> nRet Then
        MsgBox ("Set Gain error")
    End If
    
    nRet = MV_CC_SetFloatValue(m_pHandle, "AcquisitionFrameRate", FRAME_RATE_EDIT.Text)
    If MV_OK <> nRet Then
        MsgBox ("Set AcquisitionFrameRate error")
    End If
End Sub
' 软触发一次
Private Sub SOFTWARE_ONCE_BUTTON_Click()
    If True <> m_bStartGrabbing Then
        Exit Sub
    End If
    Dim nRet As Long
    nRet = MV_CC_SetCommandValue(m_pHandle, "TriggerSoftware")
    If MV_OK <> nRet Then
        MsgBox ("Set TriggerSoftware error")
        Exit Sub
    End If
End Sub
' 设置软触发
Private Sub SOFTWARE_TRIGGER_CHECK_Click()
    Dim nRet As Long
    If 1 = SOFTWARE_TRIGGER_CHECK.Value Then
        nRet = MV_CC_SetEnumValue(m_pHandle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE)
        If MV_OK <> nRet Then
            MsgBox ("Set TriggerSource error")
            Exit Sub
        End If
        SOFTWARE_ONCE_BUTTON.Enabled = True
    Else
        nRet = MV_CC_SetEnumValue(m_pHandle, "TriggerSource", MV_TRIGGER_SOURCE_LINE0)
        If MV_OK <> nRet Then
            MsgBox ("Set TriggerSource error")
            Exit Sub
        End If
        SOFTWARE_ONCE_BUTTON.Enabled = False
    End If
    
End Sub
' 开始取流
Private Sub START_GRABBING_BUTTON_Click()
    If False = m_bCreateDevice Or False = m_bOpenDevice Or True = m_bStartGrabbing Then
        Exit Sub
    End If
    Dim nRet As Long
    nRet = MV_CC_StartGrabbing(m_pHandle)
    If MV_OK <> nRet Then
        MsgBox ("MV_CC_StopGrabbing error")
        Exit Sub
    End If
    
    nRet = MV_CC_Display(m_pHandle, PICTURE_DISPLAY.hWnd)
    If MV_OK <> nRet Then
        MsgBox ("MV_CC_Display error")
        Exit Sub
    End If
    
    m_bStartGrabbing = True
    Call EnableWindowWhenStart
            
End Sub
' 停止取流
Private Sub STOP_GRABBING_BUTTON_Click()
    If False = m_bCreateDevice Or False = m_bOpenDevice Or False = m_bStartGrabbing Then
        Exit Sub
    End If
    Dim nRet As Long
    nRet = MV_CC_StopGrabbing(m_pHandle)
    If MV_OK <> nRet Then
        MsgBox ("MV_CC_StopGrabbing error")
        Exit Sub
    End If
    m_bStartGrabbing = False
    Call EnableWindowWhenStop
End Sub
' 设置触发
Private Sub TRIGGER_MODE_RADIO_Click()
    Dim nRet As Long
    nRet = MV_CC_SetEnumValue(m_pHandle, "TriggerMode", MV_TRIGGER_MODE_ON)
    If MV_OK <> nRet Then
        MsgBox ("TRIGGER_MODE_RADIO_Click error")
    End If
    If m_bStartGrabbing = True And SOFTWARE_TRIGGER_CHECK.Value = 1 Then
        SOFTWARE_ONCE_BUTTON.Enabled = True
    End If
End Sub

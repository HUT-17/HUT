Attribute VB_Name = "CameraInterface"
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Public Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Declare Function VirtualAlloc Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long, ByVal flAllocationType As Long, ByVal flProtect As Long) As Long
'''''''''''''''''''''''常量定义''''''''''''''''''''''''''''''''''''''
'正确码定义
Public Const MV_OK = &H0                        ' 成功，无错误
'通用错误码定义:范围0x80000000-0x800000FF
Public Const MV_E_HANDLE = &H80000000           ' 错误或无效的句柄
Public Const MV_E_SUPPORT = &H80000001          ' 不支持的功能
Public Const MV_E_BUFOVER = &H80000002          ' 缓存已满
Public Const MV_E_CALLORDER = &H80000003        ' 函数调用顺序错误
Public Const MV_E_PARAMETER = &H80000004        ' 错误的参数
Public Const MV_E_RESOURCE = &H80000006         ' 资源申请失败
Public Const MV_E_NODATA = &H80000007           ' 无数据
Public Const MV_E_PRECONDITION = &H80000008     ' 前置条件有误，或运行环境已发生变化
Public Const MV_E_VERSION = &H80000009          ' 版本不匹配
Public Const MV_E_NOENOUGH_BUF = &H8000000A     ' 传入的内存空间不足
Public Const MV_E_UNKNOW = &H800000FF           ' 未知的错误
' GenICam系列错误:范围= &H80000100-&H800001FF
Public Const MV_E_GC_GENERIC = &H80000100       ' 通用错误
Public Const MV_E_GC_ARGUMENT = &H80000101      ' 参数非法
Public Const MV_E_GC_RANGE = &H80000102         ' 值超出范围
Public Const MV_E_GC_PROPERTY = &H80000103      ' 属性
Public Const MV_E_GC_RUNTIME = &H80000104       ' 运行环境有问题
Public Const MV_E_GC_LOGICAL = &H80000105       ' 逻辑错误
Public Const MV_E_GC_ACCESS = &H80000106        ' 节点访问条件有误
Public Const MV_E_GC_TIMEOUT = &H80000107       ' 超时
Public Const MV_E_GC_DYNAMICCAST = &H80000108   ' 转换异常
Public Const MV_E_GC_UNKNOW = &H800001FF        ' GenICam未知错误
'GigE_STATUS对应的错误码:范围= &H80000200-&H800002FF
Public Const MV_E_NOT_IMPLEMENTED = &H80000200  ' 命令不被设备支持
Public Const MV_E_INVALID_ADDRESS = &H80000201  ' 访问的目标地址不存在
Public Const MV_E_WRITE_PROTECT = &H80000202    ' 目标地址不可写
Public Const MV_E_ACCESS_DENIED = &H80000203    ' 设备无访问权限
Public Const MV_E_BUSY = &H80000204             ' 设备忙，或网络断开
Public Const MV_E_PACKET = &H80000205           ' 网络包数据错误
Public Const MV_E_NETER = &H80000206            ' 网络相关错误
'USB_STATUS对应的错误码:范围0x80000300-0x800003FF
Public Const MV_E_USB_READ = &H80000300         ' 读usb出错
Public Const MV_E_USB_WRITE = &H80000301        ' 写usb出错
Public Const MV_E_USB_DEVICE = &H80000302       ' 设备异常
Public Const MV_E_USB_GENICAM = &H80000303      ' GenICam相关错误
Public Const MV_E_USB_BANDWIDTH = &H80000304    ' 带宽不足  该错误码新增
Public Const MV_E_USB_UNKNOW = &H800003FF       ' USB未知的错误
'升级时对应的错误码:范围= &H80000400-= &H800004FF
Public Const MV_E_UPG_FILE_MISMATCH = &H80000400 ' 升级固件不匹配
Public Const MV_E_UPG_LANGUSGE_MISMATCH = &H80000401    ' 升级固件语言不匹配
Public Const MV_E_UPG_CONFLICT = &H80000402     ' 升级冲突（设备已经在升级了再次请求升级即返回此错误）
Public Const MV_E_UPG_INNER_ERR = &H80000403    ' 升级时相机内部出现错误
Public Const MV_E_UPG_UNKNOW = &H800004FF       ' 升级时未知错误
' 设备类型定义
Public Const MV_UNKNOW_DEVICE = &H0             ' 未知设备类型，保留意义
Public Const MV_GIGE_DEVICE = &H1               ' GigE设备
Public Const MV_1394_DEVICE = &H2               ' 1394-a/b 设备
Public Const MV_USB_DEVICE = &H4                ' USB3.0 设备
Public Const MV_CAMERALINK_DEVICE = &H8          ' CameraLink设备
Public Const INFO_MAX_BUFFER_SIZE = 64
Public Const MV_MAX_TLS_NUM = 8                 ' 最多支持的传输层实例个数
Public Const MV_MAX_DEVICE_NUM = 256            ' 最大支持的设备个数
' GigEVision IP Configuration
Public Const MV_IP_CFG_STATIC = &H5000000       '静态
Public Const MV_IP_CFG_DHCP = &H6000000         'DHCP
Public Const MV_IP_CFG_LLA = &H4000000          'LLA
' GigEVision Net Transfer Mode
Public Const MV_NET_TRANS_DRIVER = &H1          '驱动
Public Const MV_NET_TRANS_SOCKET = &H2          'Socket
' 信息类型
Public Const MV_MATCH_TYPE_NET_DETECT = &H1      ' 网络流量和丢包信息
Public Const MV_MATCH_TYPE_USB_DETECT = &H2      ' host接收到来自U3V设备的字节总数
'属性树宏定义
Public Const MV_MAX_XML_NODE_NUM_C = 128        ' 某个节点对应的子节点个数最大值
Public Const MV_MAX_XML_NODE_STRLEN_C = 64      ' 节点名称字符串最大长度
Public Const MV_MAX_XML_STRVALUE_STRLEN_C = 64  '节点String值最大长度
Public Const MV_MAX_XML_DISC_STRLEN_C = 512     ' 节点描述字段最大长度
Public Const MV_MAX_XML_ENTRY_NUM = 10          ' 最多的单元数
Public Const MV_MAX_XML_PARENTS_NUM = 8         ' 父节点个数上限
Public Const MV_MAX_XML_SYMBOLIC_STRLEN_C = 64  '每个已经实现单元的名称长度
Public Const MV_MAX_XML_SYMBOLIC_NUM = 64
'  异常消息类型
Public Const MV_EXCEPTION_DEV_DISCONNECT = &H8001      ' 设备断开连接
Public Const MV_EXCEPTION_VERSION_CHECK = &H8002      ' SDK与驱动版本不匹配
' 设备的访问模式
Public Const MV_ACCESS_Exclusive = 1            ' 独占权限，其他APP只允许读CCP寄存器
Public Const MV_ACCESS_ExclusiveWithSwitch = 2  ' 可以从5模式下抢占权限，然后以独占权限打开
Public Const MV_ACCESS_Control = 3              ' 控制权限，其他APP允许读所有寄存器
Public Const MV_ACCESS_ControlWithSwitch = 4    ' 可以从5的模式下抢占权限，然后以控制权限打开
Public Const MV_ACCESS_ControlSwitchEnable = 5  ' 以可被抢占的控制权限打开
Public Const MV_ACCESS_ControlSwitchEnableWithKey = 6 ' 可以从5的模式下抢占权限，然后以可被抢占的控制权限打开
Public Const MV_ACCESS_Monitor = 7              ' 读模式打开设备，适用于控制权限下

Public Const MAX_BUF = 68157440                 ' 65M

''''''''''''''''''''''''''''''''''''''''枚举定义'''''''''''''''''''''''''''''''''''''''''
Public Enum MvGvspPixelType                     ' 像素格式定义
    PixelType_Gvsp_Undefined = -1
    PixelType_Gvsp_Mono1p = &H1010037
    PixelType_Gvsp_Mono2p = &H1020038
    PixelType_Gvsp_Mono4p = &H1040039
    PixelType_Gvsp_Mono8 = &H1080001
    PixelType_Gvsp_Mono8_Signed = &H1080002
    PixelType_Gvsp_Mono10 = &H1100003
    PixelType_Gvsp_Mono10_Packed = &H10C0004
    PixelType_Gvsp_Mono12 = &H1100005
    PixelType_Gvsp_Mono12_Packed = &H10C0006
    PixelType_Gvsp_Mono14 = &H1100025
    PixelType_Gvsp_Mono16 = &H1100007
    PixelType_Gvsp_BayerGR8 = &H1080008
    PixelType_Gvsp_BayerRG8 = &H1080009
    PixelType_Gvsp_BayerGB8 = &H108000A
    PixelType_Gvsp_BayerBG8 = &H108000B
    PixelType_Gvsp_BayerGR10 = &H110000C
    PixelType_Gvsp_BayerRG10 = &H110000D
    PixelType_Gvsp_BayerGB10 = &H110000E
    PixelType_Gvsp_BayerBG10 = &H110000F
    PixelType_Gvsp_BayerGR12 = &H1100010
    PixelType_Gvsp_BayerRG12 = &H1100011
    PixelType_Gvsp_BayerGB12 = &H1100012
    PixelType_Gvsp_BayerBG12 = &H1100013
    PixelType_Gvsp_BayerGR10_Packed = &H10C0026
    PixelType_Gvsp_BayerRG10_Packed = &H10C0027
    PixelType_Gvsp_BayerGB10_Packed = &H10C0028
    PixelType_Gvsp_BayerBG10_Packed = &H10C0029
    PixelType_Gvsp_BayerGR12_Packed = &H10C002A
    PixelType_Gvsp_BayerRG12_Packed = &H10C002B
    PixelType_Gvsp_BayerGB12_Packed = &H10C002C
    PixelType_Gvsp_BayerBG12_Packed = &H10C002D
    PixelType_Gvsp_BayerGR16 = &H110002E
    PixelType_Gvsp_BayerRG16 = &H110002F
    PixelType_Gvsp_BayerGB16 = &H1100030
    PixelType_Gvsp_BayerBG16 = &H1100031
    PixelType_Gvsp_RGB8_Packed = &H2180014
    PixelType_Gvsp_BGR8_Packed = &H2180015
    PixelType_Gvsp_RGBA8_Packed = &H2200016
    PixelType_Gvsp_BGRA8_Packed = &H2200017
    PixelType_Gvsp_RGB10_Packed = &H2300018
    PixelType_Gvsp_BGR10_Packed = &H2300019
    PixelType_Gvsp_RGB12_Packed = &H230001A
    PixelType_Gvsp_BGR12_Packed = &H230001B
    PixelType_Gvsp_RGB16_Packed = &H2300033
    PixelType_Gvsp_RGB10V1_Packed = &H220001C
    PixelType_Gvsp_RGB10V2_Packed = &H220001D
    PixelType_Gvsp_RGB12V1_Packed = &H2240034
    PixelType_Gvsp_RGB565_Packed = &H2100035
    PixelType_Gvsp_BGR565_Packed = &H2100036
    PixelType_Gvsp_YUV411_Packed = &H20C001E
    PixelType_Gvsp_YUV422_Packed = &H210001F
    PixelType_Gvsp_YUV422_YUYV_Packed = &H2100032
    PixelType_Gvsp_YUV444_Packed = &H2180020
    PixelType_Gvsp_YCBCR8_CBYCR = &H218003A
    PixelType_Gvsp_YCBCR422_8 = &H210003B
    PixelType_Gvsp_YCBCR422_8_CBYCRY = &H2100043
    PixelType_Gvsp_YCBCR411_8_CBYYCRYY = &H20C003C
    PixelType_Gvsp_YCBCR601_8_CBYCR = &H218003D
    PixelType_Gvsp_YCBCR601_422_8 = &H210003E
    PixelType_Gvsp_YCBCR601_422_8_CBYCRY = &H2100044
    PixelType_Gvsp_YCBCR601_411_8_CBYYCRYY = &H20C003F
    PixelType_Gvsp_YCBCR709_8_CBYCR = &H2180040
    PixelType_Gvsp_YCBCR709_422_8 = &H2100041
    PixelType_Gvsp_YCBCR709_422_8_CBYCRY = &H2100045
    PixelType_Gvsp_YCBCR709_411_8_CBYYCRYY = &H20C0042
    PixelType_Gvsp_RGB8_Planar = &H2180021
    PixelType_Gvsp_RGB10_Planar = &H2300022
    PixelType_Gvsp_RGB12_Planar = &H2300023
    PixelType_Gvsp_RGB16_Planar = &H2300024
End Enum


Public Enum MV_CAM_ACQUISITION_MODE        '采集模式结构体
    MV_ACQ_MODE_SINGLE = 0                 ' 单帧模式
    MV_ACQ_MODE_MUTLI = 1                  ' 多帧模式
    MV_ACQ_MODE_CONTINUOUS = 2             ' 持续采集模式
End Enum

Public Enum MV_CAM_GAIN_MODE               '增益模式结构体
    MV_GAIN_MODE_OFF = 0                   ' 关闭
    MV_GAIN_MODE_ONCE = 1                  ' 一次
    MV_GAIN_MODE_CONTINUOUS = 2            ' 连续
End Enum

Public Enum MV_CAM_EXPOSURE_MODE                '曝光模式结构体
    MV_EXPOSURE_MODE_TIMED = 0                  ' Timed
    MV_EXPOSURE_MODE_TRIGGER_WIDTH = 1          ' TriggerWidth
End Enum

Public Enum MV_CAM_EXPOSURE_AUTO_MODE           '自动曝光模式结构体
    MV_EXPOSURE_AUTO_MODE_OFF = 0               ' 关闭
    MV_EXPOSURE_AUTO_MODE_ONCE = 1              ' 一次
    MV_EXPOSURE_AUTO_MODE_CONTINUOUS = 2        ' 连续
End Enum

Public Enum MV_CAM_TRIGGER_MODE                '触发模式结构体
    MV_TRIGGER_MODE_OFF = 0                    ' 关闭
    MV_TRIGGER_MODE_ON = 1                     ' 打开
End Enum
        
Public Enum MV_CAM_GAMMA_SELECTOR               '触发模式结构体
    MV_GAMMA_SELECTOR_USER = 1                  '用户
    MV_GAMMA_SELECTOR_SRGB = 2                  'sRGB
End Enum

Public Enum MV_CAM_BALANCEWHITE_AUTO            '白平衡结构体
    MV_BALANCEWHITE_AUTO_OFF = 0                '关闭
    MV_BALANCEWHITE_AUTO_ONCE = 2               '一次
    MV_BALANCEWHITE_AUTO_CONTINUOUS = 1         ' 连续
End Enum

Public Enum MV_CAM_TRIGGER_SOURCE               '触发源结构体
    MV_TRIGGER_SOURCE_LINE0 = 0                 'Line0
    MV_TRIGGER_SOURCE_LINE1 = 1                 'Line1
    MV_TRIGGER_SOURCE_LINE2 = 2                 'Line2
    MV_TRIGGER_SOURCE_LINE3 = 3                 'Line3
    MV_TRIGGER_SOURCE_COUNTER0 = 4              'Counter0

    MV_TRIGGER_SOURCE_SOFTWARE = 7              '软触发
    MV_TRIGGER_SOURCE_FrequencyConverter = 8    '变频器
End Enum

Public Enum MV_GRAB_STRATEGY                    '取流策略结构体
    MV_GrabStrategy_OneByOne                    '从旧到新一帧一帧的获取图像（默认为该策略）
    MV_GrabStrategy_LatestImagesOnly = 1        '获取列表中最新的一帧图像（同时清除列表中的其余图像）
    MV_GrabStrategy_LatestImages = 2            '获取列表中最新的图像，个数由OutputQueueSize决定，范围为1-ImageNodeNum，设置成1等同于LatestImagesOnly,设置成ImageNodeNum等同于OneByOne
    MV_GrabStrategy_UpcomingImage = 3           '等待下一帧图像
End Enum

Public Enum MV_SAVE_IAMGE_TYPE                  '保存图片格式
    MV_Image_Undefined = 0                      '未定义的图像格式
    MV_Image_Bmp = 1                            'BMP图像格式
    MV_Image_Jpeg = 2                           'JPEG图像格式
    MV_Image_Png = 3                            'PNG图像格式
    MV_Image_Tif = 4                            'TIFF图像格式
End Enum

Public Enum MV_SAVE_POINT_CLOUD_FILE_TYPE       '保存图片格式
    MV_PointCloudFile_Undefined = 0             '未定义的点云格式
    MV_PointCloudFile_PLY = 1                   'PLY点云格式
    MV_PointCloudFile_CSV = 2                   'CSV点云格式
    MV_PointCloudFile_OBJ = 3                   'OBJ点云格式
End Enum

'''''''''''''''''''''''''''''''''结构体定义'''''''''''''''''''''''''''''''''

Public Type MV_GIGE_DEVICE_INFO            'Gige设备信息结构体
    nIpCfgOption As Long                   'IP配置选项
    nIpCfgCurrent As Long                  '当前IP配置
    nCurrentIp As Long                     '当前IP地址
    nCurrentSubNetMask As Long             '当前子网掩码
    nDefultGateWay As Long                 '当前网关
    chManufacturerName(31) As Byte         '制造商名称
    chModelName(31) As Byte                '型号名称
    chDeviceVersion(31) As Byte            '设备版本
    chManufacturerSpecificInfo(47) As Byte '制造商的具体信息
    chSerialNumber(15) As Byte             '序列号
    chUserDefinedName(15) As Byte          '用户自定义名称
    nNetExport As Long                     '网口IP地址
    nReserved(3) As Long                   '预留
End Type

Public Type MV_USB3_DEVICE_INFO            'U口相机设备信息结构体
    CrtlInEndPoint As Byte                 '控制输入端点
    CrtlOutEndPoint As Byte                '控制输出端点
    StreamEndPoint As Byte                 '流端点
    EventEndPoint As Byte                  '事件端点
    idVendor As Integer                    '供应商ID号
    idProduct As Integer                   '产品ID号
    nDeviceNumber As Long                  '设备序列号
    chDeviceGUID(INFO_MAX_BUFFER_SIZE - 1) As Byte       '设备GUID号
    chVendorName(INFO_MAX_BUFFER_SIZE - 1) As Byte       '供应商名字
    chModelName(INFO_MAX_BUFFER_SIZE - 1) As Byte        '型号名字
    chFamilyName(INFO_MAX_BUFFER_SIZE - 1) As Byte       '家族名字
    chDeviceVersion(INFO_MAX_BUFFER_SIZE - 1) As Byte    '设备版本
    chManufacturerName(INFO_MAX_BUFFER_SIZE - 1) As Byte '制造商名字
    chSerialNumber(INFO_MAX_BUFFER_SIZE - 1) As Byte     '序列号
    chUserDefinedName(INFO_MAX_BUFFER_SIZE - 1) As Byte  '用户自定义名字
    nbcdUSB As Long                                      '支持的USB协议
    nDeviceAddress As Long                               '设备地址
    nReserved(2) As Long                                 '预留
End Type

Public Type MV_CC_DEVICE_INFO              '设备信息结构体
    nMajorVer As Integer                   '主要版本
    nMinorVer As Integer                   '次要版本
    nMacAddrHigh(3) As Byte                '高MAC地址
    nMacAddrLow(3) As Byte                 '低MAC地址
    nTLayerType As Long                    '设备传输层协议类型
    nReserved(3) As Long                   '预留
    SpecialInfo(539) As Byte               '所有设备信息联合体
End Type

Public Type MV_CC_DEVICE_INFO_LIST         '设备信息结构体
    nDeviceNum As Long                     '设备数量
    pDeviceInfo(255) As Long               '设备信息
End Type

Public Type MV_FRAME_OUT_INFO              '图像信息结构体
    nWidth As Integer                      '图像宽
    nHeight As Integer                     '图像高
    enPixelType As MvGvspPixelType         '像素格式
    nFrameNum As Long                      '帧号
    nDevTimeStampHigh As Long              '时间戳高32位
    nDevTimeStampLow As Long               '时间戳低32位
    nReserved0 As Long                     '预留
    nHostTimeStamp As Double               '主机时间戳
    nFrameLen As Long                      '帧的长度
    nReserved(2) As Long                   '预留
End Type

Public Type MV_CHUNK_DATA_CONTENT
    pChunkData As Long                   'Chunk数据
    nChunkID As Long                     'ChunkId
    nChunkLen  As Long                   'Chunk的长度
    nReversed(7) As Byte                 '预留
End Type

Public Type MV_FRAME_OUT_INFO_EX         '图像信息结构体
    nWidth As Integer                    '图像宽
    nHeight As Integer                   '图像高
    enPixelType As MvGvspPixelType       '像素格式
    nFrameNum As Long                    '帧号
    nDevTimeStampHigh As Long            '时间戳高32位
    nDevTimeStampLow As Long             '时间戳低32位
    nReserved0 As Long                   '保留，8字节对齐
    nHostTimeStamp As Double             '主机生成的时间戳
    nFrameLen As Long                    '帧的长度
    nSecondCount As Long                 '秒数
    nCycleCount As Long                  '周期数
    nCycleOffset As Long                 '周期偏移量
    fGain As Long                        '增益
    fExposureTime As Long                '曝光时间
    nAverageBrightness As Long           '平均亮度
    nRed As Long                         '红色
    nGreen As Long                       '绿色
    nBlue As Long                        '蓝色
    nFrameCounter As Long                '总帧数
    nTriggerIndex As Long                '触发计数
    nInput As Long                       '输入
    nOutput As Long                      '输出
    nOffsetX As Integer                  '水平偏移量
    nOffsetY As Integer                  '垂直偏移量
    nChunkWidth As Integer               'Chunk宽
    nChunkHeight As Integer              'Chunk高
    nLostPacket As Long                  '本帧丢包数
    nUnparsedChunkNum As Long            '未解析的ChunkData个数
    UnparsedChunkList(7)  As Byte        '未解析的ChunkChunk
    nReserved(35) As Long                '预留
End Type

Public Type MV_FRAME_OUT                    '图像结构体，输出图像指针地址及图像信息
    pBufAddr As Long                        '图像指针地址
    stFrameInfo As MV_FRAME_OUT_INFO_EX     '图像信息  MV_FRAME_OUT_INFO_EX类型
    nRes(15) As Long                        '预留
End Type

Public Type MVCC_INTVALUE               'int类型结构体
    nCurValue As Long                   '当前值
    nMax As Long                        '最大值
    nMin As Long                        '最小值
    nInc As Long                        '步进间距
    nReserved(3) As Long                '预留
End Type

Public Type MVCC_FLOATVALUE               'Float类型结构体
    fCurValue As Single                   '当前值
    fMax As Single                        '最大值
    fMin As Single                        '最小值
    nReserved(3) As Long                  '预留
End Type

Public Type MVCC_STRINGVALUE               'String类型结构体
    chCurValue(255) As Byte                '当前值
    nReserved(3) As Long                   '预留
End Type

Public Type MVCC_ENUMVALUE                '枚举类型结构体
    nCurValue As Long                     '当前值
    nSupportedNum As Long                 '数据的有效数据个数
    nSupportValue(MV_MAX_XML_SYMBOLIC_NUM) As Long '支持的枚举值
    nReserved(3) As Long
End Type

Public Type MV_EVENT_OUT_INFO          '事件信息结构体
    EventName As String                'Event名称
    nEventID As Integer                'Event号
    nStreamChannel As Integer          '流通道序号
    nBlockIdHigh As Long               '帧号高位
    nBlockIdLow As Long                '帧号低位
    nTimestampHigh  As Long            '时间戳高位
    nTimestampLow As Long              '时间戳低位
    pEventData As Long                 'Event数据
    nEventDataSize As Long             'Event数据长度
    nReserved(16) As Long              '预留
    
End Type

Public Type MV_SAVE_IMAGE_PARAM        '保存图片参数
    pData As Long                      '输入数据缓存
    nDataLen As Long                   '输入数据大小
    enPixelType As MvGvspPixelType     '输入数据的像素格式
    nWidth As Integer                  '图像宽
    nHeight As Integer                 '图像高
    pImageBuffer As Long               '输出图片缓存
    nImageLen As Long                  '输出图片大小
    nBufferSize As Long                '提供的输出缓冲区大小
    enImageType As MV_SAVE_IAMGE_TYPE   '输出图片格式
End Type

Public Type MV_SAVE_IMAGE_PARAM_EX      '保存图片参数
    pData As Long                       '输入数据缓存
    nDataLen As Long                    '输入数据大小
    enPixelType As MvGvspPixelType      '输入数据的像素格式
    nWidth As Integer                   '图像宽
    nHeight As Integer                  '图像高
    pImageBuffer As Long                '输出图片缓存
    nImageLen As Long                   '输出图片大小
    nBufferSize As Long                 '提供的输出缓冲区大小
    enImageType As MV_SAVE_IAMGE_TYPE   '输出图片格式
    nJpgQuality As Long                 'JPG编码质量(50-99]，其它格式无效
    iMethodValue As Long                '插值方法
    nReserved(3) As Long                '预留
End Type

Public Type MV_NETTRANS_INFO            '网络传输的相关信息
    nReviceDataSize As Double           '已接收数据大小 [统计StartGrabbing和StopGrabbing之间的数据量
    nThrowFrameCount As Long            '丢帧数量
    nNetRecvFrameCount As Long          '已接收的帧数
    nRequestResendPacketCount As Long   '请求重发包数
    nResendPacketCount  As Long         '重发包数
End Type

Public Type MV_DISPLAY_FRAME_INFO
    hWnd  As Long                       '窗口句柄
    pData As Long                       '显示的数据
    nDataLen As Integer                 '数据长度
    nWidth As Integer                   '图像宽
    nHeight As Integer                  '图像高
    enPixelType As MvGvspPixelType      '像素格式
    nReserved(3) As Long                '预留
End Type

Public Type MV_SAVE_IMG_TO_FILE_PARAM      '保存BMP、JPEG、PNG、TIFF图片文件的参数
    enPixelType As MvGvspPixelType      '输入数据的像素格式
    pData As Long                       '输入数据缓存
    nDataLen As Long                    '输入数据大小
    nWidth As Integer                   '图像宽
    nHeight As Integer                  '图像高
    enImageType As MV_SAVE_IAMGE_TYPE   '输出图片格式
    nQuality As Long                    'JPG编码质量(50-99]，PNG编码质量[0-9]，其它格式无效
    pImagePath(255) As Byte             '输入文件路径
    iMethodValue As Long                '插值方法
    nReserved(7) As Long                '预留
End Type

Public Type MV_SAVE_POINT_CLOUD_PARAM                     '保存3D数据到缓存结构体
    nLinePntNum As Long                                   '每一行点的数量，即图像宽
    nLineNum As Long                                      '行数，即图像高
    enSrcPixelType As MvGvspPixelType                     '输入数据的像素格式
    pSrcData As Long                                      '输入数据缓存
    nSrcDataLen As Long                                   '输入数据大小
    pDstBuf As Long                                       '输出像素数据缓存
    nDstBufSize As Long                                   '提供的输出缓冲区大小(nLinePntNum * nLineNum * (16*3 + 4) + 2048)
    nDstBufLen As Long                                    '输出像素数据缓存长度
    enPointCloudFileType As MV_SAVE_POINT_CLOUD_FILE_TYPE '提供输出的点云文件类型
    nReserved(7) As Long                                  '保留字段
End Type

Public Type MV_CC_PIXEL_CONVERT_PARAM       '图像转换结构体
    nWidth As Integer                       '图像宽
    nHeight As Integer                      '图像高
    enSrcPixelType As MvGvspPixelType       '源像素格式
    pSrcData As Long                        '输入数据缓存
    nSrcDataLen As Long                     '输入数据大小
    enDstPixelType As MvGvspPixelType       '目标像素格式
    pDstBuffer As Long                      '输出数据缓存
    nDstLen As Long                         '输出数据大小
    nDstBufferSize As Long                  '提供的输出缓冲区大小
    nRes(3) As Long                         '预留
End Type

Public Type MV_CC_FILE_ACCESS       '文件存取结构体
    pUserFileName As Long           '用户文件名
    pDevFileName As Long            '设备文件名
    nReserved(32) As Long           '预留
End Type


' 函数声明

'获取SDK版本号
Public Declare Function MV_CC_GetSDKVersion Lib "MvCameraControl.dll" _
() As Long

'枚举设备
Public Declare Function MV_CC_EnumDevices Lib "MvCameraControl.dll" _
(ByVal nTLayerType As Long, ByRef pstDevList As MV_CC_DEVICE_INFO_LIST) As Long

'创建设备句柄
Public Declare Function MV_CC_CreateHandle Lib "MvCameraControl.dll" _
(ByRef handle As Long, ByVal stDeviceInfo As Long) As Long

'销毁设备句柄
Public Declare Function MV_CC_DestroyHandle Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'打开设备
Public Declare Function MV_CC_OpenDevice Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal nAccessMode As Long, ByVal nSwitchoverKey As Integer) As Long

'关闭设备
Public Declare Function MV_CC_CloseDevice Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'开始取流
Public Declare Function MV_CC_StartGrabbing Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'显示图像，注册显示窗口，内部自动显示（与MV_CC_GetImageBuffer不能同时使用，建议改用MV_CC_DisplayOneFrame接口）
Public Declare Function MV_CC_Display Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal hWnd As Long) As Long

'获取一帧图像，此函数为查询式获取，每次调用查询内部缓存有无数据，有数据则范围数据，无数据返回错误码
Public Declare Function MV_CC_GetOneFrame Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal pData As Long, ByVal nDataSize As Long, ByRef pFrameInfo As MV_FRAME_OUT_INFO) As Long

'采用超时机制获取一帧图片，SDK内部等待直到有数据时返回
Public Declare Function MV_CC_GetOneFrameTimeout Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal pData As Long, ByVal nDataSize As Long, ByRef pFrameInfo As MV_FRAME_OUT_INFO_EX, ByVal nMsec As Long) As Long

'停止取流
Public Declare Function MV_CC_StopGrabbing Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'获取Integer属性值
Public Declare Function MV_CC_GetIntValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_INTVALUE) As Long

'设置Integer型属性值
Public Declare Function MV_CC_SetIntValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal nValue As Long) As Long

'获取Enum属性值
Public Declare Function MV_CC_GetEnumValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_ENUMVALUE) As Long

'设置Enum型属性值
Public Declare Function MV_CC_SetEnumValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal nValue As Long) As Long

'获取Boolean属性值
Public Declare Function MV_CC_GetBoolValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pbValue As Boolean) As Long

'设置Boolean型属性值
Public Declare Function MV_CC_SetBoolValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal bValue As Boolean) As Long

'获取String属性值
Public Declare Function MV_CC_GetStringValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_STRINGVALUE) As Long

'设置String型属性值
Public Declare Function MV_CC_SetStringValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal strValue As String) As Long

'获取Float属性值
Public Declare Function MV_CC_GetFloatValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_FLOATVALUE) As Long

'设置float型属性值
Public Declare Function MV_CC_SetFloatValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal fValue As Single) As Long

'设置Command型属性值
Public Declare Function MV_CC_SetCommandValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String) As Long

'保存图片，支持Bmp和Jpeg.
'通过将接口可以将从设备采集到的原始图像数据转换成JPEG或者BMP等格式并存放在指定内存中，然后用户可以将转换之后的数据直接保存成图片文件。该接口调用无接口顺序要求，有图像源数据就可以进行转换，可以先调用MV_CC_GetOneFrameTimeout或者MV_CC_RegisterImageCallBackEx设置回调函数，获取一帧图像数据，然后再通过该接口转换格式。 \n
'MV_CC_SaveImageEx2比MV_CC_SaveImageEx增加参数handle，为了保证与其他接口的统一。
Public Declare Function MV_CC_SaveImageEx2 Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByRef pSaveParam As MV_SAVE_IMAGE_PARAM_EX) As Long

'显示一帧图像
Public Declare Function MV_CC_DisplayOneFrame Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByRef pstDisplayInfo As MV_DISPLAY_FRAME_INFO) As Long

'设置SDK内部图像缓存节点个数，大于等于1，在抓图前调用
Public Declare Function MV_CC_SetImageNodeNum Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal nNum As Long) As Long


'像素格式转换
Public Declare Function MV_CC_ConvertPixelType Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByRef pstDisplayInfo As MV_CC_PIXEL_CONVERT_PARAM) As Long


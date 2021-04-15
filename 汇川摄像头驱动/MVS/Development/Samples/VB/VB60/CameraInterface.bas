Attribute VB_Name = "CameraInterface"
Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Public Declare Function GetProcAddress Lib "kernel32" (ByVal hModule As Long, ByVal lpProcName As String) As Long
Public Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Public Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (lpDest As Any, lpSource As Any, ByVal cBytes As Long)
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Declare Function VirtualAlloc Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long, ByVal flAllocationType As Long, ByVal flProtect As Long) As Long
'''''''''''''''''''''''��������''''''''''''''''''''''''''''''''''''''
'��ȷ�붨��
Public Const MV_OK = &H0                        ' �ɹ����޴���
'ͨ�ô����붨��:��Χ0x80000000-0x800000FF
Public Const MV_E_HANDLE = &H80000000           ' �������Ч�ľ��
Public Const MV_E_SUPPORT = &H80000001          ' ��֧�ֵĹ���
Public Const MV_E_BUFOVER = &H80000002          ' ��������
Public Const MV_E_CALLORDER = &H80000003        ' ��������˳�����
Public Const MV_E_PARAMETER = &H80000004        ' ����Ĳ���
Public Const MV_E_RESOURCE = &H80000006         ' ��Դ����ʧ��
Public Const MV_E_NODATA = &H80000007           ' ������
Public Const MV_E_PRECONDITION = &H80000008     ' ǰ���������󣬻����л����ѷ����仯
Public Const MV_E_VERSION = &H80000009          ' �汾��ƥ��
Public Const MV_E_NOENOUGH_BUF = &H8000000A     ' ������ڴ�ռ䲻��
Public Const MV_E_UNKNOW = &H800000FF           ' δ֪�Ĵ���
' GenICamϵ�д���:��Χ= &H80000100-&H800001FF
Public Const MV_E_GC_GENERIC = &H80000100       ' ͨ�ô���
Public Const MV_E_GC_ARGUMENT = &H80000101      ' �����Ƿ�
Public Const MV_E_GC_RANGE = &H80000102         ' ֵ������Χ
Public Const MV_E_GC_PROPERTY = &H80000103      ' ����
Public Const MV_E_GC_RUNTIME = &H80000104       ' ���л���������
Public Const MV_E_GC_LOGICAL = &H80000105       ' �߼�����
Public Const MV_E_GC_ACCESS = &H80000106        ' �ڵ������������
Public Const MV_E_GC_TIMEOUT = &H80000107       ' ��ʱ
Public Const MV_E_GC_DYNAMICCAST = &H80000108   ' ת���쳣
Public Const MV_E_GC_UNKNOW = &H800001FF        ' GenICamδ֪����
'GigE_STATUS��Ӧ�Ĵ�����:��Χ= &H80000200-&H800002FF
Public Const MV_E_NOT_IMPLEMENTED = &H80000200  ' ������豸֧��
Public Const MV_E_INVALID_ADDRESS = &H80000201  ' ���ʵ�Ŀ���ַ������
Public Const MV_E_WRITE_PROTECT = &H80000202    ' Ŀ���ַ����д
Public Const MV_E_ACCESS_DENIED = &H80000203    ' �豸�޷���Ȩ��
Public Const MV_E_BUSY = &H80000204             ' �豸æ��������Ͽ�
Public Const MV_E_PACKET = &H80000205           ' ��������ݴ���
Public Const MV_E_NETER = &H80000206            ' ������ش���
'USB_STATUS��Ӧ�Ĵ�����:��Χ0x80000300-0x800003FF
Public Const MV_E_USB_READ = &H80000300         ' ��usb����
Public Const MV_E_USB_WRITE = &H80000301        ' дusb����
Public Const MV_E_USB_DEVICE = &H80000302       ' �豸�쳣
Public Const MV_E_USB_GENICAM = &H80000303      ' GenICam��ش���
Public Const MV_E_USB_BANDWIDTH = &H80000304    ' ������  �ô���������
Public Const MV_E_USB_UNKNOW = &H800003FF       ' USBδ֪�Ĵ���
'����ʱ��Ӧ�Ĵ�����:��Χ= &H80000400-= &H800004FF
Public Const MV_E_UPG_FILE_MISMATCH = &H80000400 ' �����̼���ƥ��
Public Const MV_E_UPG_LANGUSGE_MISMATCH = &H80000401    ' �����̼����Բ�ƥ��
Public Const MV_E_UPG_CONFLICT = &H80000402     ' ������ͻ���豸�Ѿ����������ٴ��������������ش˴���
Public Const MV_E_UPG_INNER_ERR = &H80000403    ' ����ʱ����ڲ����ִ���
Public Const MV_E_UPG_UNKNOW = &H800004FF       ' ����ʱδ֪����
' �豸���Ͷ���
Public Const MV_UNKNOW_DEVICE = &H0             ' δ֪�豸���ͣ���������
Public Const MV_GIGE_DEVICE = &H1               ' GigE�豸
Public Const MV_1394_DEVICE = &H2               ' 1394-a/b �豸
Public Const MV_USB_DEVICE = &H4                ' USB3.0 �豸
Public Const MV_CAMERALINK_DEVICE = &H8          ' CameraLink�豸
Public Const INFO_MAX_BUFFER_SIZE = 64
Public Const MV_MAX_TLS_NUM = 8                 ' ���֧�ֵĴ����ʵ������
Public Const MV_MAX_DEVICE_NUM = 256            ' ���֧�ֵ��豸����
' GigEVision IP Configuration
Public Const MV_IP_CFG_STATIC = &H5000000       '��̬
Public Const MV_IP_CFG_DHCP = &H6000000         'DHCP
Public Const MV_IP_CFG_LLA = &H4000000          'LLA
' GigEVision Net Transfer Mode
Public Const MV_NET_TRANS_DRIVER = &H1          '����
Public Const MV_NET_TRANS_SOCKET = &H2          'Socket
' ��Ϣ����
Public Const MV_MATCH_TYPE_NET_DETECT = &H1      ' ���������Ͷ�����Ϣ
Public Const MV_MATCH_TYPE_USB_DETECT = &H2      ' host���յ�����U3V�豸���ֽ�����
'�������궨��
Public Const MV_MAX_XML_NODE_NUM_C = 128        ' ĳ���ڵ��Ӧ���ӽڵ�������ֵ
Public Const MV_MAX_XML_NODE_STRLEN_C = 64      ' �ڵ������ַ�����󳤶�
Public Const MV_MAX_XML_STRVALUE_STRLEN_C = 64  '�ڵ�Stringֵ��󳤶�
Public Const MV_MAX_XML_DISC_STRLEN_C = 512     ' �ڵ������ֶ���󳤶�
Public Const MV_MAX_XML_ENTRY_NUM = 10          ' ���ĵ�Ԫ��
Public Const MV_MAX_XML_PARENTS_NUM = 8         ' ���ڵ��������
Public Const MV_MAX_XML_SYMBOLIC_STRLEN_C = 64  'ÿ���Ѿ�ʵ�ֵ�Ԫ�����Ƴ���
Public Const MV_MAX_XML_SYMBOLIC_NUM = 64
'  �쳣��Ϣ����
Public Const MV_EXCEPTION_DEV_DISCONNECT = &H8001      ' �豸�Ͽ�����
Public Const MV_EXCEPTION_VERSION_CHECK = &H8002      ' SDK�������汾��ƥ��
' �豸�ķ���ģʽ
Public Const MV_ACCESS_Exclusive = 1            ' ��ռȨ�ޣ�����APPֻ�����CCP�Ĵ���
Public Const MV_ACCESS_ExclusiveWithSwitch = 2  ' ���Դ�5ģʽ����ռȨ�ޣ�Ȼ���Զ�ռȨ�޴�
Public Const MV_ACCESS_Control = 3              ' ����Ȩ�ޣ�����APP��������мĴ���
Public Const MV_ACCESS_ControlWithSwitch = 4    ' ���Դ�5��ģʽ����ռȨ�ޣ�Ȼ���Կ���Ȩ�޴�
Public Const MV_ACCESS_ControlSwitchEnable = 5  ' �Կɱ���ռ�Ŀ���Ȩ�޴�
Public Const MV_ACCESS_ControlSwitchEnableWithKey = 6 ' ���Դ�5��ģʽ����ռȨ�ޣ�Ȼ���Կɱ���ռ�Ŀ���Ȩ�޴�
Public Const MV_ACCESS_Monitor = 7              ' ��ģʽ���豸�������ڿ���Ȩ����

Public Const MAX_BUF = 68157440                 ' 65M

''''''''''''''''''''''''''''''''''''''''ö�ٶ���'''''''''''''''''''''''''''''''''''''''''
Public Enum MvGvspPixelType                     ' ���ظ�ʽ����
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


Public Enum MV_CAM_ACQUISITION_MODE        '�ɼ�ģʽ�ṹ��
    MV_ACQ_MODE_SINGLE = 0                 ' ��֡ģʽ
    MV_ACQ_MODE_MUTLI = 1                  ' ��֡ģʽ
    MV_ACQ_MODE_CONTINUOUS = 2             ' �����ɼ�ģʽ
End Enum

Public Enum MV_CAM_GAIN_MODE               '����ģʽ�ṹ��
    MV_GAIN_MODE_OFF = 0                   ' �ر�
    MV_GAIN_MODE_ONCE = 1                  ' һ��
    MV_GAIN_MODE_CONTINUOUS = 2            ' ����
End Enum

Public Enum MV_CAM_EXPOSURE_MODE                '�ع�ģʽ�ṹ��
    MV_EXPOSURE_MODE_TIMED = 0                  ' Timed
    MV_EXPOSURE_MODE_TRIGGER_WIDTH = 1          ' TriggerWidth
End Enum

Public Enum MV_CAM_EXPOSURE_AUTO_MODE           '�Զ��ع�ģʽ�ṹ��
    MV_EXPOSURE_AUTO_MODE_OFF = 0               ' �ر�
    MV_EXPOSURE_AUTO_MODE_ONCE = 1              ' һ��
    MV_EXPOSURE_AUTO_MODE_CONTINUOUS = 2        ' ����
End Enum

Public Enum MV_CAM_TRIGGER_MODE                '����ģʽ�ṹ��
    MV_TRIGGER_MODE_OFF = 0                    ' �ر�
    MV_TRIGGER_MODE_ON = 1                     ' ��
End Enum
        
Public Enum MV_CAM_GAMMA_SELECTOR               '����ģʽ�ṹ��
    MV_GAMMA_SELECTOR_USER = 1                  '�û�
    MV_GAMMA_SELECTOR_SRGB = 2                  'sRGB
End Enum

Public Enum MV_CAM_BALANCEWHITE_AUTO            '��ƽ��ṹ��
    MV_BALANCEWHITE_AUTO_OFF = 0                '�ر�
    MV_BALANCEWHITE_AUTO_ONCE = 2               'һ��
    MV_BALANCEWHITE_AUTO_CONTINUOUS = 1         ' ����
End Enum

Public Enum MV_CAM_TRIGGER_SOURCE               '����Դ�ṹ��
    MV_TRIGGER_SOURCE_LINE0 = 0                 'Line0
    MV_TRIGGER_SOURCE_LINE1 = 1                 'Line1
    MV_TRIGGER_SOURCE_LINE2 = 2                 'Line2
    MV_TRIGGER_SOURCE_LINE3 = 3                 'Line3
    MV_TRIGGER_SOURCE_COUNTER0 = 4              'Counter0

    MV_TRIGGER_SOURCE_SOFTWARE = 7              '����
    MV_TRIGGER_SOURCE_FrequencyConverter = 8    '��Ƶ��
End Enum

Public Enum MV_GRAB_STRATEGY                    'ȡ�����Խṹ��
    MV_GrabStrategy_OneByOne                    '�Ӿɵ���һ֡һ֡�Ļ�ȡͼ��Ĭ��Ϊ�ò��ԣ�
    MV_GrabStrategy_LatestImagesOnly = 1        '��ȡ�б������µ�һ֡ͼ��ͬʱ����б��е�����ͼ��
    MV_GrabStrategy_LatestImages = 2            '��ȡ�б������µ�ͼ�񣬸�����OutputQueueSize��������ΧΪ1-ImageNodeNum�����ó�1��ͬ��LatestImagesOnly,���ó�ImageNodeNum��ͬ��OneByOne
    MV_GrabStrategy_UpcomingImage = 3           '�ȴ���һ֡ͼ��
End Enum

Public Enum MV_SAVE_IAMGE_TYPE                  '����ͼƬ��ʽ
    MV_Image_Undefined = 0                      'δ�����ͼ���ʽ
    MV_Image_Bmp = 1                            'BMPͼ���ʽ
    MV_Image_Jpeg = 2                           'JPEGͼ���ʽ
    MV_Image_Png = 3                            'PNGͼ���ʽ
    MV_Image_Tif = 4                            'TIFFͼ���ʽ
End Enum

Public Enum MV_SAVE_POINT_CLOUD_FILE_TYPE       '����ͼƬ��ʽ
    MV_PointCloudFile_Undefined = 0             'δ����ĵ��Ƹ�ʽ
    MV_PointCloudFile_PLY = 1                   'PLY���Ƹ�ʽ
    MV_PointCloudFile_CSV = 2                   'CSV���Ƹ�ʽ
    MV_PointCloudFile_OBJ = 3                   'OBJ���Ƹ�ʽ
End Enum

'''''''''''''''''''''''''''''''''�ṹ�嶨��'''''''''''''''''''''''''''''''''

Public Type MV_GIGE_DEVICE_INFO            'Gige�豸��Ϣ�ṹ��
    nIpCfgOption As Long                   'IP����ѡ��
    nIpCfgCurrent As Long                  '��ǰIP����
    nCurrentIp As Long                     '��ǰIP��ַ
    nCurrentSubNetMask As Long             '��ǰ��������
    nDefultGateWay As Long                 '��ǰ����
    chManufacturerName(31) As Byte         '����������
    chModelName(31) As Byte                '�ͺ�����
    chDeviceVersion(31) As Byte            '�豸�汾
    chManufacturerSpecificInfo(47) As Byte '�����̵ľ�����Ϣ
    chSerialNumber(15) As Byte             '���к�
    chUserDefinedName(15) As Byte          '�û��Զ�������
    nNetExport As Long                     '����IP��ַ
    nReserved(3) As Long                   'Ԥ��
End Type

Public Type MV_USB3_DEVICE_INFO            'U������豸��Ϣ�ṹ��
    CrtlInEndPoint As Byte                 '��������˵�
    CrtlOutEndPoint As Byte                '��������˵�
    StreamEndPoint As Byte                 '���˵�
    EventEndPoint As Byte                  '�¼��˵�
    idVendor As Integer                    '��Ӧ��ID��
    idProduct As Integer                   '��ƷID��
    nDeviceNumber As Long                  '�豸���к�
    chDeviceGUID(INFO_MAX_BUFFER_SIZE - 1) As Byte       '�豸GUID��
    chVendorName(INFO_MAX_BUFFER_SIZE - 1) As Byte       '��Ӧ������
    chModelName(INFO_MAX_BUFFER_SIZE - 1) As Byte        '�ͺ�����
    chFamilyName(INFO_MAX_BUFFER_SIZE - 1) As Byte       '��������
    chDeviceVersion(INFO_MAX_BUFFER_SIZE - 1) As Byte    '�豸�汾
    chManufacturerName(INFO_MAX_BUFFER_SIZE - 1) As Byte '����������
    chSerialNumber(INFO_MAX_BUFFER_SIZE - 1) As Byte     '���к�
    chUserDefinedName(INFO_MAX_BUFFER_SIZE - 1) As Byte  '�û��Զ�������
    nbcdUSB As Long                                      '֧�ֵ�USBЭ��
    nDeviceAddress As Long                               '�豸��ַ
    nReserved(2) As Long                                 'Ԥ��
End Type

Public Type MV_CC_DEVICE_INFO              '�豸��Ϣ�ṹ��
    nMajorVer As Integer                   '��Ҫ�汾
    nMinorVer As Integer                   '��Ҫ�汾
    nMacAddrHigh(3) As Byte                '��MAC��ַ
    nMacAddrLow(3) As Byte                 '��MAC��ַ
    nTLayerType As Long                    '�豸�����Э������
    nReserved(3) As Long                   'Ԥ��
    SpecialInfo(539) As Byte               '�����豸��Ϣ������
End Type

Public Type MV_CC_DEVICE_INFO_LIST         '�豸��Ϣ�ṹ��
    nDeviceNum As Long                     '�豸����
    pDeviceInfo(255) As Long               '�豸��Ϣ
End Type

Public Type MV_FRAME_OUT_INFO              'ͼ����Ϣ�ṹ��
    nWidth As Integer                      'ͼ���
    nHeight As Integer                     'ͼ���
    enPixelType As MvGvspPixelType         '���ظ�ʽ
    nFrameNum As Long                      '֡��
    nDevTimeStampHigh As Long              'ʱ�����32λ
    nDevTimeStampLow As Long               'ʱ�����32λ
    nReserved0 As Long                     'Ԥ��
    nHostTimeStamp As Double               '����ʱ���
    nFrameLen As Long                      '֡�ĳ���
    nReserved(2) As Long                   'Ԥ��
End Type

Public Type MV_CHUNK_DATA_CONTENT
    pChunkData As Long                   'Chunk����
    nChunkID As Long                     'ChunkId
    nChunkLen  As Long                   'Chunk�ĳ���
    nReversed(7) As Byte                 'Ԥ��
End Type

Public Type MV_FRAME_OUT_INFO_EX         'ͼ����Ϣ�ṹ��
    nWidth As Integer                    'ͼ���
    nHeight As Integer                   'ͼ���
    enPixelType As MvGvspPixelType       '���ظ�ʽ
    nFrameNum As Long                    '֡��
    nDevTimeStampHigh As Long            'ʱ�����32λ
    nDevTimeStampLow As Long             'ʱ�����32λ
    nReserved0 As Long                   '������8�ֽڶ���
    nHostTimeStamp As Double             '�������ɵ�ʱ���
    nFrameLen As Long                    '֡�ĳ���
    nSecondCount As Long                 '����
    nCycleCount As Long                  '������
    nCycleOffset As Long                 '����ƫ����
    fGain As Long                        '����
    fExposureTime As Long                '�ع�ʱ��
    nAverageBrightness As Long           'ƽ������
    nRed As Long                         '��ɫ
    nGreen As Long                       '��ɫ
    nBlue As Long                        '��ɫ
    nFrameCounter As Long                '��֡��
    nTriggerIndex As Long                '��������
    nInput As Long                       '����
    nOutput As Long                      '���
    nOffsetX As Integer                  'ˮƽƫ����
    nOffsetY As Integer                  '��ֱƫ����
    nChunkWidth As Integer               'Chunk��
    nChunkHeight As Integer              'Chunk��
    nLostPacket As Long                  '��֡������
    nUnparsedChunkNum As Long            'δ������ChunkData����
    UnparsedChunkList(7)  As Byte        'δ������ChunkChunk
    nReserved(35) As Long                'Ԥ��
End Type

Public Type MV_FRAME_OUT                    'ͼ��ṹ�壬���ͼ��ָ���ַ��ͼ����Ϣ
    pBufAddr As Long                        'ͼ��ָ���ַ
    stFrameInfo As MV_FRAME_OUT_INFO_EX     'ͼ����Ϣ  MV_FRAME_OUT_INFO_EX����
    nRes(15) As Long                        'Ԥ��
End Type

Public Type MVCC_INTVALUE               'int���ͽṹ��
    nCurValue As Long                   '��ǰֵ
    nMax As Long                        '���ֵ
    nMin As Long                        '��Сֵ
    nInc As Long                        '�������
    nReserved(3) As Long                'Ԥ��
End Type

Public Type MVCC_FLOATVALUE               'Float���ͽṹ��
    fCurValue As Single                   '��ǰֵ
    fMax As Single                        '���ֵ
    fMin As Single                        '��Сֵ
    nReserved(3) As Long                  'Ԥ��
End Type

Public Type MVCC_STRINGVALUE               'String���ͽṹ��
    chCurValue(255) As Byte                '��ǰֵ
    nReserved(3) As Long                   'Ԥ��
End Type

Public Type MVCC_ENUMVALUE                'ö�����ͽṹ��
    nCurValue As Long                     '��ǰֵ
    nSupportedNum As Long                 '���ݵ���Ч���ݸ���
    nSupportValue(MV_MAX_XML_SYMBOLIC_NUM) As Long '֧�ֵ�ö��ֵ
    nReserved(3) As Long
End Type

Public Type MV_EVENT_OUT_INFO          '�¼���Ϣ�ṹ��
    EventName As String                'Event����
    nEventID As Integer                'Event��
    nStreamChannel As Integer          '��ͨ�����
    nBlockIdHigh As Long               '֡�Ÿ�λ
    nBlockIdLow As Long                '֡�ŵ�λ
    nTimestampHigh  As Long            'ʱ�����λ
    nTimestampLow As Long              'ʱ�����λ
    pEventData As Long                 'Event����
    nEventDataSize As Long             'Event���ݳ���
    nReserved(16) As Long              'Ԥ��
    
End Type

Public Type MV_SAVE_IMAGE_PARAM        '����ͼƬ����
    pData As Long                      '�������ݻ���
    nDataLen As Long                   '�������ݴ�С
    enPixelType As MvGvspPixelType     '�������ݵ����ظ�ʽ
    nWidth As Integer                  'ͼ���
    nHeight As Integer                 'ͼ���
    pImageBuffer As Long               '���ͼƬ����
    nImageLen As Long                  '���ͼƬ��С
    nBufferSize As Long                '�ṩ�������������С
    enImageType As MV_SAVE_IAMGE_TYPE   '���ͼƬ��ʽ
End Type

Public Type MV_SAVE_IMAGE_PARAM_EX      '����ͼƬ����
    pData As Long                       '�������ݻ���
    nDataLen As Long                    '�������ݴ�С
    enPixelType As MvGvspPixelType      '�������ݵ����ظ�ʽ
    nWidth As Integer                   'ͼ���
    nHeight As Integer                  'ͼ���
    pImageBuffer As Long                '���ͼƬ����
    nImageLen As Long                   '���ͼƬ��С
    nBufferSize As Long                 '�ṩ�������������С
    enImageType As MV_SAVE_IAMGE_TYPE   '���ͼƬ��ʽ
    nJpgQuality As Long                 'JPG��������(50-99]��������ʽ��Ч
    iMethodValue As Long                '��ֵ����
    nReserved(3) As Long                'Ԥ��
End Type

Public Type MV_NETTRANS_INFO            '���紫��������Ϣ
    nReviceDataSize As Double           '�ѽ������ݴ�С [ͳ��StartGrabbing��StopGrabbing֮���������
    nThrowFrameCount As Long            '��֡����
    nNetRecvFrameCount As Long          '�ѽ��յ�֡��
    nRequestResendPacketCount As Long   '�����ط�����
    nResendPacketCount  As Long         '�ط�����
End Type

Public Type MV_DISPLAY_FRAME_INFO
    hWnd  As Long                       '���ھ��
    pData As Long                       '��ʾ������
    nDataLen As Integer                 '���ݳ���
    nWidth As Integer                   'ͼ���
    nHeight As Integer                  'ͼ���
    enPixelType As MvGvspPixelType      '���ظ�ʽ
    nReserved(3) As Long                'Ԥ��
End Type

Public Type MV_SAVE_IMG_TO_FILE_PARAM      '����BMP��JPEG��PNG��TIFFͼƬ�ļ��Ĳ���
    enPixelType As MvGvspPixelType      '�������ݵ����ظ�ʽ
    pData As Long                       '�������ݻ���
    nDataLen As Long                    '�������ݴ�С
    nWidth As Integer                   'ͼ���
    nHeight As Integer                  'ͼ���
    enImageType As MV_SAVE_IAMGE_TYPE   '���ͼƬ��ʽ
    nQuality As Long                    'JPG��������(50-99]��PNG��������[0-9]��������ʽ��Ч
    pImagePath(255) As Byte             '�����ļ�·��
    iMethodValue As Long                '��ֵ����
    nReserved(7) As Long                'Ԥ��
End Type

Public Type MV_SAVE_POINT_CLOUD_PARAM                     '����3D���ݵ�����ṹ��
    nLinePntNum As Long                                   'ÿһ�е����������ͼ���
    nLineNum As Long                                      '��������ͼ���
    enSrcPixelType As MvGvspPixelType                     '�������ݵ����ظ�ʽ
    pSrcData As Long                                      '�������ݻ���
    nSrcDataLen As Long                                   '�������ݴ�С
    pDstBuf As Long                                       '����������ݻ���
    nDstBufSize As Long                                   '�ṩ�������������С(nLinePntNum * nLineNum * (16*3 + 4) + 2048)
    nDstBufLen As Long                                    '����������ݻ��泤��
    enPointCloudFileType As MV_SAVE_POINT_CLOUD_FILE_TYPE '�ṩ����ĵ����ļ�����
    nReserved(7) As Long                                  '�����ֶ�
End Type

Public Type MV_CC_PIXEL_CONVERT_PARAM       'ͼ��ת���ṹ��
    nWidth As Integer                       'ͼ���
    nHeight As Integer                      'ͼ���
    enSrcPixelType As MvGvspPixelType       'Դ���ظ�ʽ
    pSrcData As Long                        '�������ݻ���
    nSrcDataLen As Long                     '�������ݴ�С
    enDstPixelType As MvGvspPixelType       'Ŀ�����ظ�ʽ
    pDstBuffer As Long                      '������ݻ���
    nDstLen As Long                         '������ݴ�С
    nDstBufferSize As Long                  '�ṩ�������������С
    nRes(3) As Long                         'Ԥ��
End Type

Public Type MV_CC_FILE_ACCESS       '�ļ���ȡ�ṹ��
    pUserFileName As Long           '�û��ļ���
    pDevFileName As Long            '�豸�ļ���
    nReserved(32) As Long           'Ԥ��
End Type


' ��������

'��ȡSDK�汾��
Public Declare Function MV_CC_GetSDKVersion Lib "MvCameraControl.dll" _
() As Long

'ö���豸
Public Declare Function MV_CC_EnumDevices Lib "MvCameraControl.dll" _
(ByVal nTLayerType As Long, ByRef pstDevList As MV_CC_DEVICE_INFO_LIST) As Long

'�����豸���
Public Declare Function MV_CC_CreateHandle Lib "MvCameraControl.dll" _
(ByRef handle As Long, ByVal stDeviceInfo As Long) As Long

'�����豸���
Public Declare Function MV_CC_DestroyHandle Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'���豸
Public Declare Function MV_CC_OpenDevice Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal nAccessMode As Long, ByVal nSwitchoverKey As Integer) As Long

'�ر��豸
Public Declare Function MV_CC_CloseDevice Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'��ʼȡ��
Public Declare Function MV_CC_StartGrabbing Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'��ʾͼ��ע����ʾ���ڣ��ڲ��Զ���ʾ����MV_CC_GetImageBuffer����ͬʱʹ�ã��������MV_CC_DisplayOneFrame�ӿڣ�
Public Declare Function MV_CC_Display Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal hWnd As Long) As Long

'��ȡһ֡ͼ�񣬴˺���Ϊ��ѯʽ��ȡ��ÿ�ε��ò�ѯ�ڲ������������ݣ���������Χ���ݣ������ݷ��ش�����
Public Declare Function MV_CC_GetOneFrame Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal pData As Long, ByVal nDataSize As Long, ByRef pFrameInfo As MV_FRAME_OUT_INFO) As Long

'���ó�ʱ���ƻ�ȡһ֡ͼƬ��SDK�ڲ��ȴ�ֱ��������ʱ����
Public Declare Function MV_CC_GetOneFrameTimeout Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal pData As Long, ByVal nDataSize As Long, ByRef pFrameInfo As MV_FRAME_OUT_INFO_EX, ByVal nMsec As Long) As Long

'ֹͣȡ��
Public Declare Function MV_CC_StopGrabbing Lib "MvCameraControl.dll" _
(ByVal handle As Long) As Long

'��ȡInteger����ֵ
Public Declare Function MV_CC_GetIntValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_INTVALUE) As Long

'����Integer������ֵ
Public Declare Function MV_CC_SetIntValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal nValue As Long) As Long

'��ȡEnum����ֵ
Public Declare Function MV_CC_GetEnumValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_ENUMVALUE) As Long

'����Enum������ֵ
Public Declare Function MV_CC_SetEnumValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal nValue As Long) As Long

'��ȡBoolean����ֵ
Public Declare Function MV_CC_GetBoolValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pbValue As Boolean) As Long

'����Boolean������ֵ
Public Declare Function MV_CC_SetBoolValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal bValue As Boolean) As Long

'��ȡString����ֵ
Public Declare Function MV_CC_GetStringValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_STRINGVALUE) As Long

'����String������ֵ
Public Declare Function MV_CC_SetStringValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal strValue As String) As Long

'��ȡFloat����ֵ
Public Declare Function MV_CC_GetFloatValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByRef pstValue As MVCC_FLOATVALUE) As Long

'����float������ֵ
Public Declare Function MV_CC_SetFloatValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String, ByVal fValue As Single) As Long

'����Command������ֵ
Public Declare Function MV_CC_SetCommandValue Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal strName As String) As Long

'����ͼƬ��֧��Bmp��Jpeg.
'ͨ�����ӿڿ��Խ����豸�ɼ�����ԭʼͼ������ת����JPEG����BMP�ȸ�ʽ�������ָ���ڴ��У�Ȼ���û����Խ�ת��֮�������ֱ�ӱ����ͼƬ�ļ����ýӿڵ����޽ӿ�˳��Ҫ����ͼ��Դ���ݾͿ��Խ���ת���������ȵ���MV_CC_GetOneFrameTimeout����MV_CC_RegisterImageCallBackEx���ûص���������ȡһ֡ͼ�����ݣ�Ȼ����ͨ���ýӿ�ת����ʽ�� \n
'MV_CC_SaveImageEx2��MV_CC_SaveImageEx���Ӳ���handle��Ϊ�˱�֤�������ӿڵ�ͳһ��
Public Declare Function MV_CC_SaveImageEx2 Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByRef pSaveParam As MV_SAVE_IMAGE_PARAM_EX) As Long

'��ʾһ֡ͼ��
Public Declare Function MV_CC_DisplayOneFrame Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByRef pstDisplayInfo As MV_DISPLAY_FRAME_INFO) As Long

'����SDK�ڲ�ͼ�񻺴�ڵ���������ڵ���1����ץͼǰ����
Public Declare Function MV_CC_SetImageNodeNum Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByVal nNum As Long) As Long


'���ظ�ʽת��
Public Declare Function MV_CC_ConvertPixelType Lib "MvCameraControl.dll" _
(ByVal handle As Long, ByRef pstDisplayInfo As MV_CC_PIXEL_CONVERT_PARAM) As Long


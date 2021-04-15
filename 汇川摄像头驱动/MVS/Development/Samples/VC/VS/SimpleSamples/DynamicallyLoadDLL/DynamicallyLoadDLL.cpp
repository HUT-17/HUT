#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include "CameraParams.h"
#include "MvErrorDefine.h"

typedef unsigned int    (__stdcall * DLL_GetSDKVersion)         ();
typedef int             (__stdcall * DLL_EnumerateTls)          ();
typedef int             (__stdcall * DLL_EnumDevices)           (IN unsigned int nTLayerType, IN OUT MV_CC_DEVICE_INFO_LIST * pstDevList);
typedef int             (__stdcall * DLL_EnumDevicesEx)         (IN unsigned int nTLayerType, IN OUT MV_CC_DEVICE_INFO_LIST* pstDevList, IN const char* pManufacturerName);
typedef bool            (__stdcall * DLL_IsDeviceAccessible)    (IN MV_CC_DEVICE_INFO* pstDevInfo, IN unsigned int nAccessMode);
typedef int             (__stdcall * DLL_CreateHandle)          (OUT void ** handle, IN const MV_CC_DEVICE_INFO* pstDevInfo);
typedef int             (__stdcall * DLL_CreateHandleWithoutLog)(OUT void ** handle, IN const MV_CC_DEVICE_INFO* pstDevInfo);
typedef int             (__stdcall * DLL_DestroyHandle)         (IN void * handle);
typedef int             (__stdcall * DLL_OpenDevice)            (IN void* handle, IN unsigned int nAccessMode, IN unsigned short nSwitchoverKey);
typedef int             (__stdcall * DLL_CloseDevice)           (IN void* handle);
typedef int             (__stdcall * DLL_RegisterImageCallBackEx)(void* handle, void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),void* pUser);
typedef int             (__stdcall * DLL_RegisterImageCallBackForRGB)(void* handle, void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser);
typedef int             (__stdcall * DLL_RegisterImageCallBackForBGR)(void* handle, void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser);
typedef int             (__stdcall * DLL_StartGrabbing)         (IN void* handle);
typedef int             (__stdcall * DLL_StopGrabbing)          (IN void* handle);
typedef int             (__stdcall * DLL_GetImageForRGB)        (IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);
typedef int             (__stdcall * DLL_GetImageForBGR)        (IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);
typedef int             (__stdcall * DLL_GetImageBuffer)        (IN void* handle, MV_FRAME_OUT* pFrameInfo, int nMsec);
typedef int             (__stdcall * DLL_FreeImageBuffer)       (IN void* handle, MV_FRAME_OUT* pFrameInfo);
typedef int             (__stdcall * DLL_GetOneFrameTimeout)    (IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);
typedef int             (__stdcall * DLL_Display)               (IN void* handle, void* hWnd);
typedef int             (__stdcall * DLL_DisplayOneFrame)       (IN void* handle, IN MV_DISPLAY_FRAME_INFO* pDisplayInfo);
typedef int             (__stdcall * DLL_SetImageNodeNum)       (IN void* handle, unsigned int nNum);
typedef int             (__stdcall * DLL_GetDeviceInfo)         (IN void * handle, IN OUT MV_CC_DEVICE_INFO* pstDevInfo);
typedef int             (__stdcall * DLL_GetAllMatchInfo)       (IN void* handle, IN OUT MV_ALL_MATCH_INFO* pstInfo);

typedef int             (__stdcall * DLL_GetIntValueEx)         (IN void* handle,IN const char* strKey,OUT MVCC_INTVALUE_EX *pIntValue);
typedef int             (__stdcall * DLL_SetIntValueEx)         (IN void* handle,IN const char* strKey,IN int64_t nValue);
typedef int             (__stdcall * DLL_GetEnumValue)          (IN void* handle,IN const char* strValue,OUT MVCC_ENUMVALUE *pEnumValue);
typedef int             (__stdcall * DLL_SetEnumValue)          (IN void* handle,IN const char* strValue,IN unsigned int nValue);
typedef int             (__stdcall * DLL_SetEnumValueByString)  (IN void* handle,IN const char* strValue,IN const char* sValue);
typedef int             (__stdcall * DLL_GetFloatValue)         (IN void* handle,IN const char* strValue,OUT MVCC_FLOATVALUE *pFloatValue);
typedef int             (__stdcall * DLL_SetFloatValue)         (IN void* handle,IN const char* strValue,IN float fValue);
typedef int             (__stdcall * DLL_GetBoolValue)          (IN void* handle,IN const char* strValue,OUT bool *pBoolValue);
typedef int             (__stdcall * DLL_SetBoolValue)          (IN void* handle,IN const char* strValue,IN bool bValue);
typedef int             (__stdcall * DLL_GetStringValue)        (IN void* handle,IN const char* strKey,OUT MVCC_STRINGVALUE *pStringValue);
typedef int             (__stdcall * DLL_SetStringValue)        (IN void* handle,IN const char* strKey,IN const char * sValue);
typedef int             (__stdcall * DLL_SetCommandValue)       (IN void* handle,IN const char* strValue);
typedef int             (__stdcall * DLL_LocalUpgrade)          (IN void* handle, const void *pFilePathName);
typedef int             (__stdcall * DLL_GetUpgradeProcess)     (IN void* handle, unsigned int* pnProcess);
typedef int             (__stdcall * DLL_GetOptimalPacketSize)  (IN void* handle);
typedef int             (__stdcall * DLL_ReadMemory)            (IN void* handle , void *pBuffer, int64_t nAddress, int64_t nLength);
typedef int             (__stdcall * DLL_WriteMemory)           (IN void* handle , const void *pBuffer, int64_t nAddress, int64_t nLength);
typedef int             (__stdcall * DLL_RegisterExceptionCallBack)(IN void* handle, void(__stdcall* cbException)(unsigned int nMsgType, void* pUser), void* pUser);
typedef int             (__stdcall * DLL_RegisterEventCallBackEx)(void* handle, const char* pEventName, void(__stdcall* cbEvent)(MV_EVENT_OUT_INFO * pEventInfo, void* pUser),void* pUser);
typedef int             (__stdcall * DLL_RegisterAllEventCallBack)(void* handle, void(__stdcall* cbEvent)(MV_EVENT_OUT_INFO * pEventInfo, void* pUser),void* pUser);

typedef int             (__stdcall * DLL_ForceIpEx)             (IN void* handle, unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay);
typedef int             (__stdcall * DLL_SetIpConfig)           (IN void* handle, unsigned int nType);
typedef int             (__stdcall * DLL_SetNetTransMode)       (IN void* handle, unsigned int nType);
typedef int             (__stdcall * DLL_GetNetTransInfo)       (IN void* handle, MV_NETTRANS_INFO* pstInfo);
typedef int             (__stdcall * DLL_SetGvcpTimeout)        (IN void* handle, unsigned int nMillisec);
typedef int				(__stdcall * DLL_SetResend)             (IN void* handle, unsigned int bEnable, unsigned int nMaxResendPercent, unsigned int nResendTimeout);
typedef int				(__stdcall * DLL_SetTransmissionType)   (IN void* handle, MV_TRANSMISSION_TYPE * pstTransmissionType);
typedef int				(__stdcall * DLL_IssueActionCommand)    (IN MV_ACTION_CMD_INFO* pstActionCmdInfo, OUT MV_ACTION_CMD_RESULT_LIST* pstActionCmdResults);

typedef int				(__stdcall * DLL_SetDeviceBauderate)    (IN void* handle, unsigned int nBaudrate);
typedef int				(__stdcall * DLL_GetDeviceBauderate)    (IN void* handle,unsigned int* pnCurrentBaudrate);
typedef int				(__stdcall * DLL_GetSupportBauderates)  (IN void* handle,unsigned int* pnBaudrateAblity);
typedef int				(__stdcall * DLL_SetGenCPTimeOut)       (IN void* handle, unsigned int nMillisec);

typedef int             (__stdcall * DLL_GetGenICamXML)         (IN void* handle, IN OUT unsigned char* pData, IN unsigned int nDataSize, OUT unsigned int* pnDataLen);
typedef int             (__stdcall * DLL_SaveImageEx2)          (IN void* handle, IN OUT MV_SAVE_IMAGE_PARAM_EX* pSaveParam);
typedef int             (__stdcall * DLL_ConvertPixelType)      (IN void* handle, IN OUT MV_CC_PIXEL_CONVERT_PARAM* pstCvtParam);
typedef int				(__stdcall * DLL_SetBayerCvtQuality)    (IN void* handle, IN unsigned int BayerCvtQuality);
typedef int				(__stdcall * DLL_FeatureSave)           (IN void* handle, IN const char* pFileName);
typedef int				(__stdcall * DLL_FeatureLoad)           (IN void* handle, IN const char* pFileName);
typedef int				(__stdcall * DLL_FileAccessRead)        (IN void* handle, IN MV_CC_FILE_ACCESS * pstFileAccess);
typedef int				(__stdcall * DLL_FileAccessWrite)       (IN void* handle, IN MV_CC_FILE_ACCESS * pstFileAccess);
typedef int				(__stdcall * DLL_GetFileAccessProgress) (IN void* handle, OUT MV_CC_FILE_ACCESS_PROGRESS * pstFileAccessProgress);
typedef int				(__stdcall * DLL_StartRecord)           (IN void* handle, IN MV_CC_RECORD_PARAM* pstRecordParam);
typedef int				(__stdcall * DLL_InputOneFrame)         (IN void* handle, IN MV_CC_INPUT_FRAME_INFO * pstInputFrameInfo);
typedef int				(__stdcall * DLL_StopRecord)            (IN void* handle);

bool g_bExit = false;
unsigned int g_nPayloadSize = 0;

struct MultiThrParam
{
    void *pUser;
    HINSTANCE hDll;
};

// ch:等待按键输入 | en:Wait for key press
void WaitForKeyPress(void)
{
    while(!_kbhit())
    {
        Sleep(10);
    }
    _getch();
}

bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
    if (NULL == pstMVDevInfo)
    {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
    {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
        printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n" , pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
    }
    else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
    {
        printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
        printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
        printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}

static  unsigned int __stdcall WorkThread(void* stMuthreadPar)
{
    int nRet = MV_OK;
    struct MultiThrParam *stMulThrPar = (struct MultiThrParam*)stMuthreadPar;
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    unsigned char * pData = (unsigned char *)malloc(sizeof(unsigned char) * (g_nPayloadSize));
    if (pData == NULL)
    {
        return 0;
    }
    unsigned int nDataSize = g_nPayloadSize;

    while(1)
    {
        DLL_GetOneFrameTimeout DLLGetOneFrameTimeout = (DLL_GetOneFrameTimeout)GetProcAddress(stMulThrPar->hDll, "MV_CC_GetOneFrameTimeout");
        nRet = DLLGetOneFrameTimeout(stMulThrPar->pUser, pData, nDataSize, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n", 
                stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
        }
        else
        {
            printf("No data[0x%x]\n", nRet);
        }
        if(g_bExit)
        {
            break;
        }
    }

    free(pData);

    return 0;
}

int main()
{
    int nRet = MV_OK;
    void* handle = NULL;

    HINSTANCE MvCamCtrlDll = NULL;
    // ch:dll默认路径为：系统盘:\Program Files (x86)\Common Files\MVS\Runtime
    // en:Dynamic link library default path:System Disk:\Program Files (x86)\Common Files\MVS\Runtime
    MvCamCtrlDll = LoadLibrary("MvCameraControl.dll");
    if (MvCamCtrlDll)
    {
        do 
        {
            // ch:枚举设备 | en:Enum device
            MV_CC_DEVICE_INFO_LIST stDeviceList;
            memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
            DLL_EnumDevices DLLEnumDevices = (DLL_EnumDevices) GetProcAddress (MvCamCtrlDll,"MV_CC_EnumDevices");
            nRet = DLLEnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
            if (MV_OK != nRet)
            {
                printf("Enum Devices fail! nRet [0x%x]\n", nRet);
                break;
            }

            if (stDeviceList.nDeviceNum > 0)
            {
                for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++)
                {
                    printf("[device %d]:\n", i);
                    MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
                    if (NULL == pDeviceInfo)
                    {
                        break;
                    } 
                    PrintDeviceInfo(pDeviceInfo);            
                }  
            } 
            else
            {
                printf("Find No Devices!\n");
                break;
            }

            printf("Please Input camera index:");
            unsigned int nIndex = 0;
            scanf_s("%d", &nIndex);

            if (nIndex >= stDeviceList.nDeviceNum)
            {
                printf("Input error!\n");
                break;
            }

            // ch:选择设备并创建句柄 | en:Select device and create handle
            DLL_CreateHandle DLLCreateHandle = (DLL_CreateHandle)GetProcAddress(MvCamCtrlDll, "MV_CC_CreateHandle");
            nRet = DLLCreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
            if (MV_OK != nRet)
            {
                printf("Create Handle fail! nRet [0x%x]\n", nRet);
                break;
            }

            // ch:打开设备 | en:Open device
            DLL_OpenDevice DLLOpenDevice = (DLL_OpenDevice)GetProcAddress(MvCamCtrlDll, "MV_CC_OpenDevice");
            nRet = DLLOpenDevice(handle, MV_ACCESS_Exclusive, 0);
            if (MV_OK != nRet)
            {
                printf("Open Device fail! nRet [0x%x]\n", nRet);
                break;
            }

            // ch:设置触发模式为off | en:Set trigger mode as off
            DLL_SetEnumValue DLLSetEnumValue = (DLL_SetEnumValue)GetProcAddress(MvCamCtrlDll, "MV_CC_SetEnumValue");
            nRet = DLLSetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_OFF);
            if (MV_OK != nRet)
            {
                printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
                break;
            }

            // ch:获取数据包大小 | en:Get payload size
            MVCC_INTVALUE_EX stParam;
            memset(&stParam, 0, sizeof(MVCC_INTVALUE_EX));
            DLL_GetIntValueEx DLLGetIntValueEx = (DLL_GetIntValueEx)GetProcAddress(MvCamCtrlDll, "MV_CC_GetIntValueEx");
            nRet = DLLGetIntValueEx(handle, "PayloadSize", &stParam);
            if (MV_OK != nRet)
            {
                printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
                break;
            }
            g_nPayloadSize = (unsigned int)stParam.nCurValue;

            // ch:开始取流 | en:Start grab image
            DLL_StartGrabbing DLLStartGrabbing = (DLL_StartGrabbing)GetProcAddress(MvCamCtrlDll, "MV_CC_StartGrabbing");
            nRet = DLLStartGrabbing(handle);
            if (MV_OK != nRet)
            {
                printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
                break;
            }

            unsigned int nThreadID = 0;
            MultiThrParam stMuthreadPar;
            stMuthreadPar.pUser = handle;
            stMuthreadPar.hDll = MvCamCtrlDll;
            void* hThreadHandle = (void*) _beginthreadex( NULL , 0 , WorkThread , (void*)&stMuthreadPar, 0 , &nThreadID );
            if (NULL == hThreadHandle)
            {
                break;
            }

            printf("Press a key to stop grabbing.\n");
            WaitForKeyPress();

            g_bExit = true;
            Sleep(1000);

            // ch:停止取流 | en:Stop grab image
            DLL_StopGrabbing DLLStopGrabbing = (DLL_StartGrabbing)GetProcAddress(MvCamCtrlDll, "MV_CC_StopGrabbing");
            nRet = DLLStopGrabbing(handle);
            if (MV_OK != nRet)
            {
                printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
                break;
            }

            // ch:关闭设备 | Close device
            DLL_CloseDevice DLLCloseDevice = (DLL_CloseDevice)GetProcAddress(MvCamCtrlDll, "MV_CC_CloseDevice");
            nRet = DLLCloseDevice(handle);
            if (MV_OK != nRet)
            {
                printf("ClosDevice fail! nRet [0x%x]\n", nRet);
                break;
            }

            // ch:销毁句柄 | Destroy handle
            DLL_DestroyHandle DLLDestroyHandle = (DLL_DestroyHandle)GetProcAddress(MvCamCtrlDll, "MV_CC_DestroyHandle");
            nRet = DLLDestroyHandle(handle);
            if (MV_OK != nRet)
            {
                printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
                break;
            }
        } while (0);


        if (nRet != MV_OK)
        {
            if (handle != NULL)
            {
                DLL_DestroyHandle DLLDestroyHandle = (DLL_DestroyHandle)GetProcAddress(MvCamCtrlDll, "MV_CC_DestroyHandle");
                nRet = DLLDestroyHandle(handle);
                handle = NULL;
            }
        }
        FreeLibrary(MvCamCtrlDll);
        printf("Press a key to exit.\n");
        WaitForKeyPress();
    }
    else
    {
        DWORD errCode = 0;
        errCode = GetLastError();
        printf("error code %ld!\n",errCode);
        printf("Press a key to exit.\n");
        WaitForKeyPress();
        return -1;
    }
    return 0;
}
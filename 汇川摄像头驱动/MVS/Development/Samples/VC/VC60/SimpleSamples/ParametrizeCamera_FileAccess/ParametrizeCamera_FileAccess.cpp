#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include "MvSdkExport.h"
#pragma comment(lib, "MvSdkExport.lib")

unsigned int g_nMode = 0;
int g_nRet = MV_OK;
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

DWORD _stdcall ProgressThread(void* pUser)
{
    int nRet = MV_OK;
    MV_CC_FILE_ACCESS_PROGRESS stFileAccessProgress = {0};

    while(1)
    {
        //ch:获取文件存取进度 |en:Get progress of file access
        nRet = MV_CC_GetFileAccessProgress(pUser, &stFileAccessProgress);
        printf("State = 0x%x,Completed = %I64d,Total = %I64d\r\n",nRet,stFileAccessProgress.nCompleted,stFileAccessProgress.nTotal);
        if (nRet != MV_OK || (stFileAccessProgress.nCompleted != 0 && stFileAccessProgress.nCompleted == stFileAccessProgress.nTotal))
        {
            break;
        }
        
        Sleep(50);
    }

    return 0;
}

DWORD _stdcall FileAccessThread(void* pUser)
{
    MV_CC_FILE_ACCESS stFileAccess = {0};

    stFileAccess.pUserFileName = "UserSet1.bin";
    stFileAccess.pDevFileName = "UserSet1";
    if (1 == g_nMode)
    {
        //ch:读模式 |en:Read mode
        g_nRet = MV_CC_FileAccessRead(pUser, &stFileAccess);
        if (MV_OK != g_nRet)
        {
            printf("File Access Read fail! nRet [0x%x]\n", g_nRet);
        }
    }
    else if (2 == g_nMode)
    {
        //ch:写模式 |en:Write mode
        g_nRet = MV_CC_FileAccessWrite(pUser, &stFileAccess);
        if (MV_OK != g_nRet)
        {
            printf("File Access Write fail! nRet [0x%x]\n", g_nRet);
        }
    }

    return 0;
}

int main()
{
    int nRet = MV_OK;
    void* handle = NULL;

    do 
    {
        // ch:枚举设备 | en:Enum device
        MV_CC_DEVICE_INFO_LIST stDeviceList;
        memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
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

        printf("Please Intput camera index:");
        unsigned int nIndex = 0;
        scanf("%d", &nIndex);

        if (nIndex >= stDeviceList.nDeviceNum)
        {
            printf("Intput error!\n");
            break;
        }

        // ch:选择设备并创建句柄 | en:Select device and create handle
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:打开设备 | en:Open device
        nRet = MV_CC_OpenDevice(handle,MV_ACCESS_Exclusive,0);
        if (MV_OK != nRet)
        {
            printf("Open Device fail! nRet [0x%x]\n", nRet);
            break;
        }

        //ch:读模式 |en:Read mode
        g_nMode = 1;
        printf("Read to file.\n");
        
        void* hReadHandle = (void*) CreateThread( NULL , 0 , FileAccessThread , handle, 0 , NULL );
        if (NULL == hReadHandle)
        {
            break;
        }
        
        Sleep(5);
        
        void* hReadProgressHandle = (void*) CreateThread( NULL , 0 , ProgressThread , handle, 0 , NULL );
        if (NULL == hReadProgressHandle)
        {
            break;
        }
        
        WaitForMultipleObjects(1, &hReadHandle, TRUE, INFINITE);
        WaitForMultipleObjects(1, &hReadProgressHandle, TRUE, INFINITE);
        if (MV_OK == g_nRet)
        {
            printf("File Access Read Success!\n");
        }
        printf("\n");
        
        //ch:写模式 |en:Write mode
        g_nMode = 2;
        printf("Write from file.\n");
        
        void* hWriteHandle = (void*) CreateThread( NULL , 0 , FileAccessThread , handle, 0 , NULL );
        if (NULL == hWriteHandle)
        {
            break;
        }
        
        Sleep(5);
        
        void* hWriteProgressHandle = (void*) CreateThread( NULL , 0 , ProgressThread , handle, 0 , NULL );
        if (NULL == hWriteProgressHandle)
        {
            break;
        }
        
        WaitForMultipleObjects(1, &hWriteHandle, TRUE, INFINITE);
        WaitForMultipleObjects(1, &hWriteProgressHandle, TRUE, INFINITE);
        if (MV_OK == g_nRet)
        {
            printf("File Access Write Success!\n");
        }
        
        // ch:关闭设备 | Close device
        nRet = MV_CC_CloseDevice(handle);
        if (MV_OK != nRet)
        {
            printf("ClosDevice fail! nRet [0x%x]\n", nRet);
            break;
        }
        
        // ch:销毁句柄 | Destroy handle
        nRet = MV_CC_DestroyHandle(handle);
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
            MV_CC_DestroyHandle(handle);
            handle = NULL;
        }
    }
    
    printf("Press a key to exit.\n");
    WaitForKeyPress();
    
    return 0;
}
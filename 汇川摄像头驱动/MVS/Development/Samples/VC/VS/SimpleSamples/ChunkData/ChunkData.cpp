#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include "MvCameraControl.h"

bool g_bExit = false;

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

/*Get Image buffer function, you can get the chunk infomation from frame infomation*/
void __stdcall ImageCallBackEx(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
    if (pFrameInfo)
    {
        //ch:打印ChunkData信息 | en:Print parse the timestamp information in the frame
        printf("ImageCallBack：FrameNum[%d], ExposureTime[%f], SecondCount[%d], CycleCount[%d], CycleOffset[%d]\n", 
            pFrameInfo->nFrameNum, pFrameInfo->fExposureTime, pFrameInfo->nSecondCount, pFrameInfo->nCycleCount, pFrameInfo->nCycleOffset);

        MV_CHUNK_DATA_CONTENT* pUnparsedChunkContent = pFrameInfo->UnparsedChunkList.pUnparsedChunkContent;
        for(unsigned int i = 0;i < pFrameInfo->nUnparsedChunkNum; i++)
        {
            //ch：只打印ID和长度，内容需要根据说明文档进行解析
            printf("ChunkInfo[%d]: ChunkID[0x%x], ChunkLen[%d]\n", i, pUnparsedChunkContent->nChunkID, pUnparsedChunkContent->nChunkLen);
            pUnparsedChunkContent++;
        }
        printf("***********************************\n");
    }
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
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);
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
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:打开设备 | en:Open device
        nRet = MV_CC_OpenDevice(handle);
        if (MV_OK != nRet)
        {
            printf("Open Device fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:注册抓图回调 | en:Register image callback
        nRet = MV_CC_RegisterImageCallBackEx(handle, ImageCallBackEx, handle);
        if (MV_OK != nRet)
        {
            printf("Register Image CallBack fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:开启Chunk Mode | en:Open Chunk Mode
        nRet = MV_CC_SetBoolValue(handle, "ChunkModeActive", true);
        if (MV_OK != nRet)
        {
            printf("Set Chunk Mode fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:Chunk Selector设为Exposure | en: Chunk Selector set as Exposure
        nRet = MV_CC_SetEnumValueByString(handle, "ChunkSelector", "Exposure");
        if (MV_OK != nRet)
        {
            printf("Set Exposure Chunk fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:开启Chunk Enable | en:Open Chunk Enable
        nRet = MV_CC_SetBoolValue(handle, "ChunkEnable", true);
        if (MV_OK != nRet)
        {
            printf("Set Chunk Enable fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:Chunk Selector设为Timestamp | en: Chunk Selector set as Timestamp
        nRet = MV_CC_SetEnumValueByString(handle, "ChunkSelector", "Timestamp");
        if (MV_OK != nRet)
        {
            printf("Set Timestamp Chunk fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:开启Chunk Enable | en:Open Chunk Enable
        nRet = MV_CC_SetBoolValue(handle, "ChunkEnable", true);
        if (MV_OK != nRet)
        {
            printf("Set Chunk Enable fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:设置触发模式为off | en:Set trigger mode as off
        nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_OFF);
        if (MV_OK != nRet)
        {
            printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:开始取流 | en:Start grab image
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
            break;
        }

        printf("Press a key to stop grabbing.\n");
        WaitForKeyPress();

        g_bExit = true;
        Sleep(1000);

        // ch:停止取流 | en:Stop grab image
        nRet = MV_CC_StopGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
            break;
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

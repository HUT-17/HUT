#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include "MvCameraControl.h"

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

int main()
{
    int nRet = MV_OK;
    void* handle = NULL;
    unsigned char* pDstImageBuf = NULL;
    unsigned char* pSaveImageBuf = NULL;

    do 
    {
        // ch:枚举设备 | en:Enum device
        MV_CC_DEVICE_INFO_LIST stDeviceList = {0};
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

        // ch:判断设备是否是设置的3D格式 | en:Judge Whether the device is set to 3D format
        MVCC_ENUMVALUE EnumValue = {0};
        nRet = MV_CC_GetEnumValue(handle, "PixelFormat", &EnumValue);
        if (MV_OK != nRet)
        {
            printf("Get the Camera format fail! nRet [0x%x]\n", nRet);
            break;
        }

        enum MvGvspPixelType ePixelFormat = (MvGvspPixelType)EnumValue.nCurValue;
        switch (ePixelFormat)
        {
        case PixelType_Gvsp_Coord3D_ABC32:
        case PixelType_Gvsp_Coord3D_ABC32f:
        case PixelType_Gvsp_Coord3D_AB32:
        case PixelType_Gvsp_Coord3D_AB32f:
        case PixelType_Gvsp_Coord3D_AC32:
        case PixelType_Gvsp_Coord3D_AC32f:
        case PixelType_Gvsp_Coord3D_ABC16:
            {
                nRet = MV_OK;
                break;
            }

        default:
            {
                nRet = MV_E_SUPPORT;
                break;
            }
        }
        if (MV_OK != nRet)
        {
            printf("This is not a supported 3D format!");
            break;
        }

        // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
        if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
        {
            int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
            if (nPacketSize > 0)
            {
                nRet = MV_CC_SetIntValue(handle,"GevSCPSPacketSize",nPacketSize);
                if(nRet != MV_OK)
                {
                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
                }
            }
            else
            {
                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
            }
        }

        // ch:获取触发模式的访问模式 | en:Get Access mode of trigger mode
        MV_XML_AccessMode pAccessMode = AM_NI;
        nRet = MV_XML_GetNodeAccessMode(handle, "TriggerMode", &pAccessMode);
        if (MV_OK != nRet)
        {
            printf("Get Access mode of trigger mode fail! nRet [0x%x]\n", nRet);
        }
        else
        {
            // ch:设置触发模式为off | en:Set trigger mode as off
            nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
            if (MV_OK != nRet)
            {
                printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
                break;
            }
        }

        // ch:获取数据包大小 | en:Get payload size
        MVCC_INTVALUE_EX stIntValue = {0};
        nRet = MV_CC_GetIntValueEx(handle, "PayloadSize", &stIntValue);
        if (MV_OK != nRet)
        {
            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
            break;
        }
        unsigned int nPayloadSize = (unsigned int)stIntValue.nCurValue;

        // ch:开始取流 | en:Start grab image
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:申请足够大的缓存，用于保存获取到的图像
        unsigned int nImageNum = 100;
        unsigned char* pSaveImageBuf = (unsigned char*)malloc(nPayloadSize * nImageNum);
        if (NULL == pSaveImageBuf)
        {
            printf("Malloc  Save buffer fail!\n");
            break;
        }
        unsigned int nSaveImageSize = nPayloadSize * nImageNum;

        // ch:已获取的总图片大小
        unsigned int nSaveDataLen = 0;

        MV_FRAME_OUT stOutFrame = {0};
        for(unsigned int i = 0;i < nImageNum; i++)
        {
            nRet = MV_CC_GetImageBuffer(handle, &stOutFrame, 1000);
            if (nRet == MV_OK)
            {
                printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n",
                    stOutFrame.stFrameInfo.nWidth, stOutFrame.stFrameInfo.nHeight, stOutFrame.stFrameInfo.nFrameNum);
                
                if (nSaveImageSize > (nSaveDataLen + stOutFrame.stFrameInfo.nFrameLen))
                {
                    // ch:将图像拷贝到pSaveImageBuf | Copy one frame of image to the buffer named pSaveImageBuf
                    memcpy(pSaveImageBuf + nSaveDataLen, stOutFrame.pBufAddr, stOutFrame.stFrameInfo.nFrameLen);
                    nSaveDataLen += stOutFrame.stFrameInfo.nFrameLen;
                }

                nRet = MV_CC_FreeImageBuffer(handle, &stOutFrame);
                if(nRet != MV_OK)
                {
                    printf("Free Image Buffer fail! nRet [0x%x]\n", nRet);
                }
            }
            else
            {
                printf("No data[0x%x]\n", nRet);
            }
        }

        MV_SAVE_POINT_CLOUD_PARAM stSavePoCloudPar = {0};

        stSavePoCloudPar.nLinePntNum = stOutFrame.stFrameInfo.nWidth;
        stSavePoCloudPar.nLineNum = stOutFrame.stFrameInfo.nHeight * nImageNum;

        //ch:提供的输出缓冲区大小(nLinePntNum * nLineNum * (16*3 + 4) + 2048) | en:Output buffer size provided(nLinePntNum * nLineNum * (16*3 + 4) + 2048)
        unsigned char* pDstImageBuf = (unsigned char*)malloc(stSavePoCloudPar.nLineNum * stSavePoCloudPar.nLinePntNum * (16 * 3 + 4) + 2048);
        if (NULL == pDstImageBuf)
        {
            printf("Malloc Dst buffer fail!\n");
            break;
        }

        unsigned int nDstImageSize = stSavePoCloudPar.nLineNum * stSavePoCloudPar.nLinePntNum * (16 * 3 + 4) + 2048;

        stSavePoCloudPar.enPointCloudFileType = MV_PointCloudFile_PLY;
        stSavePoCloudPar.enSrcPixelType = stOutFrame.stFrameInfo.enPixelType;
        stSavePoCloudPar.pSrcData = pSaveImageBuf;
        stSavePoCloudPar.nSrcDataLen = nSaveDataLen;
        stSavePoCloudPar.pDstBuf = pDstImageBuf;
        stSavePoCloudPar.nDstBufSize = nDstImageSize;

        //Save point cloud data
        nRet = MV_CC_SavePointCloudData(handle, &stSavePoCloudPar);
        if(MV_OK != nRet)
        {
            printf("Save point cloud data failed!nRet [0x%x]\n", nRet);
            break;
        }

        //Write the point cloud data to the file
        char pImageName[32] = "PointCloudData.ply";
        FILE* fp = fopen(pImageName, "wb+");
        if(!fp)
        {
            printf("Allocate memory fail! nRet [0x%x]\n", nRet);
            break;
        }
        fwrite(pDstImageBuf, 1, stSavePoCloudPar.nDstBufLen, fp);
        fclose(fp);
        printf("Save point cloud data succeed!\n");

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

    if(pSaveImageBuf)
    {
        free(pSaveImageBuf);
    }
    if(pDstImageBuf)
    {
        free(pDstImageBuf);
    }

    printf("Press a key to exit.\n");
    WaitForKeyPress();

    return 0;
}

#include <stdio.h>
#include <Windows.h>
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
    unsigned char * pSrcData = NULL;
    unsigned char * pDstData = NULL;
    unsigned char *pCurveBuf = NULL;

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

        nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_OFF);
        if (MV_OK != nRet)
        {
            printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:获取数据包大小 | en:Get payload size
        MVCC_INTVALUE stParam;
        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
        nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
        if (MV_OK != nRet)
        {
            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
            break;
        }
        unsigned int nPayloadSize = stParam.nCurValue;

        // ch:开始取流 | en:Start grab image
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
            break;
        }

        MV_FRAME_OUT_INFO_EX stImageInfo = {0};
        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

        pSrcData = (unsigned char *)malloc(sizeof(unsigned char) * (nPayloadSize));
        if (pSrcData == NULL)
        {
            printf("malloc pData fail !\n");
            break;
        }
        unsigned int nSrcDataSize = nPayloadSize;

        nRet = MV_CC_GetOneFrameTimeout(handle, pSrcData, nSrcDataSize, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n", 
                stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);

            unsigned int nDataSizeForCurve = 4096 * sizeof(int) * 2;
            pCurveBuf = (unsigned char*)malloc(nDataSizeForCurve);
            if (NULL == pCurveBuf)
            {
                printf("malloc pCurveBuf fail !\n");
                nDataSizeForCurve = 0;
                break;
            }

            // ch:Bayer噪声估计 | en:Bayer noise estimate
            MV_CC_BAYER_NOISE_ESTIMATE_PARAM stEstimateParam = {0};
            stEstimateParam.nWidth = stImageInfo.nWidth;
            stEstimateParam.nHeight = stImageInfo.nHeight;
            stEstimateParam.enPixelType = stImageInfo.enPixelType;
            stEstimateParam.pSrcData = pSrcData;
            stEstimateParam.nSrcDataLen = stImageInfo.nFrameLen;
            stEstimateParam.nNoiseThreshold = 1024;
            stEstimateParam.pCurveBuf = pCurveBuf;
            nRet = MV_CC_BayerNoiseEstimate(handle, &stEstimateParam);
            if (MV_OK != nRet)
            {
                printf("Noise estimate fail! nRet [0x%x]\n", nRet);
                break;
            }

            // ch:Bayer空域降噪 | en:Bayer spatial denoise 
            pDstData = (unsigned char *)malloc(sizeof(unsigned char) * (nPayloadSize));
            if (pDstData == NULL)
            {
                printf("malloc pData fail !\n");
                break;
            }
            unsigned int nDstDataSize = nPayloadSize;

            MV_CC_BAYER_SPATIAL_DENOISE_PARAM stDisnoiseParam = {0};
            memset(&stDisnoiseParam, 0, sizeof(MV_CC_BAYER_SPATIAL_DENOISE_PARAM));

            stDisnoiseParam.nWidth = stImageInfo.nWidth;                //ch:图像宽 | en:image width
            stDisnoiseParam.nHeight = stImageInfo.nHeight;              //ch:图像高 | en:image height
            stDisnoiseParam.enPixelType = stImageInfo.enPixelType;      //ch:输入像素格式 | en:input pixel format
            stDisnoiseParam.pSrcData = pSrcData;                        //ch:输入数据缓存 | en:input data buffer
            stDisnoiseParam.nSrcDataLen = stImageInfo.nFrameLen;        //ch:输入数据大小 | en:input data size
            stDisnoiseParam.pDstBuf = pDstData;                        //ch:输出数据缓存 | en:output data buffer
            stDisnoiseParam.nDstBufSize = nDstDataSize;                 //ch:输出缓存大小 | en:output buffer size
            memcpy(&stDisnoiseParam.stNoiseProfile, &stEstimateParam.stNoiseProfile, sizeof(MV_CC_BAYER_NOISE_PROFILE_INFO));
            stDisnoiseParam.nDenoiseStrength = 50;   //ch:降噪强度(0-100) | en:denoise strength(0-100)
            stDisnoiseParam.nSharpenStrength = 16;   //ch:锐化强度(0-32) | en:sharpen strength(0-32)
            stDisnoiseParam.nNoiseCorrect = 1024;      //ch:噪声校正系数(0-1280) | en:noise correct coefficient(0-1280)
            nRet = MV_CC_BayerSpatialDenoise(handle, &stDisnoiseParam);
            if (MV_OK != nRet)
            {
                printf("Bayer spatial denoise fail! nRet [0x%x]\n", nRet);
                break;
            }

            FILE* fp = NULL;
            errno_t err = fopen_s(&fp, "BeforeDenoise.raw", "wb");
            if (0 != err || NULL == fp)
            {
                printf("Open file failed\n");
                nRet = MV_E_RESOURCE;
                break;
            }
            fwrite(pSrcData, 1, stDisnoiseParam.nSrcDataLen, fp);
            fclose(fp);

            err = fopen_s(&fp, "AfterDenoise.raw", "wb");
            if (0 != err || NULL == fp)
            {
                printf("Open file failed\n");
                nRet = MV_E_RESOURCE;
                break;
            }
            fwrite(pDstData, 1, stDisnoiseParam.nDstBufLen, fp);
            fclose(fp);
            printf("Bayer spatial denoise succeed\n");
        }
        else
        {
            printf("No data[0x%x]\n", nRet);
        }

        // ch:停止取流 | en:Stop grab image
        nRet = MV_CC_StopGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:关闭设备 | en:Close device
        nRet = MV_CC_CloseDevice(handle);
        if (MV_OK != nRet)
        {
            printf("Close Device fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:销毁句柄 | en:Destroy handle
        nRet = MV_CC_DestroyHandle(handle);
        if (MV_OK != nRet)
        {
            printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
            break;
        }
    } while (0);

    if (pSrcData)
    {
        free(pSrcData);	
        pSrcData = NULL;
    }

    if (pCurveBuf)
    {
        free(pCurveBuf);
        pCurveBuf = NULL;
    }

    if (pDstData)
    {
        free(pDstData);
        pDstData = NULL;
    }

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

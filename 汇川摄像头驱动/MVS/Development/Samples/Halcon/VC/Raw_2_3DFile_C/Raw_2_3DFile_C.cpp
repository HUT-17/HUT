#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <HalconCpp.h>
#include "MvCameraControl.h"

using namespace Halcon;

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
    unsigned char * pData = NULL;

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
        scanf("%d", &nIndex);

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

        // ch:获取触发模式的访问模式 | en:Get Access mode of trigger mode
        MV_XML_AccessMode pAccessMode = AM_NI;
        nRet = MV_XML_GetNodeAccessMode(handle, "TriggerMode", &pAccessMode);
        if (MV_OK != nRet)
        {
            printf("Get Access mode of trigger mode fail! nRet [0x%x]\n", nRet);
        }
        else
        {
            nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_OFF);
            if (MV_OK != nRet)
            {
                printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
                break;
            }
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

        pData = (unsigned char *)malloc(sizeof(unsigned char) * (nPayloadSize));
        if (pData == NULL)
        {
            printf("malloc pData fail !\n");
            break;
        }

        HTuple hObjectModel3D;
        HTuple xHVal;
        HTuple yHVal;
        HTuple zHVal;
        unsigned int i = 0;

        nRet = MV_CC_GetOneFrameTimeout(handle, pData, nPayloadSize, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            printf("Get One Frame: Width[%d], Height[%d], FrameNum[%d]\n", 
                stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);

            switch(stImageInfo.enPixelType)
            {
            case PixelType_Gvsp_Coord3D_ABC32f:
                {
                    float * pConvertData = (float *)pData;
                    for (i = 0;i < stImageInfo.nWidth;i++)
                    {
                        xHVal[i] = *pConvertData;
                        yHVal[i] = *(pConvertData+1);
                        zHVal[i] = *(pConvertData+2);
                        pConvertData += 3;
                    }
                }
                break;
            case PixelType_Gvsp_Coord3D_ABC32:
                {
                    int * pConvertData = (int *)pData;
                    for (i = 0;i < stImageInfo.nWidth;i++)
                    {
                        xHVal[i] = *pConvertData;
                        yHVal[i] = *(pConvertData+1);
                        zHVal[i] = *(pConvertData+2);
                        pConvertData += 3;
                    }
                }
                break;
            case PixelType_Gvsp_Coord3D_AB32f:
                {
                    float * pConvertData = (float *)pData;
                    for (i = 0;i < stImageInfo.nWidth;i++)
                    {
                        xHVal[i] = *pConvertData;
                        yHVal[i] = *(pConvertData+1);
                        zHVal[i] = 0.0;
                        pConvertData += 2;
                    }
                }
                break;
            case PixelType_Gvsp_Coord3D_AB32:
                {
                    int * pConvertData = (int *)pData;
                    for (i = 0;i < stImageInfo.nWidth;i++)
                    {
                        xHVal[i] = *pConvertData;
                        yHVal[i] = *(pConvertData+1);
                        zHVal[i] = 0;
                        pConvertData += 2;
                    }
                }
                break;
            case PixelType_Gvsp_Coord3D_AC32f_64:
                {
                    float * pConvertData = (float *)pData;
                    for (i = 0;i < stImageInfo.nWidth;i++)
                    {
                        xHVal[i] = *pConvertData;
                        yHVal[i] = 0.0;
                        zHVal[i] = *(pConvertData+1);
                        pConvertData += 2;
                    }
                }
                break;
            case PixelType_Gvsp_Coord3D_AC32:
                {
                    int * pConvertData = (int *)pData;
                    for (i = 0;i < stImageInfo.nWidth;i++)
                    {
                        xHVal[i] = *pConvertData;
                        yHVal[i] = 0;
                        zHVal[i] = *(pConvertData+1);
                        pConvertData += 2;
                    }
                }
                break;
            default:
                break;
            }
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
            printf("Destroy handle fail! nRet [0x%x]\n", nRet);
            break;
        }

        if (i == 0)
        {
            printf("Probably not a 3D device!\n");
            break;
        }

        printf("*********************************************************************\n");
        printf("*  0.ply;  1.obj;  *\n");
        printf("*********************************************************************\n");

        int nFileType = 0;
        printf("Select FileType: ");
        scanf("%d", &nFileType);

        gen_object_model_3d_from_points(xHVal, yHVal, zHVal, &hObjectModel3D);
        switch(nFileType)
        {
        case 0:
            {
                write_object_model_3d(hObjectModel3D, "ply", "./3DFile.ply", "invert_normals", false);
            }
            break;
        case 1:
            {
                write_object_model_3d(hObjectModel3D, "obj", "./3DFile.obj", "invert_normals", false);
            }
            break;
        default:
            printf("Input error!\n");
            break;
        }
    } while (0);

    if (pData)
    {
        free(pData);	
        pData = NULL;
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

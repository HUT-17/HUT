#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include "MvCameraControl.h"

// ch:�ȴ��������� | en:Wait for key press
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

static  unsigned int __stdcall UpcomingThread(void* pUser)
{
    Sleep(3000);//Ϊ�˵�MV_CC_GetImageBuffer���ú��ٷ�����������

    printf("Trigger Software Once for MV_GrabStrategy_UpcomingImage\n");
    MV_CC_SetCommandValue(pUser, "TriggerSoftware");

    return 0;
}

int main()
{
    int nRet = MV_OK;
    void* handle = NULL;
    unsigned char * pData = NULL;

    do 
    {
        // ch:ö���豸 | en:Enum device
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

        // ch:ѡ���豸��������� | en:Select device and create handle
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:���豸 | en:Open device
        nRet = MV_CC_OpenDevice(handle);
        if (MV_OK != nRet)
        {
            printf("Open Device fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:̽��������Ѱ���С(ֻ��GigE�����Ч) | en:Detection network optimal package size(It only works for the GigE camera)
        if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
        {
            int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
            if (nPacketSize > 0)
            {
                if(MV_CC_SetIntValue(handle,"GevSCPSPacketSize",nPacketSize) != MV_OK)
                {
                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
                }
            }
            else
            {
                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
            }
        }

        // ch:��������ģʽ | en:Set Trigger Mode and Set Trigger Source
        nRet = MV_CC_SetEnumValueByString(handle, "TriggerMode", "On");
        if (MV_OK != nRet)
        {
            printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
            break;
        }
        nRet = MV_CC_SetEnumValueByString(handle, "TriggerSource", "Software");
        if (MV_OK != nRet)
        {
            printf("Set Trigger Source fail! nRet [0x%x]\n", nRet);
            break;
        }

        unsigned int nImageNodeNum = 5;
        // ch:���û���ڵ���� | en:Set number of image node
        nRet = MV_CC_SetImageNodeNum(handle, nImageNodeNum);
        if (MV_OK != nRet)
        {
            printf("Set number of image node fail! nRet [0x%x]\n", nRet);
            break;
        }

        printf("\n**************************************************************************\n");
        printf("* 0.MV_GrabStrategy_OneByOne;       1.MV_GrabStrategy_LatestImagesOnly;  *\n");
        printf("* 2.MV_GrabStrategy_LatestImages;   3.MV_GrabStrategy_UpcomingImage;     *\n");
        printf("**************************************************************************\n");

        printf("Please Input Grab Strategy:");
        unsigned int nGrabStrategy = 0;
        scanf_s("%d", &nGrabStrategy);

        // ch:U3V�����֧��MV_GrabStrategy_UpcomingImage | en:U3V device not support UpcomingImage
        if (nGrabStrategy == MV_GrabStrategy_UpcomingImage && MV_USB_DEVICE == stDeviceList.pDeviceInfo[nIndex]->nTLayerType)
        {
            printf("U3V device not support UpcomingImage\n");
            break;
        }

        switch(nGrabStrategy)
        {
        case MV_GrabStrategy_OneByOne:
            {
                printf("Grab using the MV_GrabStrategy_OneByOne default strategy\n");
                nRet = MV_CC_SetGrabStrategy(handle, MV_GrabStrategy_OneByOne);
                if (MV_OK != nRet)
                {
                    printf("Set Grab Strategy fail! nRet [0x%x]\n", nRet);
                    break;
                }
            }
            break;
        case MV_GrabStrategy_LatestImagesOnly:
            {
                printf("Grab using strategy MV_GrabStrategy_LatestImagesOnly\n");
                nRet = MV_CC_SetGrabStrategy(handle, MV_GrabStrategy_LatestImagesOnly);
                if (MV_OK != nRet)
                {
                    printf("Set Grab Strategy fail! nRet [0x%x]\n", nRet);
                    break;
                }
            }
            break;
        case MV_GrabStrategy_LatestImages:
            {
                printf("Grab using strategy MV_GrabStrategy_LatestImages\n");
                nRet = MV_CC_SetGrabStrategy(handle, MV_GrabStrategy_LatestImages);
                if (MV_OK != nRet)
                {
                    printf("Set Grab Strategy fail! nRet [0x%x]\n", nRet);
                    break;
                }

                // ch:�������������� | en:Set Output Queue Size
                nRet = MV_CC_SetOutputQueueSize(handle, 2);
                if (MV_OK != nRet)
                {
                    printf("Set Output Queue Size fail! nRet [0x%x]\n", nRet);
                    break;
                }
            }
            break;
        case MV_GrabStrategy_UpcomingImage:
            {
                printf("Grab using strategy MV_GrabStrategy_UpcomingImage\n");
                nRet = MV_CC_SetGrabStrategy(handle, MV_GrabStrategy_UpcomingImage);
                if (MV_OK != nRet)
                {
                    printf("Set Grab Strategy fail! nRet [0x%x]\n", nRet);
                    break;
                }

                unsigned int nThreadID = 0;
                void* hThreadHandle = (void*) _beginthreadex( NULL , 0 , UpcomingThread , handle, 0 , &nThreadID );
                if (NULL == hThreadHandle)
                {
                    break;
                }
            }
            break;
        default:
            printf("Input error!Use default strategy:MV_GrabStrategy_OneByOne\n");
            break;
        }

        // ch:��ʼȡ�� | en:Start grab image
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:������������ | en:Send Trigger Software command
        for (unsigned int i = 0;i < nImageNodeNum;i++)
        {
            nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
            if (MV_OK != nRet)
            {
                printf("Send Trigger Software command fail! nRet [0x%x]\n", nRet);
                break;
            }
            Sleep(500);//���֡�ʹ�С��TriggerDelay�ܴ󣬿��ܻ������������û��ȫ����Ч������ȡ�������ݵ����
        }

        MV_FRAME_OUT stOutFrame = {0};
        if (nGrabStrategy != MV_GrabStrategy_UpcomingImage)
        {
            while(true)
            {
                nRet = MV_CC_GetImageBuffer(handle, &stOutFrame, 0);//��ʱʱ�����ó�0����Ϊ�����б����Ѿ������ݴ���
                if (nRet == MV_OK)
                {
                    printf("Get One Frame: Width[%d], Height[%d], FrameNum[%d]\n", 
                        stOutFrame.stFrameInfo.nWidth, stOutFrame.stFrameInfo.nHeight, stOutFrame.stFrameInfo.nFrameNum);
                }
                else
                {
                    printf("No data[0x%x]\n", nRet);
                    break;
                }

                nRet = MV_CC_FreeImageBuffer(handle, &stOutFrame);
                if(nRet != MV_OK)
                {
                    printf("Free Image Buffer fail! nRet [0x%x]\n", nRet);
                }
            }
        }
        else//������upcoming
        {
            nRet = MV_CC_GetImageBuffer(handle, &stOutFrame, 5000);//��Ҫ�Ƚϴ�ĳ�ʱʱ������ȡ�����������֡����
            if (nRet == MV_OK)
            {
                printf("Get One Frame: Width[%d], Height[%d], FrameNum[%d]\n", 
                    stOutFrame.stFrameInfo.nWidth, stOutFrame.stFrameInfo.nHeight, stOutFrame.stFrameInfo.nFrameNum);

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

        // ch:ֹͣȡ�� | en:Stop grab image
        nRet = MV_CC_StopGrabbing(handle);
        if (MV_OK != nRet)
        {
            printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:�ر��豸 | en:Close device
        nRet = MV_CC_CloseDevice(handle);
        if (MV_OK != nRet)
        {
            printf("Close Device fail! nRet [0x%x]\n", nRet);
            break;
        }

        // ch:���پ�� | en:Destroy handle
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

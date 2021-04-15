#include "MvCameraControl.h"
#include <stdio.h>
#include <Windows.h>
#include <conio.h>


// Wait for key press
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
    if (pstMVDevInfo->nTLayerType == MV_CAMERALINK_DEVICE)
    {
        printf("chPortID: [%s]\n", pstMVDevInfo->SpecialInfo.stCamLInfo.chPortID);
        printf("chModelName: [%s]\n", pstMVDevInfo->SpecialInfo.stCamLInfo.chModelName);
        printf("chFamilyName: [%s]\n", pstMVDevInfo->SpecialInfo.stCamLInfo.chFamilyName);
        printf("chDeviceVersion: [%s]\n", pstMVDevInfo->SpecialInfo.stCamLInfo.chDeviceVersion);
        printf("chManufacturerName: [%s]\n", pstMVDevInfo->SpecialInfo.stCamLInfo.chManufacturerName);
        printf("Serial Number: [%s]\n", pstMVDevInfo->SpecialInfo.stCamLInfo.chSerialNumber);
    }
    else
    {
        printf("Not support.\n");
    }

    return true;
}


// Get the value of various feature nodes
int GetParameters(void* handle)
{
    if ( NULL == handle )
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;

    // Get value of Integer nodes. Such as, 'width' etc.
    MVCC_INTVALUE stIntVal;
    memset(&stIntVal, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(handle, "Width", &stIntVal);
    if (MV_OK != nRet)
    {
        printf("Get Width fail! nRet [0x%x]\n", nRet);
        return nRet;
    }
    printf("Current Width [%d]\n", stIntVal.nCurValue);

    // Get value of Enum nodes. Such as, 'TriggerMode' etc.
    MVCC_ENUMVALUE stEnumVal;
    memset(&stEnumVal, 0, sizeof(MVCC_ENUMVALUE));
    nRet = MV_CC_GetEnumValue(handle, "TriggerMode", &stEnumVal);
    if (MV_OK != nRet)
    {
        printf("Get Trigger Mode fail! nRet [0x%x]\n", nRet);
        return nRet;
    }
    printf("Current TriggerMode [%d]\n", stEnumVal.nCurValue);

    // Get value of float nodes. Such as, 'AcquisitionFrameRate' etc.
    MVCC_FLOATVALUE stFloatVal;
    memset(&stFloatVal, 0, sizeof(MVCC_FLOATVALUE));
    nRet = MV_CC_GetFloatValue(handle, "AcquisitionFrameRate", &stFloatVal);
    if (MV_OK != nRet)
    {
        printf("Get AcquisitionFrameRate fail! nRet [0x%x]\n", nRet);
        return nRet;
    }
    printf("Current AcquisitionFrameRate [%f] Fps\n", stFloatVal.fCurValue);

    // Get value of bool nodes. Such as, 'AcquisitionFrameRateEnable' etc.
    bool bBoolVal = false;
    nRet = MV_CC_GetBoolValue(handle, "AcquisitionFrameRateEnable", &bBoolVal);
    if (MV_OK != nRet)
    {
        printf("Get AcquisitionFrameRateEnable fail! nRet [0x%x]\n", nRet);
        return nRet;
    }
    printf("Current AcquisitionFrameRateEnable [%d]\n", bBoolVal);

    // Get value of String nodes. Such as, 'DeviceUserID' etc.
    MVCC_STRINGVALUE stStrVal;
    memset(&stStrVal, 0, sizeof(MVCC_STRINGVALUE));
    nRet = MV_CC_GetStringValue(handle, "DeviceUserID", &stStrVal);
    if (MV_OK != nRet)
    {
        printf("Get DeviceUserID fail! nRet [0x%x]\n", nRet);
        return nRet;
    }
    printf("Current DeviceUserID [%s]\n", stStrVal.chCurValue);

    return MV_OK;
}


// Set the value of various feature nodes
int SetParameters(void* handle)
{
    if ( NULL == handle )
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;

    // Set value of Integer nodes. Such as, 'width' etc.
    nRet = MV_CC_SetIntValue(handle, "Width", 200);
    if (MV_OK != nRet)
    {
        printf("Set Width fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // Set value of float nodes. Such as, 'AcquisitionFrameRate' etc.
    nRet = MV_CC_SetFloatValue(handle, "AcquisitionFrameRate", 8.8f);
    if (MV_OK != nRet)
    {
        printf("Set AcquisitionFrameRate fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // Set value of bool nodes. Such as, 'AcquisitionFrameRateEnable' etc.
    nRet = MV_CC_SetBoolValue(handle, "AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet)
    {
        printf("Set AcquisitionFrameRateEnable fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // Set value of String nodes. Such as, 'DeviceUserID' etc.
    nRet = MV_CC_SetStringValue(handle, "DeviceUserID", "UserIDChanged");
    if (MV_OK != nRet)
    {
        printf("Set DeviceUserID fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // Execute Command nodes. Such as, 'TriggerSoftware' etc.
    // precondition
    // Set value of Enum nodes. Such as, 'TriggerMode' etc.
    nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_ON);
    if (MV_OK != nRet)
    {
        printf("Set TriggerMode fail! nRet [0x%x]\n", nRet);
        return nRet;
    }
    nRet = MV_CC_SetEnumValue(handle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
    if (MV_OK != nRet)
    {
        printf("Set TriggerSource fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    // execute command
    nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
    if (MV_OK != nRet)
    {
        printf("Execute TriggerSoftware fail! nRet [0x%x]\n", nRet);
        return nRet;
    }

    return MV_OK;
}


void __stdcall ExceptionCallBack(unsigned int nMsgType, void* pUser)
{
    if(nMsgType == MV_EXCEPTION_DEV_DISCONNECT)
    {
        printf("MV_EXCEPTION_DEV_DISCONNECT");
    }
    else
    {
        printf("Unknown exception 0x[%x] ", nMsgType);
    }
}


int main()
{
    int nRet = MV_OK;
    void* handle = NULL;
    bool bDevConnected = false;  //whether a device is connected

    do 
    {
        // Enum devices
        MV_CC_DEVICE_INFO_LIST stDeviceList;
        memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
        nRet = MV_CC_EnumDevices(MV_CAMERALINK_DEVICE, &stDeviceList);
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

        // Select device and create handle
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet)
        {
            printf("Create Handle fail! nRet [0x%x]\n", nRet);
            break;
        }

        // Open the specified device
        nRet = MV_CC_OpenDevice(handle);
        if (MV_OK != nRet)
        {
            printf("Open Device fail! nRet [0x%x]\n", nRet);
            break;
        }
        bDevConnected = true;

        // Register disconnected exception callBack
        nRet = MV_CC_RegisterExceptionCallBack(handle, ExceptionCallBack, handle);
        if (MV_OK != nRet)
        {
            printf("RegisterExceptionCallBack fail! nRet [0x%x]\n", nRet);
            break;
        }

        /*******************characteristic interfaces for CameraLink device*********************/
        // Get supported bauderates of the combined device and host interface
        unsigned int nBaudrateAblity = 0;
        nRet = MV_CAML_GetSupportBauderates(handle, &nBaudrateAblity);
        if (MV_OK != nRet)
        {
            printf("Get supported bauderate fail! nRet [0x%x]\n", nRet);
            break;
        }
        printf("Current device supported bauderate [0x%x]\n", nBaudrateAblity);

        // Set device bauderate
        nRet = MV_CAML_SetDeviceBauderate(handle, MV_CAML_BAUDRATE_115200);
        if (MV_OK != nRet)
        {
            printf("Set Device Bauderate fail! nRet [0x%x]\n", nRet);
            break;
        }

        // Get the current device bauderate
        unsigned int nCurrentBaudrate = 0;
        nRet = MV_CAML_GetDeviceBauderate(handle, &nCurrentBaudrate);
        if (MV_OK != nRet)
        {
            printf("Get device bauderate fail! nRet [0x%x]\n", nRet);
            break;
        }
        printf("Current device bauderate [0x%x]\n", nCurrentBaudrate);

        /****************************properties configuration**********************************/
        // Get the value of various feature nodes
        nRet = GetParameters(handle);
        if (MV_OK != nRet)
        {
            printf("Get parameters fail! nRet [0x%x]\n", nRet);
            break;
        }

        // Set the value of various feature nodes
        nRet = SetParameters(handle);
        if (MV_OK != nRet)
        {
            printf("Set parameters fail! nRet [0x%x]\n", nRet);
            break;
        }

        // Close device
        nRet = MV_CC_CloseDevice(handle);
        if (MV_OK != nRet)
        {
            printf("ClosDevice fail! nRet [0x%x]\n", nRet);
            break;
        }
        bDevConnected = false;

        // Destroy handle
        nRet = MV_CC_DestroyHandle(handle);
        if (MV_OK != nRet)
        {
            printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
            break;
        }
        printf("Device successfully closed.\n");
    } while (0);


    // Ensure that the device is closed
    if (nRet != MV_OK)
    {
        if ( bDevConnected )
        {
            MV_CC_CloseDevice(handle);
            bDevConnected = false;
        }
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

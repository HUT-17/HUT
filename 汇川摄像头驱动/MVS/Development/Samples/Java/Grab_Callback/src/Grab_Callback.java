/***************************************************************************************************
 * @file      Grab_Callback.java
 * @breif     Use functions provided in MvCameraControlWrapper.jar to grab images
 * @author    zhanglei72
 * @date      2020/01/12
 *
 * @warning
 * @version   V1.0.0  2020/01/12 Create this file
 *            V1.0.1  2020/02/10 add parameter checking
 * @since     2020/02/10
 **************************************************************************************************/

import java.util.ArrayList;
import java.util.Scanner;

import MvCameraControlWrapper.*;
import static MvCameraControlWrapper.MvCameraControlDefines.*;

public class Grab_Callback
{
    private static void printDeviceInfo(MV_CC_DEVICE_INFO stDeviceInfo)
    {
        if (null == stDeviceInfo) {
            System.out.println("stDeviceInfo is null");
            return;
        }

        if (stDeviceInfo.transportLayerType == MV_GIGE_DEVICE)
        {
            System.out.println("\tCurrentIp:       " + stDeviceInfo.gigEInfo.currentIp);
            System.out.println("\tModel:           " + stDeviceInfo.gigEInfo.modelName);
            System.out.println("\tUserDefinedName: " + stDeviceInfo.gigEInfo.userDefinedName);
        }
        else if (stDeviceInfo.transportLayerType == MV_USB_DEVICE)
        {
            System.out.println("\tUserDefinedName: " + stDeviceInfo.usb3VInfo.userDefinedName);
            System.out.println("\tSerial Number:   " + stDeviceInfo.usb3VInfo.serialNumber);
            System.out.println("\tDevice Number:   " + stDeviceInfo.usb3VInfo.deviceNumber);
        }
        else
        {
            System.err.print("Device is not supported! \n");
        }

        System.out.println("\tAccessible:      "
            + MvCameraControl.MV_CC_IsDeviceAccessible(stDeviceInfo, MV_ACCESS_Exclusive));
        System.out.println("");
    }

    private static void printFrameInfo(MV_FRAME_OUT_INFO stFrameInfo)
    {
        if (null == stFrameInfo)
        {
            System.err.println("stFrameInfo is null");
            return;
        }

        StringBuilder frameInfo = new StringBuilder("");
        frameInfo.append(("\tFrameNum[" + stFrameInfo.frameNum + "]"));
        frameInfo.append("\tWidth[" + stFrameInfo.width + "]");
        frameInfo.append("\tHeight[" + stFrameInfo.height + "]");
        frameInfo.append(String.format("\tPixelType[%#x]", stFrameInfo.pixelType.getnValue()));

        System.out.println(frameInfo.toString());
    }

    public static int chooseCamera(ArrayList<MV_CC_DEVICE_INFO> stDeviceList)
    {
        if (null == stDeviceList)
        {
            return -1;
        }
        
        // Choose a device to operate
        int camIndex = -1;
        Scanner scanner = new Scanner(System.in);

        while (true)
        {
            try
            {
                System.out.print("Please input camera index (-1 to quit):");
                camIndex = scanner.nextInt();
                if ((camIndex >= 0 && camIndex < stDeviceList.size()) || -1 == camIndex)
                {
                    break;
                }
                else
                {
                    System.out.println("Input error: " + camIndex);
                }
            }
            catch (Exception e)
            {
                e.printStackTrace();
                camIndex = -1;
                break;
            }
        }
        scanner.close();

        if (-1 == camIndex)
        {
            System.out.println("Bye.");
            return camIndex;
        }

        if (0 <= camIndex && stDeviceList.size() > camIndex)
        {
            if (MV_GIGE_DEVICE == stDeviceList.get(camIndex).transportLayerType)
            {
                System.out.println("Connect to camera[" + camIndex + "]: " + stDeviceList.get(camIndex).gigEInfo.userDefinedName);
            }
            else if (MV_USB_DEVICE == stDeviceList.get(camIndex).transportLayerType)
            {
                System.out.println("Connect to camera[" + camIndex + "]: " + stDeviceList.get(camIndex).usb3VInfo.userDefinedName);
            }
            else
            {
                System.out.println("Device is not supported.");
            }
        }
        else
        {
            System.out.println("Invalid index " + camIndex);
            camIndex = -1;
        }

        return camIndex;
    }
    
    public static void main(String[] args)
    {
        int nRet = MV_OK;
        int camIndex = -1;
        Handle hCamera = null;
        ArrayList<MV_CC_DEVICE_INFO> stDeviceList = null;

        do
        {
            System.out.println("SDK Version " + MvCameraControl.MV_CC_GetSDKVersion());
            
            // Enumerate GigE and USB devices
            try
            {
                stDeviceList = MvCameraControl.MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE);
                if (0 >= stDeviceList.size())
                {
                    System.out.println("No devices found!");
                    break;
                }
                int i = 0;
                for (MV_CC_DEVICE_INFO stDeviceInfo : stDeviceList)
                {
                    if (null == stDeviceInfo)
                    {
                        continue;
                    }
                    System.out.println("[camera " + (i++) + "]");
                    printDeviceInfo(stDeviceInfo);
                }
            }
            catch (CameraControlException e)
            {
                System.err.println("Enumrate devices failed!" + e.toString());
                e.printStackTrace();
                break;
            }

            // choose camera
            camIndex = chooseCamera(stDeviceList);
            if (-1 == camIndex)
            {
                break;
            }

            // Create device handle
            try
            {
                hCamera = MvCameraControl.MV_CC_CreateHandle(stDeviceList.get(camIndex));
            }
            catch (CameraControlException e)
            {
                System.err.println("Create handle failed!" + e.toString());
                e.printStackTrace();
                hCamera = null;
                break;
            }

            // Open selected device
            nRet = MvCameraControl.MV_CC_OpenDevice(hCamera);
            if (MV_OK != nRet)
            {
                System.err.printf("Connect to camera failed, errcode: [%#x]\n", nRet);
                break;
            }

            // Register image callback
            nRet = MvCameraControl.MV_CC_RegisterImageCallBack(hCamera, new CameraImageCallBack() {
                @Override
                public int OnImageCallBack(byte[] bytes,MV_FRAME_OUT_INFO mv_frame_out_info)
                {
                    printFrameInfo(mv_frame_out_info);
                    return 0;
                }
            });
            if (MV_OK != nRet)
            {
                System.err.printf("register image callback failed, errcode: [%#x]\n", nRet);
                break;
            }

            // set continuous acquisition
            nRet = MvCameraControl.MV_CC_SetEnumValueByString(hCamera, "AcquisitionMode", "Continuous");
            if (MV_OK != nRet) 
            {
                System.err.printf("Set AcquisitionMode to Continous failed, errcode: [%#x]\n", nRet);
                break;
            }

            // Turn on trigger mode
            nRet = MvCameraControl.MV_CC_SetEnumValueByString(hCamera, "TriggerMode", "On");
            if (MV_OK != nRet)
            {
                System.err.printf("SetTriggerMode failed, errcode: [%#x]\n", nRet);
                break;
            }

            // set trigger source to software
            nRet = MvCameraControl.MV_CC_SetEnumValueByString(hCamera, "TriggerSource", "Software");
            if (MV_OK != nRet)
            {
                System.err.printf("SetTriggerSource to software failed, errcode: [%#x]\n", nRet);
                break;
            }

            // Start grabbing
            nRet = MvCameraControl.MV_CC_StartGrabbing(hCamera);
            if (MV_OK != nRet)
            {
                System.err.printf("StartGrabbing failed, errcode: [%#x]\n", nRet);
                break;
            }

            System.out.println("Software trigger: ");
            for (int i = 0; i < 10; i++)
            {
                nRet = MvCameraControl.MV_CC_SetCommandValue(hCamera, "TriggerSoftware");
                if (MV_OK != nRet)
                {
                    System.err.printf("Software trigger failed, errcode: [%#x]\n", nRet);
                }

                try
                {
                    Thread.sleep(1 * 1000);
                }
                catch (InterruptedException e)
                {
                    e.printStackTrace();
                }
            }

            // Turn off trigger mode and stop acquisition
            nRet = MvCameraControl.MV_CC_SetEnumValueByString(hCamera, "TriggerMode", "Off");
            if (MV_OK != nRet)
            {
                System.err.printf("SetTriggerMode failed, errcode: [%#x]\n", nRet);
                break;
            }
            nRet = MvCameraControl.MV_CC_StopGrabbing(hCamera);
            if (MV_OK != nRet)
            {
                System.err.printf("StopGrabbing failed, errcode: [%#x]\n", nRet);
                break;
            }

            // Continuous mode grabbing for 10 seconds
            System.out.println("Continuous grabbing: ");
            nRet = MvCameraControl.MV_CC_StartGrabbing(hCamera);
            if (MV_OK != nRet) {
                System.err.printf("StartGrabbing failed, errcode: [%#x]\n", nRet);
                break;
            }

            try
            {
                Thread.sleep(10 * 1000);
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }

            // Stop grabbing
            nRet = MvCameraControl.MV_CC_StopGrabbing(hCamera);
            if (MV_OK != nRet)
            {
                System.err.printf("StopGrabbing failed, errcode: [%#x]\n", nRet);
                break;
            }
        } while (false);

        if (null != hCamera)
        {
            // Destroy handle
            nRet = MvCameraControl.MV_CC_DestroyHandle(hCamera);
            if (MV_OK != nRet) {
                System.err.printf("DestroyHandle failed, errcode: [%#x]\n", nRet);
            }
        }
    }
}

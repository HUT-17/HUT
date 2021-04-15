﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MvCamCtrl.NET;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;

namespace GrabImage
{
    class GrabImage
    {
        static bool g_bExit = false;
        static uint g_nPayloadSize = 0;
        public static void ReceiveImageWorkThread(object obj)
        {
            int nRet = MyCamera.MV_OK;
            MyCamera device = obj as MyCamera;
            MyCamera.MV_FRAME_OUT_INFO_EX stImageInfo = new MyCamera.MV_FRAME_OUT_INFO_EX();
            IntPtr pData = Marshal.AllocHGlobal((int)g_nPayloadSize);
            if (pData == IntPtr.Zero)
            {
                return;
            }
            uint nDataSize = g_nPayloadSize;
            MyCamera.MV_CC_INPUT_FRAME_INFO stInputFrameInfo = new MyCamera.MV_CC_INPUT_FRAME_INFO();

            while (true)
            {
                nRet = device.MV_CC_GetOneFrameTimeout_NET(pData, nDataSize, ref stImageInfo, 1000);
                if (nRet == MyCamera.MV_OK)
                {
                    Console.WriteLine("Get One Frame:" + "Width[" + Convert.ToString(stImageInfo.nWidth) + "] , Height[" + Convert.ToString(stImageInfo.nHeight)
                                    + "] , FrameNum[" + Convert.ToString(stImageInfo.nFrameNum) + "]");

                    stInputFrameInfo.pData = pData;
                    stInputFrameInfo.nDataLen = stImageInfo.nFrameLen;
                    nRet = device.MV_CC_InputOneFrame_NET(ref stInputFrameInfo);
                    if (MyCamera.MV_OK != nRet)
                    {
                        Console.WriteLine("Input one frame failed: nRet {0:x8}", nRet);
                    }
                }
                else
                {
                    Console.WriteLine("No data:{0:x8}", nRet);
                }
                if (g_bExit)
                {
                    break;
                }
            }
        }

        static void Main(string[] args)
        {
            int nRet = MyCamera.MV_OK;
            MyCamera device = new MyCamera();
            do
            {
                // ch:枚举设备 | en:Enum device
                MyCamera.MV_CC_DEVICE_INFO_LIST stDevList = new MyCamera.MV_CC_DEVICE_INFO_LIST();
                nRet = MyCamera.MV_CC_EnumDevices_NET(MyCamera.MV_GIGE_DEVICE | MyCamera.MV_USB_DEVICE, ref stDevList);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Enum device failed:{0:x8}", nRet);
                    break;
                }
                Console.WriteLine("Enum device count : " + Convert.ToString(stDevList.nDeviceNum));
                if (0 == stDevList.nDeviceNum)
                {
                    break;
                }

                MyCamera.MV_CC_DEVICE_INFO stDevInfo;                            // 通用设备信息

                // ch:打印设备信息 en:Print device info
                for (Int32 i = 0; i < stDevList.nDeviceNum; i++)
                {
                    stDevInfo = (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(stDevList.pDeviceInfo[i], typeof(MyCamera.MV_CC_DEVICE_INFO));

                    if (MyCamera.MV_GIGE_DEVICE == stDevInfo.nTLayerType)
                    {
                        MyCamera.MV_GIGE_DEVICE_INFO stGigEDeviceInfo = (MyCamera.MV_GIGE_DEVICE_INFO)MyCamera.ByteToStruct(stDevInfo.SpecialInfo.stGigEInfo, typeof(MyCamera.MV_GIGE_DEVICE_INFO));
                        uint nIp1 = ((stGigEDeviceInfo.nCurrentIp & 0xff000000) >> 24);
                        uint nIp2 = ((stGigEDeviceInfo.nCurrentIp & 0x00ff0000) >> 16);
                        uint nIp3 = ((stGigEDeviceInfo.nCurrentIp & 0x0000ff00) >> 8);
                        uint nIp4 = (stGigEDeviceInfo.nCurrentIp & 0x000000ff);
                        Console.WriteLine("\n" + i.ToString() + ": [GigE] User Define Name : " + stGigEDeviceInfo.chUserDefinedName);
                        Console.WriteLine("device IP :" + nIp1 + "." + nIp2 + "." + nIp3 + "." + nIp4);
                    }
                    else if (MyCamera.MV_USB_DEVICE == stDevInfo.nTLayerType)
                    {
                        MyCamera.MV_USB3_DEVICE_INFO stUsb3DeviceInfo = (MyCamera.MV_USB3_DEVICE_INFO)MyCamera.ByteToStruct(stDevInfo.SpecialInfo.stUsb3VInfo, typeof(MyCamera.MV_USB3_DEVICE_INFO));
                        Console.WriteLine("\n" + i.ToString() + ": [U3V] User Define Name : " + stUsb3DeviceInfo.chUserDefinedName);
                        Console.WriteLine("\n Serial Number : " + stUsb3DeviceInfo.chSerialNumber);
                        Console.WriteLine("\n Device Number : " + stUsb3DeviceInfo.nDeviceNumber);
                    }
                }

                Int32 nDevIndex = 0;
                Console.Write("\nPlease input index （0 -- {0:d}） : ", stDevList.nDeviceNum - 1);
                try
                {
                    nDevIndex = Convert.ToInt32(Console.ReadLine());
                }
                catch
                {
                    Console.Write("Invalid Input!\n");
                    break;
                }

                if (nDevIndex > stDevList.nDeviceNum - 1 || nDevIndex < 0)
                {
                    Console.Write("Input Error!\n");
                    break;
                }
                stDevInfo = (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(stDevList.pDeviceInfo[nDevIndex], typeof(MyCamera.MV_CC_DEVICE_INFO));

                // ch:创建设备 | en:Create device
                nRet = device.MV_CC_CreateDevice_NET(ref stDevInfo);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Create device failed:{0:x8}", nRet);
                    break;
                }

                // ch:打开设备 | en:Open device
                nRet = device.MV_CC_OpenDevice_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Open device failed:{0:x8}", nRet);
                    break;
                }

                // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
                if (stDevInfo.nTLayerType == MyCamera.MV_GIGE_DEVICE)
                {
                    int nPacketSize = device.MV_CC_GetOptimalPacketSize_NET();
                    if (nPacketSize > 0)
                    {
                        nRet = device.MV_CC_SetIntValue_NET("GevSCPSPacketSize", (uint)nPacketSize);
                        if (nRet != MyCamera.MV_OK)
                        {
                            Console.WriteLine("Warning: Set Packet Size failed {0:x8}", nRet);
                        }
                    }
                    else
                    {
                        Console.WriteLine("Warning: Get Packet Size failed {0:x8}", nPacketSize);
                    }
                }

                // ch:设置触发模式为off || en:set trigger mode as off
                if (MyCamera.MV_OK != device.MV_CC_SetEnumValue_NET("TriggerMode", 0))
                {
                    Console.WriteLine("Set TriggerMode failed!");
                    break;
                }

                // ch:获取包大小 || en: Get Payload Size
                MyCamera.MVCC_INTVALUE stParam = new MyCamera.MVCC_INTVALUE();
                nRet = device.MV_CC_GetIntValue_NET("PayloadSize", ref stParam);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Get PayloadSize failed:{0:x8}", nRet);
                    break;
                }
                g_nPayloadSize = stParam.nCurValue;

                MyCamera.MV_CC_RECORD_PARAM stRecordPar = new MyCamera.MV_CC_RECORD_PARAM();
                nRet = device.MV_CC_GetIntValue_NET("Width", ref stParam);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Get Width failed: nRet {0:x8}", nRet);
                    break;
                }
                stRecordPar.nWidth = (ushort)stParam.nCurValue;

                nRet = device.MV_CC_GetIntValue_NET("Height", ref stParam);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Get Height failed: nRet {0:x8}", nRet);
                    break;
                }
                stRecordPar.nHeight = (ushort)stParam.nCurValue;

                MyCamera.MVCC_ENUMVALUE stEnumValue = new MyCamera.MVCC_ENUMVALUE();
                nRet = device.MV_CC_GetEnumValue_NET("PixelFormat", ref stEnumValue);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Get Width failed: nRet {0:x8}", nRet);
                    break;
                }
                stRecordPar.enPixelType = (MyCamera.MvGvspPixelType)stEnumValue.nCurValue;

                MyCamera.MVCC_FLOATVALUE stFloatValue = new MyCamera.MVCC_FLOATVALUE();
                nRet = device.MV_CC_GetFloatValue_NET("ResultingFrameRate", ref stFloatValue);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Get Float value failed: nRet {0:x8}", nRet);
                    break;
                }
                // ch:帧率(大于1/16)fps | en:Frame Rate (>1/16)fps
                stRecordPar.fFrameRate = stFloatValue.fCurValue;
                // ch:码率kbps(128kbps-16Mbps) | en:Bitrate kbps(128kbps-16Mbps)
                stRecordPar.nBitRate = 1000;
                // ch:录像格式(仅支持AVI) | en:Record Format(AVI is only supported)
                stRecordPar.enRecordFmtType = MyCamera.MV_RECORD_FORMAT_TYPE.MV_FormatType_AVI;
                stRecordPar.strFilePath = "./Recording.avi";
                nRet = device.MV_CC_StartRecord_NET(ref stRecordPar);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Start Record failed: nRet {0:x8}", nRet);
                    break;
                }

                // ch:开启抓图 | en:start grab
                nRet = device.MV_CC_StartGrabbing_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Start grabbing failed:{0:x8}", nRet);
                    break;
                }

                Thread hReceiveImageThreadHandle = new Thread(ReceiveImageWorkThread);
                hReceiveImageThreadHandle.Start(device);

                Console.WriteLine("Press enter to exit");
                Console.ReadKey();

                g_bExit = true;
                hReceiveImageThreadHandle.Join();

                // ch:停止抓图 | en:Stop grab image
                nRet = device.MV_CC_StopGrabbing_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Stop grabbing failed{0:x8}", nRet);
                    break;
                }

                // ch:停止录像 | en:Stop record
                nRet = device.MV_CC_StopRecord_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Stop Record failed{0:x8}", nRet);
                    break;
                }

                // ch:关闭设备 | en:Close device
                nRet = device.MV_CC_CloseDevice_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Close device failed{0:x8}", nRet);
                    break;
                }

                // ch:销毁设备 | en:Destroy device
                nRet = device.MV_CC_DestroyDevice_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Destroy device failed:{0:x8}", nRet);
                    break;
                }
            } while (false);

            if (MyCamera.MV_OK != nRet)
            {
                // ch:销毁设备 | en:Destroy device
                nRet = device.MV_CC_DestroyDevice_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Destroy device failed:{0:x8}", nRet);
                }
            }

            Console.WriteLine("Press enter to exit");
            Console.ReadKey();
        }
    }
}

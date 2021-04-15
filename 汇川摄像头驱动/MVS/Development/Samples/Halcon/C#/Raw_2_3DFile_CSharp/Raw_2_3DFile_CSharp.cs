using System;
using System.Collections.Generic;
using MvCamCtrl.NET;
using System.Runtime.InteropServices;
using System.IO;
using HalconDotNet;

namespace Raw_2_3DFile
{
    class Raw_2_3DFile
    {
        static void Main(string[] args)
        {
            int nRet = MyCamera.MV_OK;
            MyCamera device = new MyCamera();

            do
            {
                // ch:枚举设备 | en:Enum deivce
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

                MyCamera.MV_CC_DEVICE_INFO stDevInfo;

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

                // ch:创建设备 | en: Create device
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

                // ch:获取触发模式的访问模式 | en:Get Access mode of trigger mode
                MyCamera.MV_XML_AccessMode pAccessMode = MyCamera.MV_XML_AccessMode.AM_NI;
                nRet = device.MV_XML_GetNodeAccessMode_NET("TriggerMode", ref pAccessMode);
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Get Access mode of trigger mode fail! nRet [0x%x]\n", nRet);
                }
                else
                {
                    // ch:设置触发模式为off || en:set trigger mode as off
                    nRet = device.MV_CC_SetEnumValue_NET("TriggerMode", 0);
                    if (MyCamera.MV_OK != nRet)
                    {
                        Console.WriteLine("Set TriggerMode failed!");
                        break;
                    }
                }

                // ch:开启抓图 || en: start grab image
                nRet = device.MV_CC_StartGrabbing_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Start grabbing failed:{0:x8}", nRet);
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
                UInt32 nPayloadSize = stParam.nCurValue;

                byte[] BufForDriver = new byte[nPayloadSize];
                IntPtr pData = Marshal.UnsafeAddrOfPinnedArrayElement(BufForDriver, 0);
                MyCamera.MV_FRAME_OUT_INFO_EX stFrameInfo = new MyCamera.MV_FRAME_OUT_INFO_EX();

                HTuple hObjectModel3D = null;
                HTuple hv_X = new HTuple();
                HTuple hv_Y = new HTuple();
                HTuple hv_Z = new HTuple();
                Int32 j = 0;

                nRet = device.MV_CC_GetOneFrameTimeout_NET(pData, nPayloadSize, ref stFrameInfo, 1000);
                // 获取一帧图像
                if (MyCamera.MV_OK == nRet)
                {
                    Console.WriteLine("Get One Frame:" + "Width[" + Convert.ToString(stFrameInfo.nWidth) + "] , Height[" + Convert.ToString(stFrameInfo.nHeight)
                                        + "] , FrameNum[" + Convert.ToString(stFrameInfo.nFrameNum) + "]");
                    switch (stFrameInfo.enPixelType)
                    {
                        case MyCamera.MvGvspPixelType.PixelType_Gvsp_Coord3D_ABC32f:
                            {
                                float[] ConvertData = new float[nPayloadSize];
                                Marshal.Copy(pData, ConvertData, 0, (int)stFrameInfo.nFrameLen);
                                for (j = 0; j < stFrameInfo.nWidth; j++)
                                {
                                    hv_X[j] = ConvertData[j * 3];
                                    hv_Y[j] = ConvertData[j * 3 + 1];
                                    hv_Z[j] = ConvertData[j * 3 + 2];
                                }
                            }
                            break;
                        case MyCamera.MvGvspPixelType.PixelType_Gvsp_Coord3D_ABC32:
                            {
                                Int32[] ConvertData = new Int32[nPayloadSize];
                                Marshal.Copy(pData, ConvertData, 0, (int)stFrameInfo.nFrameLen);
                                for (j = 0; j < stFrameInfo.nWidth; j++)
                                {
                                    hv_X[j] = ConvertData[j * 3];
                                    hv_Y[j] = ConvertData[j * 3 + 1];
                                    hv_Z[j] = ConvertData[j * 3 + 2];
                                }
                            }
                            break;
                        case MyCamera.MvGvspPixelType.PixelType_Gvsp_Coord3D_AB32f:
                            {
                                float[] ConvertData = new float[nPayloadSize];
                                Marshal.Copy(pData, ConvertData, 0, (int)stFrameInfo.nFrameLen);
                                for (j = 0; j < stFrameInfo.nWidth; j++)
                                {
                                    hv_X[j] = ConvertData[j * 2];
                                    hv_Y[j] = ConvertData[j * 2 + 1];
                                    hv_Z[j] = 0.0;
                                }
                            }
                            break;
                        case MyCamera.MvGvspPixelType.PixelType_Gvsp_Coord3D_AB32:
                            {
                                Int32[] ConvertData = new Int32[nPayloadSize];
                                Marshal.Copy(pData, ConvertData, 0, (int)stFrameInfo.nFrameLen);
                                for (j = 0; j < stFrameInfo.nWidth; j++)
                                {
                                    hv_X[j] = ConvertData[j * 2];
                                    hv_Y[j] = ConvertData[j * 2 + 1];
                                    hv_Z[j] = 0;
                                }
                            }
                            break;
                        case MyCamera.MvGvspPixelType.PixelType_Gvsp_Coord3D_AC32f:
                            {
                                float[] ConvertData = new float[nPayloadSize];
                                Marshal.Copy(pData, ConvertData, 0, (int)stFrameInfo.nFrameLen);
                                for (j = 0; j < stFrameInfo.nWidth; j++)
                                {
                                    hv_X[j] = ConvertData[j * 2];
                                    hv_Y[j] = 0.0;
                                    hv_Z[j] = ConvertData[j * 2 + 1];
                                }
                            }
                            break;
                        case MyCamera.MvGvspPixelType.PixelType_Gvsp_Coord3D_AC32:
                            {
                                Int32[] ConvertData = new Int32[nPayloadSize];
                                Marshal.Copy(pData, ConvertData, 0, (int)stFrameInfo.nFrameLen);
                                for (j = 0; j < stFrameInfo.nWidth; j++)
                                {
                                    hv_X[j] = ConvertData[j * 2];
                                    hv_Y[j] = 0;
                                    hv_Z[j] = ConvertData[j * 2 + 1];
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
                else
                {
                    Console.WriteLine("No data{0:x8}", nRet);
                }

                // ch:停止抓图 | en:Stop grabbing
                nRet = device.MV_CC_StopGrabbing_NET();
                if (MyCamera.MV_OK != nRet)
                {
                    Console.WriteLine("Stop grabbing failed{0:x8}", nRet);
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

                if (j == 0)
                {
                    Console.Write("Probably not a 3D device!\n");
                    break;
                }

                Console.Write("*********************************************************************\n");
                Console.Write("*  0.ply;  1.obj;  *\n");
                Console.Write("*********************************************************************\n");

                Int32 nFileType = 0;
                Console.Write("Select FileType: ");
                try
                {
                    nFileType = Convert.ToInt32(Console.ReadLine());
                }
                catch
                {
                    Console.Write("Invalid Input!\n");
                    break;
                }

                HOperatorSet.GenObjectModel3dFromPoints(hv_X, hv_Y, hv_Z, out hObjectModel3D);
                switch (nFileType)
                {
                    case 0:
                        {
                            HOperatorSet.WriteObjectModel3d(hObjectModel3D, "ply", "./3DFile.ply", "invert_normals", "false");
                        }
                        break;
                    case 1:
                        {
                            HOperatorSet.WriteObjectModel3d(hObjectModel3D, "obj", "./3DFile.obj", "invert_normals", "false");
                        }
                        break;
                    default:
                        Console.Write("Input error!\n");
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

Imports System.Runtime.InteropServices
Imports System.Threading
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Module GrabStrategies

    Dim dev As MyCamera = New MyCamera

    Sub UpcomingThread()
        Thread.Sleep(3000)

        dev.MV_CC_SetCommandValue_NET("TriggerSoftware")
    End Sub

    Sub Main()
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stDeviceInfoList As MyCamera.MV_CC_DEVICE_INFO_LIST = New MyCamera.MV_CC_DEVICE_INFO_LIST

        Do While (True)
            ' ch:枚举设备 | en:Enum device
            nRet = MyCamera.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE Or MyCamera.MV_USB_DEVICE), stDeviceInfoList)
            If MyCamera.MV_OK <> nRet Then
                Console.WriteLine("Enum Device failed:{0:x8}", nRet)
                Exit Do
            End If

            If (0 = stDeviceInfoList.nDeviceNum) Then
                Console.WriteLine("No Find Gige | Usb Device !")
                Exit Do
            End If

            '  ch:打印设备信息 | en:Print device info
            Dim i As Int32
            For i = 0 To stDeviceInfoList.nDeviceNum - 1
                Dim stDeviceInfo As MyCamera.MV_CC_DEVICE_INFO = New MyCamera.MV_CC_DEVICE_INFO
                stDeviceInfo = CType(Marshal.PtrToStructure(stDeviceInfoList.pDeviceInfo(i), GetType(MyCamera.MV_CC_DEVICE_INFO)), MyCamera.MV_CC_DEVICE_INFO)
                If (MyCamera.MV_GIGE_DEVICE = stDeviceInfo.nTLayerType) Then
                    Dim stGigeInfoPtr As IntPtr = Marshal.AllocHGlobal(216)
                    Marshal.Copy(stDeviceInfo.SpecialInfo.stGigEInfo, 0, stGigeInfoPtr, 216)
                    Dim stGigeInfo As MyCamera.MV_GIGE_DEVICE_INFO
                    stGigeInfo = CType(Marshal.PtrToStructure(stGigeInfoPtr, GetType(MyCamera.MV_GIGE_DEVICE_INFO)), MyCamera.MV_GIGE_DEVICE_INFO)
                    Dim nIpByte1 As UInt32 = (stGigeInfo.nCurrentIp And &HFF000000) >> 24
                    Dim nIpByte2 As UInt32 = (stGigeInfo.nCurrentIp And &HFF0000) >> 16
                    Dim nIpByte3 As UInt32 = (stGigeInfo.nCurrentIp And &HFF00) >> 8
                    Dim nIpByte4 As UInt32 = (stGigeInfo.nCurrentIp And &HFF)

                    Console.WriteLine("DEV[" + Convert.ToString(i) + "]: NAME[" + stGigeInfo.chUserDefinedName + "]")
                    Console.WriteLine("IP[" + nIpByte1.ToString() + "." + nIpByte2.ToString() + "." + nIpByte3.ToString() + "." + nIpByte4.ToString() + "]")
                    Console.WriteLine("")
                Else
                    Dim stUsbInfoPtr As IntPtr = Marshal.AllocHGlobal(540)
                    Marshal.Copy(stDeviceInfo.SpecialInfo.stUsb3VInfo, 0, stUsbInfoPtr, 540)
                    Dim stUsbInfo As MyCamera.MV_USB3_DEVICE_INFO
                    stUsbInfo = CType(Marshal.PtrToStructure(stUsbInfoPtr, GetType(MyCamera.MV_USB3_DEVICE_INFO)), MyCamera.MV_USB3_DEVICE_INFO)

                    Console.WriteLine("U3V[" + Convert.ToString(i) + "]: NAME[" + stUsbInfo.chUserDefinedName + "]")
                    Console.WriteLine("Model[" + stUsbInfo.chSerialNumber + "]")
                    Console.WriteLine("")
                End If
            Next

            Console.Write("please select a device:")
            Dim nIndex As Int32
            Try
                nIndex = Console.ReadLine()
            Catch ex As Exception
                Console.WriteLine("Invalid input!")
                Console.WriteLine("push enter to exit")
                System.Console.ReadLine()
                End
            End Try

            If nIndex > stDeviceInfoList.nDeviceNum - 1 Then
                Console.WriteLine("Invalid input!")
                Console.WriteLine("push enter to exit")
                System.Console.ReadLine()
                End
            End If

            If nIndex < 0 Then
                Console.WriteLine("Invalid input!")
                Console.WriteLine("push enter to exit")
                System.Console.ReadLine()
                End
            End If

            Dim stdevInfo As MyCamera.MV_CC_DEVICE_INFO
            stdevInfo = CType(Marshal.PtrToStructure(stDeviceInfoList.pDeviceInfo(nIndex), GetType(MyCamera.MV_CC_DEVICE_INFO)), MyCamera.MV_CC_DEVICE_INFO)

            ' ch:创建句柄 | en:Create handle
            nRet = dev.MV_CC_CreateDevice_NET(stdevInfo)
            If 0 <> nRet Then
                Console.WriteLine("Create device failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:打开相机 | en:Open device
            nRet = dev.MV_CC_OpenDevice_NET()
            If 0 <> nRet Then
                Console.WriteLine("Open device failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
            If stdevInfo.nTLayerType = MyCamera.MV_GIGE_DEVICE Then
                Dim nPacketSize As Int32
                nPacketSize = dev.MV_CC_GetOptimalPacketSize_NET()
                If nPacketSize > 0 Then
                    nRet = dev.MV_CC_SetIntValue_NET("GevSCPSPacketSize", nPacketSize)
                    If 0 <> nRet Then
                        Console.WriteLine("Warning: Set Packet Size failed:{0:x8}", nRet)
                    End If
                Else
                    Console.WriteLine("Warning: Get Packet Size failed:{0:x8}", nPacketSize)
                End If
            End If

            ' ch:设置软触发模式 | en:Set Trigger Mode and Set Trigger Source
            nRet = dev.MV_CC_SetEnumValueByString_NET("TriggerMode", "On")
            If 0 <> nRet Then
                Console.WriteLine("Set Trigger Mode failed:{0:x8}", nRet)
                Exit Do
            End If
            nRet = dev.MV_CC_SetEnumValueByString_NET("TriggerSource", "Software")
            If 0 <> nRet Then
                Console.WriteLine("Set Trigger Source failed:{0:x8}", nRet)
                Exit Do
            End If

            Dim nImageNodeNum As UInt32 = 5
            ' ch:设置缓存节点个数 | en:Set number of image node
            nRet = dev.MV_CC_SetImageNodeNum_NET(nImageNodeNum)
            If 0 <> nRet Then
                Console.WriteLine("Set number of image node fail:{0:x8}", nRet)
                Exit Do
            End If

            Console.WriteLine("**************************************************************************")
            Console.WriteLine("* 0.MV_GrabStrategy_OneByOne;       1.MV_GrabStrategy_LatestImagesOnly;  *")
            Console.WriteLine("* 2.MV_GrabStrategy_LatestImages;   3.MV_GrabStrategy_UpcomingImage;     *")
            Console.WriteLine("**************************************************************************")

            Console.Write("Please Intput Grab Strategy:")
            Dim nGrabStrategy As Int32
            Try
                nGrabStrategy = Console.ReadLine()
            Catch ex As Exception
                Console.WriteLine("Invalid input!")
                Exit Do
            End Try

            If nGrabStrategy = MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_UpcomingImage And MyCamera.MV_USB_DEVICE = stdevInfo.nTLayerType Then
                Console.WriteLine("U3V device not support UpcomingImage")
                Exit Do
            End If

            Select Case nGrabStrategy
                Case MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_OneByOne
                    Console.WriteLine("Grab using the MV_GrabStrategy_OneByOne default strategy")
                    nRet = dev.MV_CC_SetGrabStrategy_NET(MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_OneByOne)
                    If 0 <> nRet Then
                        Console.WriteLine("Set Grab Strategy fail:{0:x8}", nRet)
                        Exit Do
                    End If

                Case MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_LatestImagesOnly
                    Console.WriteLine("Grab using strategy MV_GrabStrategy_LatestImagesOnly")
                    nRet = dev.MV_CC_SetGrabStrategy_NET(MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_LatestImagesOnly)
                    If 0 <> nRet Then
                        Console.WriteLine("Set Grab Strategy fail:{0:x8}", nRet)
                        Exit Do
                    End If

                Case MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_LatestImages
                    Console.WriteLine("Grab using strategy MV_GrabStrategy_LatestImages")
                    nRet = dev.MV_CC_SetGrabStrategy_NET(MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_LatestImages)
                    If 0 <> nRet Then
                        Console.WriteLine("Set Grab Strategy fail:{0:x8}", nRet)
                        Exit Do
                    End If

                    ' ch:设置输出缓存个数 | en:Set Output Queue Size
                    nRet = dev.MV_CC_SetOutputQueueSize_NET(2)
                    If 0 <> nRet Then
                        Console.WriteLine("Set Grab Strategy fail:{0:x8}", nRet)
                        Exit Do
                    End If

                Case MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_UpcomingImage
                    Console.WriteLine("Grab using strategy MV_GrabStrategy_UpcomingImage")
                    nRet = dev.MV_CC_SetGrabStrategy_NET(MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_UpcomingImage)
                    If 0 <> nRet Then
                        Console.WriteLine("Set Grab Strategy fail:{0:x8}", nRet)
                        Exit Do
                    End If

                    Dim hUpcomingThread As New System.Threading.Thread(AddressOf UpcomingThread)
                    hUpcomingThread.Start()

                Case Else
                    Console.WriteLine("Input error!Use default strategy:MV_GrabStrategy_OneByOne")
            End Select

            ' ch:开启抓图 | en:start grab image
            nRet = dev.MV_CC_StartGrabbing_NET()
            If 0 <> nRet Then
                Console.WriteLine("Start grabbing failed:{0:x8}", nRet)
                Exit Do
            End If

            For i = 0 To nImageNodeNum - 1
                nRet = dev.MV_CC_SetCommandValue_NET("TriggerSoftware")
                If 0 <> nRet Then
                    Console.WriteLine("Send Trigger Software command fail:{0:x8}", nRet)
                    Exit Do
                End If
                '如果帧率过小或TriggerDelay很大，可能会出现软触发命令没有全部起效而导致取不到数据的情况
                Threading.Thread.Sleep(500)
            Next

            Dim stOutFrame As MyCamera.MV_FRAME_OUT = New MyCamera.MV_FRAME_OUT
            If nGrabStrategy <> MyCamera.MV_GRAB_STRATEGY.MV_GrabStrategy_UpcomingImage Then
                While (True)
                    nRet = dev.MV_CC_GetImageBuffer_NET(stOutFrame, 0)
                    If 0 = nRet Then
                        Console.WriteLine("Get Image Buffer:" + "Width[" + Convert.ToString(stOutFrame.stFrameInfo.nWidth) + "] , Height[" + Convert.ToString(stOutFrame.stFrameInfo.nHeight) + "] , FrameNum[" + Convert.ToString(stOutFrame.stFrameInfo.nFrameNum) + "]")
                    Else
                        Console.WriteLine("No data:{0:x8}", nRet)
                        Exit While
                    End If

                    nRet = dev.MV_CC_FreeImageBuffer_NET(stOutFrame)
                    If 0 <> nRet Then
                        Console.WriteLine("Free Image Buffer fail:{0:x8}", nRet)
                    End If
                End While
            Else
                nRet = dev.MV_CC_GetImageBuffer_NET(stOutFrame, 5000)
                If 0 = nRet Then
                    Console.WriteLine("Get Image Buffer:" + "Width[" + Convert.ToString(stOutFrame.stFrameInfo.nWidth) + "] , Height[" + Convert.ToString(stOutFrame.stFrameInfo.nHeight) + "] , FrameNum[" + Convert.ToString(stOutFrame.stFrameInfo.nFrameNum) + "]")

                    nRet = dev.MV_CC_FreeImageBuffer_NET(stOutFrame)
                    If 0 <> nRet Then
                        Console.WriteLine("Free Image Buffer fail:{0:x8}", nRet)
                    End If
                Else
                    Console.WriteLine("No data:{0:x8}", nRet)
                End If
            End If

            ' ch:停止抓图 | en:Stop grab image
            nRet = dev.MV_CC_StopGrabbing_NET()
            If 0 <> nRet Then
                Console.WriteLine("Stop grabbing failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:关闭相机 | en:Close device
            nRet = dev.MV_CC_CloseDevice_NET()
            If 0 <> nRet Then
                Console.WriteLine("Open device failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:销毁句柄 | en:Destroy handle
            nRet = dev.MV_CC_DestroyDevice_NET()
            If 0 <> nRet Then
                Console.WriteLine("Destroy device failed:{0:x8}", nRet)
            End If

            Exit Do
        Loop

        If 0 <> nRet Then
            ' ch:销毁句柄 | en:Destroy handle
            nRet = dev.MV_CC_DestroyDevice_NET()
            If 0 <> nRet Then
                Console.WriteLine("Destroy device failed:{0:x8}", nRet)
            End If
        End If

        Console.WriteLine("Press enter to exit")
        System.Console.ReadLine()
    End Sub

End Module

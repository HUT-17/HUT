Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Module Grab_Callback

    Dim dev As MyCamera = New MyCamera
    Dim pBufForSaveImage As IntPtr
    Dim nBufForSaveImage As Int32
    Dim m_byteImageBuffer(1024 * 1024 * 1) As Byte
    Dim m_byteImageBufferLen As Int32 = 1024 * 1024 * 1

    Private Sub cbOutputdelegateFunc(ByVal pData As IntPtr, ByRef pFrameInfo As MyCamera.MV_FRAME_OUT_INFO, ByVal pUser As IntPtr)
        Console.WriteLine("Width:" + Convert.ToString(pFrameInfo.nWidth) + " Height:" + Convert.ToString(pFrameInfo.nHeight) + " FrameNum:" + Convert.ToString(pFrameInfo.nFrameNum))
    End Sub

    Sub Main()
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stDeviceInfoList As MyCamera.MV_CC_DEVICE_INFO_LIST = New MyCamera.MV_CC_DEVICE_INFO_LIST
        Dim cbCallback As MyCamera.cbOutputdelegate = New MyCamera.cbOutputdelegate(AddressOf cbOutputdelegateFunc)

        Do While (True)
            ' ch:枚举设备 | en:Enum device
            nRet = MyCamera.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE Or MyCamera.MV_USB_DEVICE), stDeviceInfoList)
            If MyCamera.MV_OK <> nRet Then
                Console.WriteLine("Enum Device failed:{0:x8}", nRet)
                Return
            End If

            If (0 = stDeviceInfoList.nDeviceNum) Then
                Console.WriteLine("No Find Gige | Usb Device !")
                Return
            End If

            ' ch:打印设备信息 | en:Print device info
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

            ' ch:获取包大小 || en: Get Payload Size
            Dim stParam As MyCamera.MVCC_INTVALUE = New MyCamera.MVCC_INTVALUE()
            nRet = dev.MV_CC_GetIntValue_NET("PayloadSize", stParam)
            If (MyCamera.MV_OK <> nRet) Then
                Console.WriteLine("Get PayloadSize failed:{0:x8}", nRet)
                Exit Do
            End If

            nBufForSaveImage = stParam.nCurValue * 3 + 2048
            pBufForSaveImage = Marshal.AllocHGlobal(nBufForSaveImage)

            ' ch:注册抓图回调 | en:Register image callback
            nRet = dev.MV_CC_RegisterImageCallBack_NET(cbCallback, 0)
            If MyCamera.MV_OK <> nRet Then
                Console.WriteLine("Register image callback failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:开启取流 | en:Start grabbing
            nRet = dev.MV_CC_StartGrabbing_NET()
            If 0 <> nRet Then
                Console.WriteLine("Start grabbing fauled:{0:x8}", nRet)
                Exit Do
            End If

            Console.WriteLine("Press enter to continue")
            System.Console.ReadLine()

            ' ch:停止取流 | en:Stop grabbing
            nRet = dev.MV_CC_StopGrabbing_NET()
            If 0 <> nRet Then
                Console.WriteLine("Stop Grabbing failed:{0:x8}", nRet)
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
                Exit Do
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

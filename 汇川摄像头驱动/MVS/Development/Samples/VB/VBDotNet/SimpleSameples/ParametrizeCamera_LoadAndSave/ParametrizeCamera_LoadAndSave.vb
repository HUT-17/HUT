Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports MvCamCtrl.NET


Module ParametrizeCamera_LoadAndSave

    Sub Main()
        Dim dev As MyCamera = New MyCamera
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

            Console.WriteLine("Start export the camera properties to the file")
            Console.WriteLine("Wait......")
            ' ch:将相机属性导出到文件中
            ' en:Export the camera properties to the file
            nRet = dev.MV_CC_FeatureSave_NET("CameraFile_save")
            If (MyCamera.MV_OK <> nRet) Then
                Console.WriteLine("FeatureSave failed!")
                Exit Do
            End If
            Console.WriteLine("Finish export the camera properties to the file")

            Console.WriteLine("Start import the camera properties from the file")
            Console.WriteLine("Wait......")
            ' ch:从文件中导入相机属性
            ' en:Import the camera properties from the file
            nRet = dev.MV_CC_FeatureLoad_NET("CameraFile_save")
            If (MyCamera.MV_OK <> nRet) Then
                Console.WriteLine("FeatureLoad failed!")
                Exit Do
            End If
            Console.WriteLine("Finish import the camera properties from the file")

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

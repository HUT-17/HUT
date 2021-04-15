Imports System.Runtime.InteropServices
Imports System.Threading
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Module ParametrizeCamera_FileAccess

    Dim dev As MyCamera = New MyCamera
    Dim g_nRet As Int32 = MyCamera.MV_OK
    Dim g_nMode As UInt32 = 0

    Sub FileAccessProgress()
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stFileAccessProgress As MyCamera.MV_CC_FILE_ACCESS_PROGRESS = New MyCamera.MV_CC_FILE_ACCESS_PROGRESS()

        While (True)
            'ch:获取文件存取进度 |en:Get progress of file access
            nRet = dev.MV_CC_GetFileAccessProgress_NET(stFileAccessProgress)
            Console.WriteLine("State = {0:x8},Completed = {1},Total = {2}", nRet, stFileAccessProgress.nCompleted, stFileAccessProgress.nTotal)
            If (nRet <> MyCamera.MV_OK Or (stFileAccessProgress.nCompleted <> 0 And stFileAccessProgress.nCompleted = stFileAccessProgress.nTotal)) Then
                Return
            End If

            Thread.Sleep(50)
        End While
    End Sub

    Sub FileAccessThread()
        Dim stFileAccess As MyCamera.MV_CC_FILE_ACCESS = New MyCamera.MV_CC_FILE_ACCESS()

        stFileAccess.pUserFileName = "UserSet1.bin"
        stFileAccess.pDevFileName = "UserSet1"
        If 1 = g_nMode Then
            'ch:读模式 |en:Read mode
            g_nRet = dev.MV_CC_FileAccessRead_NET(stFileAccess)
            If (MyCamera.MV_OK <> g_nRet) Then
                Console.WriteLine("File Access Read failed:{0:x8}", g_nRet)
            End If
        End If

        If 2 = g_nMode Then
            'ch:写模式 |en:Write mode
            g_nRet = dev.MV_CC_FileAccessWrite_NET(stFileAccess)
            If (MyCamera.MV_OK <> g_nRet) Then
                Console.WriteLine("File Access Write failed:{0:x8}", g_nRet)
            End If
        End If
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

            'ch:读模式 |en:Read mode
            Console.WriteLine("Read to file")
            g_nMode = 1

            Dim hReadHandle As New System.Threading.Thread(AddressOf FileAccessThread)
            hReadHandle.Start()

            Thread.Sleep(5)

            Dim hReadProgressHandle As New System.Threading.Thread(AddressOf FileAccessProgress)
            hReadProgressHandle.Start()

            hReadProgressHandle.Join()
            hReadHandle.Join()
            If 0 = g_nRet Then
                Console.WriteLine("File Access Read Success")
            End If

            Console.WriteLine("")

            'ch:写模式 |en:Write mode
            Console.WriteLine("Write to file")
            g_nMode = 2

            Dim hWriteHandle As New System.Threading.Thread(AddressOf FileAccessThread)
            hWriteHandle.Start()

            Thread.Sleep(5)

            Dim hWriteProgressHandle As New System.Threading.Thread(AddressOf FileAccessProgress)
            hWriteProgressHandle.Start()

            hWriteProgressHandle.Join()
            hWriteHandle.Join()
            If 0 = g_nRet Then
                Console.WriteLine("File Access Write Success")
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

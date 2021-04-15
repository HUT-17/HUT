Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Module MultiCast

    Sub Main()
        Dim dev As MyCamera = New MyCamera
        Dim Info As String
        Dim nRet As Int32 = MyCamera.MV_OK
        Dim stDeviceInfoList As MyCamera.MV_CC_DEVICE_INFO_LIST = New MyCamera.MV_CC_DEVICE_INFO_LIST

        Do While (True)
            ' ch:枚举设备 | en:Enum device
            nRet = MyCamera.MV_CC_EnumDevices_NET((MyCamera.MV_GIGE_DEVICE), stDeviceInfoList)
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

            '  ch:创建句柄 | en:Create handle
            nRet = dev.MV_CC_CreateDevice_NET(stdevInfo)
            If 0 <> nRet Then
                Console.WriteLine("Create device failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:查询用户使用的模式
            ' Query the user for the mode to use.
            Dim monitorMode As Boolean = False
            Dim key As String

            ' ch:询问用户启动多播控制应用程序或多播监控应用程序
            ' Ask the user to launch: the multicast controlling application or the multicast monitoring application.
            Console.WriteLine("Start multicast sample in (c)ontrol or in (m)onitor mode? (c/m)")
            key = Convert.ToString(Console.ReadLine())
            While ((key <> "c") And (key <> "m") And (key <> "C") And (key <> "M"))
            End While
            monitorMode = (key = "m") Or (key = "M")


            ' ch:打开相机 | en:Open device
            If (monitorMode) Then
                nRet = dev.MV_CC_OpenDevice_NET(MyCamera.MV_ACCESS_Monitor, 0)
                If (MyCamera.MV_OK <> nRet) Then
                    Console.WriteLine("Open device failed:{0:x8}", nRet)
                    Exit Do
                End If
            Else
                nRet = dev.MV_CC_OpenDevice_NET(MyCamera.MV_ACCESS_Control, 0)
                If (MyCamera.MV_OK <> nRet) Then
                    Console.WriteLine("Open device failed:{0:x8}", nRet)
                    Exit Do
                End If
            End If

            ' ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
            If stdevInfo.nTLayerType = MyCamera.MV_GIGE_DEVICE And monitorMode = False Then
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

            ' 指定组播ip
            Dim strIp As String = "239.0.1.23"
            Dim nIp1 As UInt32 = Convert.ToUInt32(Split(strIp, ".")(0))
            Dim nIp2 As UInt32 = Convert.ToUInt32(Split(strIp, ".")(1))
            Dim nIp3 As UInt32 = Convert.ToUInt32(Split(strIp, ".")(2))
            Dim nIp4 As UInt32 = Convert.ToUInt32(Split(strIp, ".")(3))
            Dim nIp As UInt32 = ((nIp1 << 24) Or (nIp2 << 16) Or (nIp3 << 8) Or nIp4)

            ' ch:可指定端口号作为组播组端口 | en:multicast port
            Dim stTransmissionType As MyCamera.MV_CC_TRANSMISSION_TYPE = New MyCamera.MV_CC_TRANSMISSION_TYPE()
            stTransmissionType.enTransmissionType = MyCamera.MV_GIGE_TRANSMISSION_TYPE.MV_GIGE_TRANSTYPE_MULTICAST
            stTransmissionType.nDestIp = nIp
            stTransmissionType.nDestPort = 8787

            nRet = dev.MV_GIGE_SetTransmissionType_NET(stTransmissionType)
            If (MyCamera.MV_OK <> nRet) Then
                Console.WriteLine("MV_GIGE_SetTransmissionType fail! nRet [%x]\n", nRet)
                Exit Do
            End If

            ' ch:开启取流 | en:Start grabbing
            nRet = dev.MV_CC_StartGrabbing_NET()
            If 0 <> nRet Then
                Console.WriteLine("Start grabbing fauled:{0:x8}", nRet)
            End If


            Dim stParam As MyCamera.MVCC_INTVALUE = New MyCamera.MVCC_INTVALUE()
            nRet = dev.MV_CC_GetIntValue_NET("PayloadSize", stParam)
            If (MyCamera.MV_OK <> nRet) Then
                Console.WriteLine("Get PayloadSize failed:{0:x8}", nRet)
                Exit Do
            End If
            Dim nPayloadSize As Int32 = stParam.nCurValue

            ' ch:抓取图像 | en:Get image
            Dim nCount As Int32 = 0
            Dim pBufForDriver As IntPtr = Marshal.AllocHGlobal(nPayloadSize)
            Dim pBufForSaveImage As IntPtr = Marshal.AllocHGlobal(nPayloadSize * 3 + 2048)
            Dim FrameInfo As MyCamera.MV_FRAME_OUT_INFO_EX = New MyCamera.MV_FRAME_OUT_INFO_EX
            Dim m_byteImageBuffer(1024 * 1024) As Byte
            Dim m_byteImageBufferLen As Int32 = 1024 * 1024

            Do While nCount <> 10
                nCount = nCount + 1
                nRet = dev.MV_CC_GetOneFrameTimeout_NET(pBufForDriver, nPayloadSize, FrameInfo, 1000)
                If MyCamera.MV_OK = nRet Then
                    Console.WriteLine("Width:" + Convert.ToString(FrameInfo.nWidth) + " Height:" + Convert.ToString(FrameInfo.nHeight) + " FrameNum:" + Convert.ToString(FrameInfo.nFrameNum))
                    Dim stSaveParam As MyCamera.MV_SAVE_IMAGE_PARAM_EX = New MyCamera.MV_SAVE_IMAGE_PARAM_EX()
                    stSaveParam.enImageType = MyCamera.MV_SAVE_IAMGE_TYPE.MV_Image_Bmp
                    stSaveParam.enPixelType = FrameInfo.enPixelType
                    stSaveParam.pData = pBufForDriver
                    stSaveParam.nDataLen = FrameInfo.nFrameLen
                    stSaveParam.nHeight = FrameInfo.nHeight
                    stSaveParam.nWidth = FrameInfo.nWidth
                    stSaveParam.pImageBuffer = pBufForSaveImage
                    stSaveParam.nBufferSize = nPayloadSize * 3 + 2048
                    stSaveParam.nJpgQuality = 80
                    nRet = dev.MV_CC_SaveImageEx_NET(stSaveParam)
                    If MyCamera.MV_OK <> nRet Then
                        Console.WriteLine("Save Image failed:{0:x8}", nRet)
                        Continue Do
                    End If

                    ' ch:将图像数据保存到本地文件 | en:Save image data to local file
                    If stSaveParam.nImageLen <> m_byteImageBufferLen Then
                        m_byteImageBufferLen = stSaveParam.nImageLen
                        ReDim m_byteImageBuffer(m_byteImageBufferLen)
                    End If
                    Marshal.Copy(pBufForSaveImage, m_byteImageBuffer, 0, m_byteImageBufferLen)
                    My.Computer.FileSystem.WriteAllBytes("Image" + nCount.ToString() + ".bmp", m_byteImageBuffer, False)
                    Continue Do
                Else
                    Console.WriteLine("Get one frame failed:{0:x8}", nRet)
                End If
            Loop
            Marshal.FreeHGlobal(pBufForDriver)
            Marshal.FreeHGlobal(pBufForSaveImage)

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

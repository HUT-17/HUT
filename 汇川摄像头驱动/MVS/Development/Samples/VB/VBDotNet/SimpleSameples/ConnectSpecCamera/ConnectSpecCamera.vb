Imports System.Runtime.InteropServices
Imports System.Threading.Thread
Imports System.Net.IPAddress
Imports MvCamCtrl.NET

Module ConnectSpecCamera

    Sub Main()
        Dim dev As MyCamera = New MyCamera
        Dim nRet As Int32 = MyCamera.MV_OK

        Dim stDevInfo As MyCamera.MV_CC_DEVICE_INFO = New MyCamera.MV_CC_DEVICE_INFO
        stDevInfo.nTLayerType = MyCamera.MV_GIGE_DEVICE
        Dim stGigEDev As MyCamera.MV_GIGE_DEVICE_INFO = New MyCamera.MV_GIGE_DEVICE_INFO

        ' ch:需要连接的相机ip(根据实际填充) | en:The camera IP that needs to be connected (based on actual padding)
        Console.WriteLine("Please input Device Ip : ")
        Dim strCurrentIp As String
        strCurrentIp = Console.ReadLine()
        ' ch:相机对应的网卡ip(根据实际填充) | en:The pc IP that needs to be connected (based on actual padding)
        Console.WriteLine("Please Net Export Ip : ")
        Dim strNetExport As String
        strNetExport = Console.ReadLine()
        Try
            Dim nIp1 As UInt32 = Convert.ToUInt32(Split(strCurrentIp, ".")(0))
            Dim nIp2 As UInt32 = Convert.ToUInt32(Split(strCurrentIp, ".")(1))
            Dim nIp3 As UInt32 = Convert.ToUInt32(Split(strCurrentIp, ".")(2))
            Dim nIp4 As UInt32 = Convert.ToUInt32(Split(strCurrentIp, ".")(3))
            stGigEDev.nCurrentIp = ((nIp1 << 24) Or (nIp2 << 16) Or (nIp3 << 8) Or nIp4)

            nIp1 = Convert.ToInt32(Split(strNetExport, ".")(0))
            nIp2 = Convert.ToInt32(Split(strNetExport, ".")(1))
            nIp3 = Convert.ToInt32(Split(strNetExport, ".")(2))
            nIp4 = Convert.ToInt32(Split(strNetExport, ".")(3))
            stGigEDev.nNetExport = ((nIp1 << 24) Or (nIp2 << 16) Or (nIp3 << 8) Or nIp4)
        Catch ex As Exception
            Console.WriteLine("Invalid input!")
            Console.WriteLine("push enter to exit")
            System.Console.ReadLine()
            End
        End Try
        Dim stGigeInfoPtr As IntPtr = Marshal.AllocHGlobal(216)
        Marshal.StructureToPtr(stGigEDev, stGigeInfoPtr, False)
        ReDim stDevInfo.SpecialInfo.stGigEInfo(539)
        Marshal.Copy(stGigeInfoPtr, stDevInfo.SpecialInfo.stGigEInfo, 0, 540)

        Do While (True)
            ' ch:创建句柄 | en:Create handle
            nRet = dev.MV_CC_CreateDevice_NET(stDevInfo)
            If 0 <> nRet Then
                Console.WriteLine("Create device failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:打开相机 | en:Open devic
            nRet = dev.MV_CC_OpenDevice_NET()
            If 0 <> nRet Then
                Console.WriteLine("Open device failed:{0:x8}", nRet)
                Exit Do
            End If

            ' ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
            If stDevInfo.nTLayerType = MyCamera.MV_GIGE_DEVICE Then
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

            ' ch:开启取流 | en:Start grabbing
            nRet = dev.MV_CC_StartGrabbing_NET()
            If 0 <> nRet Then
                Console.WriteLine("Start grabbing fauled:{0:x8}", nRet)
                Exit Do
            End If


            ' ch:获取包大小 || en: Get Payload Size
            Dim stParam As MyCamera.MVCC_INTVALUE = New MyCamera.MVCC_INTVALUE()
            nRet = dev.MV_CC_GetIntValue_NET("PayloadSize", stParam)
            If (MyCamera.MV_OK <> nRet) Then
                Console.WriteLine("Get PayloadSize failed:{0:x8}", nRet)
                Exit Do
            End If
            Dim nPayloadSize As Int32 = stParam.nCurValue

            Dim nCount As Int32 = 0
            Dim pBufForDriver As IntPtr = Marshal.AllocHGlobal(nPayloadSize)
            Dim pBufForSaveImage As IntPtr = Marshal.AllocHGlobal(nPayloadSize * 3 + 2048)
            Dim FrameInfo As MyCamera.MV_FRAME_OUT_INFO_EX = New MyCamera.MV_FRAME_OUT_INFO_EX
            Dim m_byteImageBuffer(nPayloadSize * 3 + 2048) As Byte
            Dim m_byteImageBufferLen As Int32 = nPayloadSize * 3 + 2048

            ' ch:抓取图像 | en:Get image
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
                    Try
                        My.Computer.FileSystem.WriteAllBytes("Image" + nCount.ToString() + ".bmp", m_byteImageBuffer, False)
                    Catch ex As Exception
                    End Try
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

        Console.WriteLine("push enter to exit")
        System.Console.ReadLine()
    End Sub

End Module

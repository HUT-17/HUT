using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MvCamCtrl.NET;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;
using Cognex.VisionPro;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.Collections.ObjectModel;

namespace Raw2ICogImage_CSharp
{
    public partial class Form1 : Form
    {
        #region 参数

        MyCamera.MV_CC_DEVICE_INFO_LIST m_pDeviceList;
        private MyCamera m_pMyCamera;
        bool m_bGrabbing;
        byte[] m_pDataForRed = new byte[20 * 1024 * 1024];
        byte[] m_pDataForGreen = new byte[20 * 1024 * 1024];
        byte[] m_pDataForBlue = new byte[20 * 1024 * 1024];
        UInt32 g_nPayloadSize = 0;
        UInt32 m_nRowStep = 0;

        #endregion

        public Form1()
        {
            InitializeComponent();
        }

        private void bnEnum_Click(object sender, EventArgs e)
        {
            DeviceListAcq();
        }

        private void bnOpen_Click(object sender, EventArgs e)
        {
            if (m_pDeviceList.nDeviceNum == 0 || cbDeviceList.SelectedIndex == -1)
            {
                MessageBox.Show("No device,please select");
                return;
            }
            int nRet = -1;

            //ch:获取选择的设备信息 | en:Get selected device information
            MyCamera.MV_CC_DEVICE_INFO device =
                (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(m_pDeviceList.pDeviceInfo[cbDeviceList.SelectedIndex],
                                                              typeof(MyCamera.MV_CC_DEVICE_INFO));

            m_pMyCamera = new MyCamera();
            nRet = m_pMyCamera.MV_CC_CreateDevice_NET(ref device);
            if (MyCamera.MV_OK != nRet)
            {
                return;
            }

            // ch:打开设备 | en:Open device
            nRet = m_pMyCamera.MV_CC_OpenDevice_NET();
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Open Device Fail");
                return;
            }

            // ch:获取包大小 || en: Get Payload Size
            MyCamera.MVCC_INTVALUE stParam = new MyCamera.MVCC_INTVALUE();
            nRet = m_pMyCamera.MV_CC_GetIntValue_NET("PayloadSize", ref stParam);
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Get PayloadSize Fail");
                return;
            }
            g_nPayloadSize = stParam.nCurValue;

            // ch:获取高 || en: Get Height
            nRet = m_pMyCamera.MV_CC_GetIntValue_NET("Height", ref stParam);
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Get Height Fail");
                return;
            }
            uint nHeight = stParam.nCurValue;

            // ch:获取宽 || en: Get Width
            nRet = m_pMyCamera.MV_CC_GetIntValue_NET("Width", ref stParam);
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Get Width Fail");
                return;
            }
            uint nWidth = stParam.nCurValue;

            // ch:获取步长 || en: Get nRowStep
            m_nRowStep = nWidth * nHeight;

            // ch:设置触发模式为off || en:set trigger mode as off
            m_pMyCamera.MV_CC_SetEnumValue_NET("AcquisitionMode", 2);
            m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", 0);

            SetCtrlWhenOpen();

        }

        /// <summary>
        /// 枚举相机
        /// </summary>
        private void DeviceListAcq()
        {
            int nRet;
            // ch:创建设备列表 || en: Create device list
            System.GC.Collect();
            cbDeviceList.Items.Clear();
            nRet = MyCamera.MV_CC_EnumDevices_NET(MyCamera.MV_GIGE_DEVICE | MyCamera.MV_USB_DEVICE, ref m_pDeviceList);
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Enum Devices Fail");
                return;
            }

            // ch:在窗体列表中显示设备名 || Display the device'name on window's list
            for (int i = 0; i < m_pDeviceList.nDeviceNum; i++)
            {
                MyCamera.MV_CC_DEVICE_INFO device = (MyCamera.MV_CC_DEVICE_INFO)Marshal.PtrToStructure(m_pDeviceList.pDeviceInfo[i], typeof(MyCamera.MV_CC_DEVICE_INFO));
                if (device.nTLayerType == MyCamera.MV_GIGE_DEVICE)
                {
                    IntPtr buffer = Marshal.UnsafeAddrOfPinnedArrayElement(device.SpecialInfo.stGigEInfo, 0);
                    MyCamera.MV_GIGE_DEVICE_INFO gigeInfo = (MyCamera.MV_GIGE_DEVICE_INFO)Marshal.PtrToStructure(buffer, typeof(MyCamera.MV_GIGE_DEVICE_INFO));
                    if (gigeInfo.chUserDefinedName != "")
                    {
                        cbDeviceList.Items.Add("GigE: " + gigeInfo.chUserDefinedName + " (" + gigeInfo.chSerialNumber + ")");
                    }
                    else
                    {
                        cbDeviceList.Items.Add("GigE: " + gigeInfo.chManufacturerName + " " + gigeInfo.chModelName + " (" + gigeInfo.chSerialNumber + ")");
                    }
                }
                else if (device.nTLayerType == MyCamera.MV_USB_DEVICE)
                {
                    IntPtr buffer = Marshal.UnsafeAddrOfPinnedArrayElement(device.SpecialInfo.stUsb3VInfo, 0);
                    MyCamera.MV_USB3_DEVICE_INFO usbInfo = (MyCamera.MV_USB3_DEVICE_INFO)Marshal.PtrToStructure(buffer, typeof(MyCamera.MV_USB3_DEVICE_INFO));
                    if (usbInfo.chUserDefinedName != "")
                    {
                        cbDeviceList.Items.Add("USB: " + usbInfo.chUserDefinedName + " (" + usbInfo.chSerialNumber + ")");
                    }
                    else
                    {
                        cbDeviceList.Items.Add("USB: " + usbInfo.chManufacturerName + " " + usbInfo.chModelName + " (" + usbInfo.chSerialNumber + ")");
                    }
                }
            }

            //.ch: 选择第一项 || en: Select the first item
            if (m_pDeviceList.nDeviceNum != 0)
            {
                cbDeviceList.SelectedIndex = 0;
            }
        }

        /// <summary>
        /// 打开相机时，界面变化
        /// </summary>
        private void SetCtrlWhenOpen()
        {
            bnOpen.Enabled = false;

            bnClose.Enabled = true;
            bnStartGrab.Enabled = true;
            bnStopGrab.Enabled = false;
            bnContinuesMode.Enabled = true;
            bnContinuesMode.Checked = true;
            bnTriggerMode.Enabled = true;
            cbSoftTrigger.Enabled = false;
            bnTriggerExec.Enabled = false;

        }

        /// <summary>
        /// 关闭相机时，界面变化
        /// </summary>
        private void SetCtrlWhenClose()
        {
            bnOpen.Enabled = true;

            bnClose.Enabled = false;
            bnStartGrab.Enabled = false;
            bnStopGrab.Enabled = false;
            bnContinuesMode.Enabled = false;
            bnTriggerMode.Enabled = false;
            cbSoftTrigger.Enabled = false;
            bnTriggerExec.Enabled = false;

        }

        private void bnClose_Click(object sender, EventArgs e)
        {
            if (m_bGrabbing)
            {
                m_bGrabbing = false;
                // ch:停止抓图 || en:Stop grab image
                m_pMyCamera.MV_CC_StopGrabbing_NET();

                // ch: 控件操作 || en: Control operation
                SetCtrlWhenStopGrab();
            }

            // ch:关闭设备 || en: Close device
            m_pMyCamera.MV_CC_CloseDevice_NET();

            // ch: 控件操作 || en: Control operation
            SetCtrlWhenClose();

            m_bGrabbing = false;
        }

        private void bnContinuesMode_CheckedChanged(object sender, EventArgs e)
        {
            int nRet = MyCamera.MV_OK;
            if (bnContinuesMode.Checked)
            {
                nRet = m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", 0);
                if (nRet != MyCamera.MV_OK)
                {
                    MessageBox.Show("Set TriggerMode Fail");
                    return;
                }
                cbSoftTrigger.Enabled = false;
                bnTriggerExec.Enabled = false;
            }
        }

        private void bnTriggerMode_CheckedChanged(object sender, EventArgs e)
        {
            int nRet = MyCamera.MV_OK;
            if (bnTriggerMode.Checked)
            {
                nRet = m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerMode", 1);
                if (nRet != MyCamera.MV_OK)
                {
                    MessageBox.Show("Set TriggerMode Fail");
                    return;
                }

                // ch: 触发源选择:0 - Line0 || en :TriggerMode select;
                //           1 - Line1;
                //           2 - Line2;
                //           3 - Line3;
                //           4 - Counter;
                //           7 - Software;
                if (cbSoftTrigger.Checked)
                {
                    nRet = m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", 7);
                    if (nRet != MyCamera.MV_OK)
                    {
                        MessageBox.Show("Set TriggerSource Fail");
                        return;
                    }
                    if (m_bGrabbing)
                    {
                        bnTriggerExec.Enabled = true;
                    }
                }
                else
                {
                    nRet = m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", 0);
                    if (nRet != MyCamera.MV_OK)
                    {
                        MessageBox.Show("Set TriggerSource Fail");
                        return;
                    }
                }
                cbSoftTrigger.Enabled = true;
            }
        }

        private void SetCtrlWhenStartGrab()
        {

            bnStartGrab.Enabled = false;
            bnStopGrab.Enabled = true;

            if (bnTriggerMode.Checked && cbSoftTrigger.Checked)
            {
                bnTriggerExec.Enabled = true;
            }

        }
        private void SetCtrlWhenStopGrab()
        {
            bnStartGrab.Enabled = true;
            bnStopGrab.Enabled = false;

            bnTriggerExec.Enabled = false;
        }

        /// <summary>
        /// 图像是否为Mono格式
        /// </summary>
        /// <param name="enType"></param>
        /// <returns></returns>
        private bool IsMonoPixelFormat(MyCamera.MvGvspPixelType enType)
        {
            switch (enType)
            {
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono8:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono10:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono10_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono12:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono12_Packed:
                    return true;
                default:
                    return false;
            }
        }

        /// <summary>
        /// 图像是否为彩色
        /// </summary>
        /// <param name="enType"></param>
        /// <returns></returns>
        private bool IsColorPixelFormat(MyCamera.MvGvspPixelType enType)
        {
            switch (enType)
            {
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_RGB8_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BGR8_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_RGBA8_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BGRA8_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_YUV422_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_YUV422_YUYV_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGR8:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerRG8:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGB8:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerBG8:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGB10:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGB10_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerBG10:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerBG10_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerRG10:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerRG10_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGR10:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGR10_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGB12:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGB12_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerBG12:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerBG12_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerRG12:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerRG12_Packed:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGR12:
                case MyCamera.MvGvspPixelType.PixelType_Gvsp_BayerGR12_Packed:
                    return true;
                default:
                    return false;
            }
        }

        /// <summary>
        /// 其他黑白格式转为Mono8
        /// </summary>
        /// <param name="obj"></param>
        /// <param name="pInData">输出图片数据</param>
        /// <param name="pOutData">输出图片数据</param>
        /// <param name="nHeight">高</param>
        /// <param name="nWidth">宽</param>
        /// <param name="nPixelType">像素格式</param>
        /// <returns></returns>
        public Int32 ConvertToMono8(object obj, IntPtr pInData, IntPtr pOutData, ushort nHeight,ushort nWidth, MyCamera.MvGvspPixelType nPixelType)
        {
            if (IntPtr.Zero == pInData || IntPtr.Zero == pOutData)
            {
                return MyCamera.MV_E_PARAMETER;
            }

            int nRet = MyCamera.MV_OK;
            MyCamera device = obj as MyCamera;
            MyCamera.MV_PIXEL_CONVERT_PARAM stPixelConvertParam = new MyCamera.MV_PIXEL_CONVERT_PARAM();

            stPixelConvertParam.pSrcData = pInData;//源数据
            if (IntPtr.Zero == stPixelConvertParam.pSrcData)
            {
                return -1;
            }

            stPixelConvertParam.nWidth = nWidth;//图像宽度
            stPixelConvertParam.nHeight = nHeight;//图像高度
            stPixelConvertParam.enSrcPixelType = nPixelType;//源数据的格式
            stPixelConvertParam.nSrcDataLen = (uint)(nWidth * nHeight * ((((uint)nPixelType) >> 16) & 0x00ff) >> 3);

            stPixelConvertParam.nDstBufferSize = (uint)(nWidth * nHeight * ((((uint)MyCamera.MvGvspPixelType.PixelType_Gvsp_RGB8_Packed) >> 16) & 0x00ff) >> 3);
            stPixelConvertParam.pDstBuffer = pOutData;//转换后的数据
            stPixelConvertParam.enDstPixelType = MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono8;
            stPixelConvertParam.nDstBufferSize = (uint)(nWidth * nHeight * 3);

            nRet = device.MV_CC_ConvertPixelType_NET(ref stPixelConvertParam);//格式转换
            if (MyCamera.MV_OK != nRet)
            {
                return -1;
            }

            return nRet;
        }

        /// <summary>
        /// 其他彩色格式转为RGB8
        /// </summary>
        /// <param name="obj"></param>
        /// <param name="pSrc"></param>
        /// <param name="nHeight"></param>
        /// <param name="nWidth"></param>
        /// <param name="nPixelType"></param>
        /// <param name="pDst"></param>
        /// <returns></returns>
        public Int32 ConvertToRGB(object obj, IntPtr pSrc, ushort nHeight, ushort nWidth, MyCamera.MvGvspPixelType nPixelType, IntPtr pDst)
        {
            if (IntPtr.Zero == pSrc || IntPtr.Zero == pDst)
            {
                return MyCamera.MV_E_PARAMETER;
            }

            int nRet = MyCamera.MV_OK;
            MyCamera device = obj as MyCamera;
            MyCamera.MV_PIXEL_CONVERT_PARAM stPixelConvertParam = new MyCamera.MV_PIXEL_CONVERT_PARAM();

            stPixelConvertParam.pSrcData = pSrc;//源数据
            if (IntPtr.Zero == stPixelConvertParam.pSrcData)
            {
                return -1;
            }

            stPixelConvertParam.nWidth = nWidth;//图像宽度
            stPixelConvertParam.nHeight = nHeight;//图像高度
            stPixelConvertParam.enSrcPixelType = nPixelType;//源数据的格式
            stPixelConvertParam.nSrcDataLen = (uint)(nWidth * nHeight * ((((uint)nPixelType) >> 16) & 0x00ff) >> 3);

            stPixelConvertParam.nDstBufferSize = (uint)(nWidth * nHeight * ((((uint)MyCamera.MvGvspPixelType.PixelType_Gvsp_RGB8_Packed) >> 16) & 0x00ff) >> 3);
            stPixelConvertParam.pDstBuffer = pDst;//转换后的数据
            stPixelConvertParam.enDstPixelType = MyCamera.MvGvspPixelType.PixelType_Gvsp_RGB8_Packed;
            stPixelConvertParam.nDstBufferSize = (uint)nWidth * nHeight * 3;

            nRet = device.MV_CC_ConvertPixelType_NET(ref stPixelConvertParam);//格式转换
            if (MyCamera.MV_OK != nRet)
            {
                return -1;
            }

            return MyCamera.MV_OK;
        }

        /// <summary>
        /// 显示图片
        /// </summary>
        /// <param name="nHeight">高</param>
        /// <param name="nWidth">宽</param>
        /// <param name="pImageBuf">图片数据</param>
        /// <param name="enPixelType">像素格式</param>
        public void VisionProDisplay(UInt32 nHeight, UInt32 nWidth, IntPtr pImageBuf, MyCamera.MvGvspPixelType enPixelType)
        {
            // ch: 显示 || display
            try
            {
                if (enPixelType == MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono8)
                {
                    CogImage8Root cogImage8Root = new CogImage8Root();
                    cogImage8Root.Initialize((Int32)nWidth, (Int32)nHeight, pImageBuf, (Int32)nWidth, null);

                    CogImage8Grey cogImage8Grey = new CogImage8Grey();
                    cogImage8Grey.SetRoot(cogImage8Root);
                    this.cogDisplayImage.Image = cogImage8Grey.ScaleImage((int)nWidth, (int)nHeight);
					System.GC.Collect();
                }
                else
                {
                    CogImage8Root image0 = new CogImage8Root();
                    IntPtr ptr0 = new IntPtr(pImageBuf.ToInt64());
                    image0.Initialize((int)nWidth, (int)nHeight, ptr0, (int)nWidth, null);

                    CogImage8Root image1 = new CogImage8Root();
                    IntPtr ptr1 = new IntPtr(pImageBuf.ToInt64() + m_nRowStep);
                    image1.Initialize((int)nWidth, (int)nHeight, ptr1, (int)nWidth, null);

                    CogImage8Root image2 = new CogImage8Root();
                    IntPtr ptr2 = new IntPtr(pImageBuf.ToInt64() + m_nRowStep * 2);
                    image2.Initialize((int)nWidth, (int)nHeight, ptr2, (int)nWidth, null);

                    CogImage24PlanarColor colorImage = new CogImage24PlanarColor();
                    colorImage.SetRoots(image0, image1, image2);

                    this.cogDisplayImage.Image = colorImage.ScaleImage((int)nWidth, (int)nHeight);
					System.GC.Collect();
                }
            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }
            return;
        }

        /// <summary>
        /// 接收图片线程函数
        /// </summary>
        /// <param name="obj"></param>
        public void ReceiveImageWorkThread(object obj)
        {
            int nRet = MyCamera.MV_OK;
            MyCamera device = obj as MyCamera;
            MyCamera.MV_FRAME_OUT_INFO_EX pFrameInfo = new MyCamera.MV_FRAME_OUT_INFO_EX();
            IntPtr pData = Marshal.AllocHGlobal((int)g_nPayloadSize);
            if (pData == IntPtr.Zero)
            {
                return;
            }
            IntPtr pImageBuffer = Marshal.AllocHGlobal((int)m_nRowStep * 3);
            if (pImageBuffer == IntPtr.Zero)
            {
                return;
            }

            IntPtr pTemp = IntPtr.Zero;
            Byte[] byteArrImageData = new Byte[m_nRowStep * 3];

            while (m_bGrabbing)
            {
                nRet = device.MV_CC_GetOneFrameTimeout_NET(pData, g_nPayloadSize, ref pFrameInfo, 1000);
                if (MyCamera.MV_OK == nRet)
                {
                    MyCamera.MvGvspPixelType pixelType = MyCamera.MvGvspPixelType.PixelType_Gvsp_RGB8_Packed;
                    if (IsColorPixelFormat(pFrameInfo.enPixelType))    // 彩色图像处理
                    {
                        if (pFrameInfo.enPixelType == MyCamera.MvGvspPixelType.PixelType_Gvsp_RGB8_Packed)
                        {
                            pTemp = pData;
                        }
                        else
                        {
                            // 其他格式彩色图像转为RGB
                            nRet = ConvertToRGB(obj, pData, pFrameInfo.nHeight, pFrameInfo.nWidth, pFrameInfo.enPixelType, pImageBuffer);
                            if (MyCamera.MV_OK != nRet)
                            {
                                return;
                            }
                            pTemp = pImageBuffer;
                        }

                        // Packed转Plane
                        unsafe
                        {
                            byte* pBufForSaveImage = (byte*)pTemp;

                            UInt32 nSupWidth = (pFrameInfo.nWidth + (UInt32)3) & 0xfffffffc;

                            for (int nRow = 0; nRow < pFrameInfo.nHeight; nRow++)
                            {
                                for (int col = 0; col < pFrameInfo.nWidth; col++)
                                {
                                    byteArrImageData[nRow * nSupWidth + col] = pBufForSaveImage[nRow * pFrameInfo.nWidth * 3 + (3 * col)];
                                    byteArrImageData[pFrameInfo.nWidth * pFrameInfo.nHeight + nRow * nSupWidth + col] = pBufForSaveImage[nRow * pFrameInfo.nWidth * 3 + (3 * col + 1)];
                                    byteArrImageData[pFrameInfo.nWidth * pFrameInfo.nHeight*2 + nRow * nSupWidth + col] = pBufForSaveImage[nRow * pFrameInfo.nWidth * 3 + (3 * col + 2)];
                                }
                            }
                            pTemp = Marshal.UnsafeAddrOfPinnedArrayElement(byteArrImageData, 0);
                        }
                    }
                    else if (IsMonoPixelFormat(pFrameInfo.enPixelType))    // Mono图像处理
                    {
                        if (pFrameInfo.enPixelType == MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono8)
                        {
                            pTemp = pData;
                        }
                        else
                        {
                            // 其他格式Mono转为Mono8
                            nRet = ConvertToMono8(device, pData, pImageBuffer, pFrameInfo.nHeight, pFrameInfo.nWidth, pFrameInfo.enPixelType);
                            if (MyCamera.MV_OK != nRet)
                            {
                                return;
                            }
                            pTemp = pImageBuffer;
                        }
                        pixelType = MyCamera.MvGvspPixelType.PixelType_Gvsp_Mono8;
                    }
                    else
                    {
                        continue;
                    }
                    VisionProDisplay(pFrameInfo.nHeight, pFrameInfo.nWidth, pTemp, pixelType);
                }
                else
                {
                    continue;
                }
            }
            if (pData != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pData);
            }
            if (pImageBuffer != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pImageBuffer);
            }
            return;
        }

        private void bnStartGrab_Click(object sender, EventArgs e)
        {
            int nRet;
            // ch:开启抓图 | en:start grab
            nRet = m_pMyCamera.MV_CC_StartGrabbing_NET();
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Start Grabbing Fail");
                return;
            }
            m_bGrabbing = true;

            Thread hReceiveImageThreadHandle = new Thread(ReceiveImageWorkThread);
            hReceiveImageThreadHandle.Start(m_pMyCamera);

            // ch: 控件操作 || en: Control operation
            SetCtrlWhenStartGrab();
        }

        private void cbSoftTrigger_CheckedChanged(object sender, EventArgs e)
        {
            if (cbSoftTrigger.Checked)
            {

                // ch: 触发源设为软触发 || en: set trigger mode as Software
                m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", 7);
                if (m_bGrabbing)
                {
                    bnTriggerExec.Enabled = true;
                }
            }
            else
            {
                m_pMyCamera.MV_CC_SetEnumValue_NET("TriggerSource", 0);
                bnTriggerExec.Enabled = false;
            }
        }

        private void bnTriggerExec_Click(object sender, EventArgs e)
        {
            int nRet;

            // ch: 触发命令 || en: Trigger command
            nRet = m_pMyCamera.MV_CC_SetCommandValue_NET("TriggerSoftware");
            if (MyCamera.MV_OK != nRet)
            {
                MessageBox.Show("Trigger Fail");
            }
        }

        private void bnStopGrab_Click(object sender, EventArgs e)
        {
            int nRet = -1;
            // ch:停止抓图 || en:Stop grab image
            nRet = m_pMyCamera.MV_CC_StopGrabbing_NET();
            if (nRet != MyCamera.MV_OK)
            {
                MessageBox.Show("Stop Grabbing Fail");
            }
            m_bGrabbing = false;
            // ch: 控件操作 || en: Control operation
            SetCtrlWhenStopGrab();
        }
    }
}

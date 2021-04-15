//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BasicDemo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

Status __fastcall TForm1::DisplayWindowInitial(void)
{
    m_hwndDisplay = Display->Handle;
    EnableWindowWhenClose();

    return STATUS_OK;
}

Status __fastcall TForm1::EnableWindowWhenClose(void)
{
    Open->Enabled = 0;
    Close->Enabled = 0;
    ContinuesMode->Enabled = 0;
    TriggerMode->Enabled = 0;
    StartGrabbing->Enabled = 0;
    StopGrabbing->Enabled = 0;
    Software->Enabled = 0;
    SoftwareOnce->Enabled = 0;
    SaveBMP->Enabled = 0;
    SaveJPG->Enabled = 0;
    ExpouseEdit->Enabled = 0;
    GainEdit->Enabled = 0;
    GetPara->Enabled = 0;
    SetPara->Enabled = 0;

    return STATUS_OK;
}
Status __fastcall TForm1::EnableWindowWhenOpenNotStart(void)
{
    Open->Enabled = 0;
    Close->Enabled = 1;
    ContinuesMode->Enabled = 1;
    TriggerMode->Enabled = 1;
    StartGrabbing->Enabled = 1;
    Software->Enabled = 1;
    ExpouseEdit->Enabled = 1;
    GainEdit->Enabled = 1;
    GetPara->Enabled = 1;
    SetPara->Enabled = 1;

    return STATUS_OK;
}
Status __fastcall TForm1::EnableWindowWhenStart(void)
{
    StartGrabbing->Enabled = 0;
    StopGrabbing->Enabled = 1;
    SaveBMP->Enabled = 1;
    SaveJPG->Enabled = 1;
    if(1 == Software->Checked && MV_TRIGGER_MODE_ON == m_nTriggerMode)
    {
        SoftwareOnce->Enabled = 1;
    }

    return STATUS_OK;
}

Status __fastcall TForm1::EnableWindowWhenStop(void)
{
    StartGrabbing->Enabled = 1;
    StopGrabbing->Enabled = 0;
    SaveBMP->Enabled = 0;
    SaveJPG->Enabled = 0;
    SoftwareOnce->Enabled = 0;

    return STATUS_OK;
}

Status __fastcall TForm1::EnumDevice()
{
    // ch:����豸�б���е���Ϣ | en:Clear information in device list
    DeviceList->Clear();

    // ch:��ʼ���豸��Ϣ�б� | en:Initial device information list
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:ö�������������豸 | en:Enumerate all device under subnet
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    // ch:��ֵ���뵽��Ϣ�б���в���ʾ���� | en:Add result to information list and display
    unsigned int i;
    int nIp1, nIp2, nIp3, nIp4;
    for (i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            char strUserName[256] = {0};
            char strDisplayName[256] = {0};
            if (strcmp("", (pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
            {
                memcpy(strUserName, pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName,
                    sizeof(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName));
            }
            else
            {
                sprintf(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
            }
            sprintf(strDisplayName, "[%d]GigE:    %s  (%d.%d.%d.%d)",i,
                strUserName, nIp1, nIp2, nIp3, nIp4);
            AnsiString strDeviceName =  strDisplayName;
            DeviceList->AddItem(strDeviceName, NULL);
        }

        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            char strUserName[256] = {0};
            char strDisplayName[256] = {0};
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                memcpy(strUserName, pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName,
                    sizeof(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName));
            }
            else
            {
                sprintf(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
            }

            sprintf(strDisplayName, "[%d]UsbV3:  %s", i, strUserName);
            AnsiString strDeviceName =  strDisplayName;
            DeviceList->AddItem(strDeviceName, NULL);
        }
        else
        {
        }
    }

    if (0 == m_stDevList.nDeviceNum)
    {
        ShowMessage("No device");
        return STATUS_ERROR;
    }

    Open->Enabled = 1;

    return STATUS_OK;
}

Status __fastcall TForm1::OpenDevice(void)
{
    if (TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }

    if(TRUE == m_bCreateDevice)
    {
        return STATUS_ERROR;
    }
    int nIndex = DeviceList->ItemIndex;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowMessage("Please select device");
        return STATUS_ERROR;
    }

    // ch:���豸��Ϣ�����豸ʵ�� | en:Create device instance from device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowMessage("Device does not exist");
        return STATUS_ERROR;
    }

    int nRet = MV_CC_CreateHandle(&m_hHandle, m_stDevList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    nRet = MV_CC_OpenDevice(m_hHandle);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    m_bCreateDevice = TRUE;
    m_bOpenDevice = TRUE;

    return STATUS_OK;
}

// ch:���á���ȡ�������� | en:Get and set parameters
Status __fastcall TForm1::SetTriggerMode(void)
{
    int nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerMode", m_nTriggerMode);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}
Status __fastcall TForm1::GetTriggerMode(void)
{
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = MV_CC_GetEnumValue(m_hHandle, "TriggerMode", &stEnumValue);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    m_nTriggerMode = stEnumValue.nCurValue;
    if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
    {
		TriggerModeClick(NULL);     
    }
    else if (MV_TRIGGER_MODE_OFF == m_nTriggerMode)
    {
        ContinuesModeClick(NULL);
    }
    else
    {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}
Status __fastcall TForm1::GetExposureTime(void)
{
    MVCC_FLOATVALUE stFloatValue = {0};
    int nRet = MV_CC_GetFloatValue(m_hHandle, "ExposureTime", &stFloatValue);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }
    ExpouseEdit->Text = stFloatValue.fCurValue;

    return STATUS_OK;
}
Status __fastcall TForm1::SetExposureTime(void)
{
    float  dfFloatValue = 0.0;

    // ch:�����������ع�ģʽ���������ع�ʱ����Ч | en:Set exposure mode to valid exposure time
    int nRet = MV_CC_SetEnumValue(m_hHandle, "ExposureMode", MV_EXPOSURE_MODE_TIMED);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    nRet = MV_CC_SetEnumValue(m_hHandle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    nRet = MV_CC_SetFloatValue(m_hHandle, "ExposureTime", (float)ExpouseEdit->Text.ToDouble());
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}
Status __fastcall TForm1::GetGain(void)
{
    MVCC_FLOATVALUE stFloatValue = {0};
    int nRet = MV_CC_GetFloatValue(m_hHandle, "Gain", &stFloatValue);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }
    GainEdit->Text = (int)stFloatValue.fCurValue;

    return STATUS_OK;
}
Status __fastcall TForm1::SetGain(void)
{
    int nRet = MV_CC_SetEnumValue(m_hHandle, "GainAuto", 0);
    nRet = MV_CC_SetFloatValue(m_hHandle, "Gain", (float)GainEdit->Text.ToDouble());
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}
Status __fastcall TForm1::GetTriggerSource(void)
{
    MVCC_ENUMVALUE stEnumValue = {0};
    int nRet = MV_CC_GetEnumValue(m_hHandle, "TriggerSource", &stEnumValue);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    if ((unsigned int)MV_TRIGGER_SOURCE_SOFTWARE == stEnumValue.nCurValue)
    {
        Software->Checked = 1;
    }
    else
    {
        Software->Checked = 0;
    }

    return STATUS_OK;
}
Status __fastcall TForm1::SetTriggerSource(void)
{
    if (Software->Checked)
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
        int nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            ShowMessage("Set software trigger fail");
            return STATUS_ERROR;
        }
        SoftwareOnce->Enabled = 1;
    }
    else
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
        int nRet = MV_CC_SetEnumValue(m_hHandle, "TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            ShowMessage("Set hardware trigger fail");
            return STATUS_ERROR;
        }
        SoftwareOnce->Enabled = 0;
    }

    return STATUS_OK;
}
Status __fastcall TForm1::SaveImage(void)
{
    if (FALSE == m_bStartGrabbing)
    {
        return STATUS_ERROR;
    }
	
	int nRet = MV_OK;
	m_bRetStatus = STATUS_OK;
	MVCC_INTVALUE stIntVal = {0};
	// ch:���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
	// en:Request buffer only when first time saving image, release after CloseDevice
    if (NULL == m_pBufForDriver)
    {
        // ch:������л�ȡһ֡ͼ���С | en:Get one frame size from camera
        nRet = MV_CC_GetIntValue(m_hHandle, "PayloadSize", &stIntVal);
        if (nRet != MV_OK)
        {
            ShowMessage("failed in get PayloadSize");
            return STATUS_ERROR;
        }

        m_nBufSizeForDriver = stIntVal.nCurValue;

        m_pBufForDriver = (unsigned char *)malloc(m_nBufSizeForDriver);
        if (NULL == m_pBufForDriver)
        {
            ShowMessage("malloc fail");
            return STATUS_ERROR;
        }
    }
	
    // ch:��ȡ1��ͼ
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    unsigned int nDataSize = MAX_BUF_SIZE;
    unsigned int nImageNum = 1;
    unsigned int nDataLen = 0;

    while(nImageNum)
    {
        nRet = MV_CC_GetOneFrameTimeout(m_hHandle, m_pBufForDriver, m_nBufSizeForDriver, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            nImageNum--;

            // ch:���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
			// en:Request buffer only when first time saving image, release after CloseDevice
            if (NULL == m_pBufForSaveImage)
            {
                // ch:BMPͼƬ��С��width * height * 3 + 2048(Ԥ��BMPͷ��С)
				// en:BMP image size: width * height * 3 + 2048 (Reseved BMP header size)
                m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;

                m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);
                if (NULL == m_pBufForSaveImage)
                {
                    m_bRetStatus = STATUS_ERROR;
                    break;
                }
            }
            // ch:���ö�Ӧ���������
			// en:Set camera parameters
            MV_SAVE_IMAGE_PARAM_EX stParam = {0};
            stParam.enImageType = m_nSaveImageType; // ch:��Ҫ�����ͼ������ | en:Image type to save
            stParam.enPixelType = stImageInfo.enPixelType;  // ch:�����Ӧ�����ظ�ʽ | en:Pixel format
            stParam.nWidth      = stImageInfo.nWidth;         // ch:�����Ӧ�Ŀ� | en:Width
            stParam.nHeight     = stImageInfo.nHeight;          // ch:�����Ӧ�ĸ� | en:Height
            stParam.nDataLen    = stImageInfo.nFrameLen;
            stParam.pData       = m_pBufForDriver;
            stParam.pImageBuffer = m_pBufForSaveImage;
            stParam.nBufferSize = m_nBufSizeForSaveImage;  // ch:�洢�ڵ�Ĵ�С | en:Buffer node size
            stParam.nJpgQuality     = 80; // ch:jpg���룬���ڱ���Jpgͼ��ʱ��Ч������BMPʱSDK�ں��Ըò���
										  // en:jpg encoding, only valid when saving Jpg image. SDK will ignore this parameter when saving BMP

            nRet = MV_CC_SaveImageEx(&stParam);
            if(MV_OK != nRet)
            {
                m_bRetStatus = STATUS_ERROR;
                break;
            }

            char pImageName[32] = {0};
            if (MV_Image_Bmp == stParam.enImageType)
            {
                sprintf(pImageName, "%03d.bmp", stImageInfo.nFrameNum);
            }
            else if (MV_Image_Jpeg == stParam.enImageType)
            {
                sprintf(pImageName, "%03d.jpg", stImageInfo.nFrameNum);
            }
            
            FILE* fp = fopen(pImageName, "wb");
            fwrite(m_pBufForSaveImage, 1, stParam.nImageLen, fp);
            fclose(fp);
        }
        else
        {
            m_bRetStatus = STATUS_ERROR;
            ShowMessage("No data, save image fail");
            break;
        }
    }

    return m_bRetStatus;
}

Status __fastcall TForm1::CloseDevice(void)
{
    int nRet = MV_CC_CloseDevice(m_hHandle);
    nRet = MV_CC_DestroyHandle(m_hHandle);
    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;
    m_bCreateDevice = FALSE;
    if (m_pBufForDriver)
    {
        free(m_pBufForDriver);
        m_pBufForDriver = NULL;
    }
    m_nBufSizeForDriver = 0;
	
	if (m_pBufForSaveImage)
    {
        free(m_pBufForSaveImage);
        m_pBufForSaveImage = NULL;
    }
    m_nBufSizeForSaveImage  = 0;

    return STATUS_OK;
}


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
: TForm(Owner)
{
    // ch:��Ա������ʼ�� | en:Member variable initialization
    m_bCreateDevice = FALSE;
    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;
    m_nTriggerMode = MV_TRIGGER_MODE_OFF;
    m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
    m_nSaveImageType = MV_Image_Undefined;
    m_hwndDisplay = NULL;
    m_pBufForDriver = NULL;
    m_nBufSizeForDriver = 0;
	m_pBufForSaveImage = NULL;         
	m_nBufSizeForSaveImage = 0;
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:��ʼ������ | en:Initialization
    DisplayWindowInitial();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EnumClick(TObject *Sender)
{
    int nRet = EnumDevice();
    if (MV_OK != nRet)
    {
        ShowMessage("EnumDevice failed");
    }
    return;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenClick(TObject *Sender)
{
    int nRet = OpenDevice();
    if (MV_OK != nRet)
    {
        ShowMessage("OpenClick failed");
        return;
    }
    GetParaClick(NULL); // ch:��ȡ���� | en:Get paramters
    EnableWindowWhenOpenNotStart();
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CloseClick(TObject *Sender)
{
    CloseDevice();
    EnableWindowWhenClose();
    Open->Enabled = 1;
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ContinuesModeClick(TObject *Sender)
{
    ContinuesMode->Checked = 1;
    TriggerMode->Checked = 0;
    m_nTriggerMode = MV_TRIGGER_MODE_OFF;
    int nRet = SetTriggerMode();
    if (STATUS_OK != nRet)
    {
        return;
    }
    SoftwareOnce->Enabled = 0;
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TriggerModeClick(TObject *Sender)
{
    ContinuesMode->Checked = 0;
    TriggerMode->Checked = 1;
    m_nTriggerMode = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode();
    if (STATUS_OK != nRet)
    {
        ShowMessage("Set trigger mode fail");
        return;
    }

    if (m_bStartGrabbing == TRUE)
    {
        if (TRUE == Software->Checked )
        {
            SoftwareOnce->Enabled = 1;
        }
    }
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StartGrabbingClick(TObject *Sender)
{
    if (FALSE == m_bCreateDevice || FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
    nRet = MV_CC_StartGrabbing(m_hHandle);
    if (MV_OK == nRet)
    {
        nRet = MV_CC_Display(m_hHandle, m_hwndDisplay);
        if (MV_OK != nRet)
        {
            return;
        }
    }
    else
    {
        return ;
    }
    m_bStartGrabbing = TRUE;
    EnableWindowWhenStart();

    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StopGrabbingClick(TObject *Sender)
{
    if (FALSE == m_bCreateDevice || FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
    nRet = MV_CC_StopGrabbing(m_hHandle);
    if (MV_OK != nRet)
    {
        return;
    }
    m_bStartGrabbing = FALSE;
    EnableWindowWhenStop();
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoftwareClick(TObject *Sender)
{
    int nRet = SetTriggerSource();
    if (nRet != STATUS_OK)
    {
        return;
    }

    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoftwareOnceClick(TObject *Sender)
{
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }
    int nRet = MV_CC_SetCommandValue(m_hHandle, "TriggerSoftware");
    if (MV_OK != nRet)
    {
        return;
    }

    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveBMPClick(TObject *Sender)
{
    m_nSaveImageType = MV_Image_Bmp;
    int nRet = SaveImage();
    if (STATUS_OK != nRet)
    {
        return;
    }
    ShowMessage("Save BMP succeed");
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveJPGClick(TObject *Sender)
{
    m_nSaveImageType = MV_Image_Jpeg;
    int nRet = SaveImage();
    if (STATUS_OK != nRet)
    {
        return;
    }
    ShowMessage("Save JPG succeed");
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GetParaClick(TObject *Sender)
{
    int nRet = GetTriggerMode();
    if (nRet != STATUS_OK)
    {
        ShowMessage("Get trigger mode fail");
        return;
    }

    nRet = GetExposureTime();
    if (nRet != STATUS_OK)
    {
        ShowMessage("Get exposure time fail");
        return;
    }

    nRet = GetGain();
    if (nRet != STATUS_OK)
    {
        ShowMessage("Get gain fail");
        return;
    }

    nRet = GetTriggerSource();
    if (nRet != STATUS_OK)
    {
        ShowMessage("Get trigger source fail");
        return;
    }
    return;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetParaClick(TObject *Sender)
{
    bool bIsSetSucceed = true;
    int nRet = SetExposureTime();
    if (nRet != STATUS_OK)
    {
        ShowMessage("Set exposure time fail");
        bIsSetSucceed = false;
    }
    nRet = SetGain();
    if (nRet != STATUS_OK)
    {
        ShowMessage("Set gain fail");
        bIsSetSucceed = false;
    }
    if (true == bIsSetSucceed)
    {
        ShowMessage("Set parameters succeed");
    }

    return;
}
//---------------------------------------------------------------------------


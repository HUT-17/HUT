
//

#include "stdafx.h"
#include "Raw2Himage_C.h"
#include "Raw2Himage_CDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CRaw2Himage_CDlg::CRaw2Himage_CDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRaw2Himage_CDlg::IDD, pParent)
, m_pcMyCamera(NULL)
, m_nDeviceCombo(0)
, m_bRetStatus(STATUS_ERROR)                       
, m_bCreateDevice(FALSE)
, m_bOpenDevice(FALSE)
, m_bStartGrabbing(FALSE)
, m_nTriggerMode(MV_TRIGGER_MODE_OFF)
, m_bSoftWareTriggerCheck(FALSE)
, m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
, m_pImageBuffer(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRaw2Himage_CDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_OPEN_BUTTON, m_ctrlOpenButton);
    DDX_Control(pDX, IDC_CLOSE_BUTTON, m_ctrlCloseButton);
    DDX_Control(pDX, IDC_START_GRABBING_BUTTON, m_ctrlStartGrabbingButton);
    DDX_Control(pDX, IDC_STOP_GRABBING_BUTTON, m_ctrlStopGrabbingButton);
    DDX_Control(pDX, IDC_SOFTWARE_TRIGGER_CHECK, m_ctrlSoftwareTriggerCheck);
    DDX_Control(pDX, IDC_SOFTWARE_ONCE_BUTTON, m_ctrlSoftwareOnceButton);
    DDX_Control(pDX, IDC_CONTINUS_MODE_RADIO, m_ctrlContinusModeRadio);
    DDX_Control(pDX, IDC_TRIGGER_MODE_RADIO, m_ctrlTriggerModeRadio);
    DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
    DDX_Check(pDX, IDC_SOFTWARE_TRIGGER_CHECK, m_bSoftWareTriggerCheck);
}

BEGIN_MESSAGE_MAP(CRaw2Himage_CDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ENUM_BUTTON, &CRaw2Himage_CDlg::OnBnClickedEnumButton)
    ON_BN_CLICKED(IDC_OPEN_BUTTON, &CRaw2Himage_CDlg::OnBnClickedOpenButton)
    ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CRaw2Himage_CDlg::OnBnClickedCloseButton)
    ON_BN_CLICKED(IDC_CONTINUS_MODE_RADIO, &CRaw2Himage_CDlg::OnBnClickedContinusModeRadio)
    ON_BN_CLICKED(IDC_TRIGGER_MODE_RADIO, &CRaw2Himage_CDlg::OnBnClickedTriggerModeRadio)
    ON_BN_CLICKED(IDC_START_GRABBING_BUTTON, &CRaw2Himage_CDlg::OnBnClickedStartGrabbingButton)
    ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, &CRaw2Himage_CDlg::OnBnClickedStopGrabbingButton)
    ON_BN_CLICKED(IDC_SOFTWARE_TRIGGER_CHECK, &CRaw2Himage_CDlg::OnBnClickedSoftwareTriggerCheck)
    ON_BN_CLICKED(IDC_SOFTWARE_ONCE_BUTTON, &CRaw2Himage_CDlg::OnBnClickedSoftwareOnceButton)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRaw2Himage_CDlg message handlers

BOOL CRaw2Himage_CDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    DisplayWindowInitial();     // ch:显示框初始化 | en:Display Window Initialization
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRaw2Himage_CDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRaw2Himage_CDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

       // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRaw2Himage_CDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

Status CRaw2Himage_CDlg::EnableWindowWhenClose(void)
{
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(FALSE);
    m_ctrlStartGrabbingButton.EnableWindow(FALSE);
    m_ctrlStopGrabbingButton.EnableWindow(FALSE);
    m_ctrlSoftwareTriggerCheck.EnableWindow(FALSE);
    m_ctrlSoftwareOnceButton.EnableWindow(FALSE);
    m_ctrlContinusModeRadio.EnableWindow(FALSE);
    m_ctrlTriggerModeRadio.EnableWindow(FALSE);
    return STATUS_OK;
}

// ch: 打开设备但不开始抓图 || en:The device is opened but it does not grab image
Status CRaw2Himage_CDlg::EnableWindowWhenOpenNotStart(void)
{
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(TRUE);
    m_ctrlStartGrabbingButton.EnableWindow(TRUE);
    m_ctrlSoftwareTriggerCheck.EnableWindow(TRUE);
    m_ctrlContinusModeRadio.EnableWindow(TRUE);
    m_ctrlTriggerModeRadio.EnableWindow(TRUE);
    return STATUS_OK;
}

// ch: 按下开始采集按钮时的按钮颜色 || en:color of button when it starts grabbing image
Status CRaw2Himage_CDlg::EnableWindowWhenStart(void)
{
    m_ctrlStopGrabbingButton.EnableWindow(TRUE);
    m_ctrlStartGrabbingButton.EnableWindow(FALSE);
    if (TRUE == m_bSoftWareTriggerCheck && MV_TRIGGER_MODE_ON == m_nTriggerMode)
    {
        m_ctrlSoftwareOnceButton.EnableWindow(TRUE);
    }
    return STATUS_OK;
}

// ch: 按下结束采集时的按钮颜色 || en: color of button when it stops grabbing image
Status CRaw2Himage_CDlg::EnableWindowWhenStop(void)
{
    m_ctrlStopGrabbingButton.EnableWindow(FALSE);
    m_ctrlStartGrabbingButton.EnableWindow(TRUE);
    m_ctrlSoftwareOnceButton.EnableWindow(FALSE);
    return STATUS_OK;
}

// ch: 最开始时的窗口初始化 || en: initialization of window in the beginning
Status CRaw2Himage_CDlg::DisplayWindowInitial(void)
{
    // ch: 定义显示的起点和长宽高 || en:defined display starting point, height, width, and length
    HTuple hWindowRow, hWindowColumn, hWindowWidth, hWindowHeight;  
    // ch: 定义窗口ID || en:Define window ID 
    HTuple hWindowID;

    // ch; 定义一个CRect类的对象，用于获取图像框的尺寸和位置 || en:define an object of CRect class to measure the size and position of image window
    CRect mRect;  
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_DISPLAY_STATIC);  

    CDC *pDC = pWnd->GetDC();
    pWnd->GetWindowRect(&mRect);

    // ch: 设置显示窗口的起点和宽高 || en:set the starting point, width, and height of display window 
    hWindowRow = 0;  
    hWindowColumn = 0;  
    hWindowWidth = mRect.Width();  
    hWindowHeight = mRect.Height();
    try  
    {   
        hWindowID = (Hlong)pWnd->m_hWnd;
        open_window(hWindowRow, hWindowColumn, hWindowWidth, hWindowHeight, hWindowID, "visible", "", &m_hWindowID);  
    }  
    catch (HException& except)  
    {  
        TRACE(except.message);  
    }

    EnableWindowWhenClose();
    return STATUS_OK;
}

// ch: 打开设备 || en:open device
Status CRaw2Himage_CDlg::OpenDevice(void)
{
    if (TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }
    UpdateData(TRUE);
    if(TRUE == m_bCreateDevice)
    {
        return STATUS_ERROR;
    }
    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        MessageBox(TEXT("Please select device"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        return STATUS_ERROR;
    }

    // ch: 由设备信息创建设备实例 || en:get and create example from the device information list
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        MessageBox(TEXT("Device does not exist"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        return STATUS_ERROR;
    }

    if (NULL != m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    m_pcMyCamera = new CMvCamera;
    // ch: 创建设备失败 || en:fail to create device
    if (NULL == m_pcMyCamera)
    {
        m_bRetStatus = STATUS_ERROR;
        return STATUS_ERROR;
    }

    int nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[nIndex]);
    // ch: 创建设备失败 || en:fail to create device
    if (MV_OK != nRet)
    {
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        m_bRetStatus = STATUS_ERROR;
        return STATUS_ERROR;
    }

    m_bCreateDevice = TRUE;

    m_bRetStatus = STATUS_OK;

    m_bOpenDevice = TRUE;
    return STATUS_OK;
}

// ch: 关闭设备 || en: close device
Status CRaw2Himage_CDlg::CloseDevice(void)
{   
    int nRet = MV_OK;
    if (FALSE == m_bCreateDevice || FALSE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }
    m_bRetStatus = STATUS_OK;
    m_bStartGrabbing = FALSE;

    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
    }

    m_bOpenDevice = FALSE;

    return STATUS_OK;
}

// ch: 销毁设备 || en: destroy device
Status CRaw2Himage_CDlg::DestroyHandle(void)
{
    m_bRetStatus = STATUS_OK;
    int nRet = MV_OK;

    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    if (m_pImageBuffer)
    {
        free(m_pImageBuffer);
        m_pImageBuffer = NULL;
    }
    m_bCreateDevice = FALSE;
    m_bOpenDevice = FALSE;
    m_bStartGrabbing = FALSE;
    return STATUS_OK;
}

// ch: 获取触发模式 || en: get trigger mode
Status CRaw2Himage_CDlg::GetTriggerMode(void)
{
    CString strFeature;
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = m_pcMyCamera->GetEnumValue("TriggerMode", &stEnumValue);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    m_nTriggerMode = stEnumValue.nCurValue;
    if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
    {
        OnBnClickedTriggerModeRadio();
    }
    else if (MV_TRIGGER_MODE_OFF == m_nTriggerMode)
    {
        OnBnClickedContinusModeRadio();
    }
    else
    {
        return STATUS_ERROR;
    }
    UpdateData(FALSE);

    return STATUS_OK;
}

// ch: 设置触发模式 || en: set trigger mode
Status CRaw2Himage_CDlg::SetTriggerMode(void)
{
    int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

// ch: 获取触发源 || en: get trigger source
Status CRaw2Himage_CDlg::GetTriggerSource(void)
{
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = m_pcMyCamera->GetEnumValue("TriggerSource", &stEnumValue);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    if ((unsigned int)MV_TRIGGER_SOURCE_SOFTWARE == stEnumValue.nCurValue)
    {
        m_bSoftWareTriggerCheck = TRUE;
    }
    else
    {
        m_bSoftWareTriggerCheck = FALSE;
    }
    UpdateData(FALSE);

    return STATUS_OK;
}

// ch: 设置触发源 || en: set trigger source
Status CRaw2Himage_CDlg::SetTriggerSource(void)
{
    UpdateData(TRUE);

    if (m_bSoftWareTriggerCheck)
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
        int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            MessageBox(TEXT("Set Software Trigger Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
            return STATUS_ERROR;
        }
        m_ctrlSoftwareOnceButton.EnableWindow(TRUE);
    }
    else
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
        int nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            MessageBox(TEXT("Set Hardware Trigger Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
            return STATUS_ERROR;
        }
        m_ctrlSoftwareOnceButton.EnableWindow(FALSE);
    }

    return STATUS_OK;
}

// ch: 按下查找设备按钮:枚举 || en:click the look for device button: enumeration
void CRaw2Himage_CDlg::OnBnClickedEnumButton()
{
    m_bRetStatus = STATUS_OK;
    CString strMsg;

    // ch: 1. 清除设备列表框中的信息 || en: 1. clear the information in the device list
    m_ctrlDeviceCombo.ResetContent();

    // ch: 2. 初始化设备信息列表 || en: 2. initialize information list of device
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:3. 枚举子网内所有设备 || en: 3. enumerate all of devices in the subnetwork
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        // ch: 枚举设备失败 || en: fail to enumerate devices
        m_bRetStatus = STATUS_ERROR;
        return;
    }

    // ch: 将值加入到信息列表框中并显示出来 || en: add value to the information list and display it
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

            wchar_t* pUserName = NULL;
            if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {

                char strUserName[256];
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, 
                pUserName, nIp1, nIp2, nIp3, nIp4);
            if (NULL != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }

        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            wchar_t* pUserName = NULL;

            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256] = {0};
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
            if (NULL != pUserName)
            {
                delete(pUserName);
                pUserName = NULL;
            }

        }
        else
        {
        }
        m_ctrlDeviceCombo.AddString(strMsg);
    }
    m_ctrlDeviceCombo.SetCurSel(CB_ERR);

    // ch: 枚举到设备之后要显示出来 || en: display the device after enumeration
    UpdateData(FALSE);

    if (0 == m_stDevList.nDeviceNum)
    {
        MessageBox(TEXT("No device"));
        return;
    }
    // ch: 将打开设备的按钮显现出来 || en: show the button for opening device
    m_ctrlOpenButton.EnableWindow(TRUE);
    return;
}

// ch: 按下打开设备按钮：打开设备 || en: press open device button: open device
void CRaw2Himage_CDlg::OnBnClickedOpenButton()
{
    int nRet = OpenDevice();
    if (STATUS_ERROR == nRet)
    {
        MessageBox(TEXT("Open Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        return;
    }

    OnBnClickedGetParameterButton(); // ch: 获取参数 || en: get parameter
    EnableWindowWhenOpenNotStart();
    return;
}

// ch: 按下关闭设备按钮：关闭设备 || en: press button for closing devices: close device
void CRaw2Himage_CDlg::OnBnClickedCloseButton()
{
    CloseDevice();
    DestroyHandle();

    EnableWindowWhenClose();
    m_ctrlOpenButton.EnableWindow(TRUE);
}

// ch: 按下连续模式按钮 || en: press button for trigger mode
void CRaw2Himage_CDlg::OnBnClickedContinusModeRadio()
{
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(FALSE);
    m_nTriggerMode = MV_TRIGGER_MODE_OFF;
    int nRet = SetTriggerMode();
    if (STATUS_OK != nRet)
    {
        return;
    }
    m_ctrlSoftwareOnceButton.EnableWindow(FALSE);

    return;
}

// ch: 按下触发模式按钮 || en: press button to start grabbing
void CRaw2Himage_CDlg::OnBnClickedTriggerModeRadio()
{
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(FALSE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(TRUE);
    m_nTriggerMode = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode();
    if (STATUS_OK != nRet)
    {
        MessageBox(TEXT("Set Trigger Mode Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        return;
    }


    if (m_bStartGrabbing == TRUE)
    {
        UpdateData(TRUE);
        if (TRUE == m_bSoftWareTriggerCheck)
        {
            m_ctrlSoftwareOnceButton.EnableWindow(TRUE);
        }
    }

    return;
}

unsigned int CRaw2Himage_CDlg::WorkThread(void* pUser)
{
    int nRet = MV_OK;
    int nPayloadSize = 0;

    CRaw2Himage_CDlg *pBasicDemo = (CRaw2Himage_CDlg*)pUser;

    // ch:获取数据包大小 | en:Get payload size
    MVCC_INTVALUE_EX stParam = {0};

    nRet = pBasicDemo->m_pcMyCamera->GetIntValue("PayloadSize", &stParam);
    if (MV_OK != nRet)
    {
        return -1;
    }
    nPayloadSize = (int)stParam.nCurValue;

    unsigned char * pData = (unsigned char *)malloc(sizeof(unsigned char) * nPayloadSize);
    if (NULL == pData)
    {
        return -1;
    }

    Halcon::Hobject Hobj;
    unsigned char* pTemp = NULL;
    unsigned char *pDataSeparate = NULL;
    unsigned int nDataSize = nPayloadSize;
    HTuple * hWindow= (HTuple *)(&pBasicDemo->m_hWindowID);

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};

    while(pBasicDemo->m_bStartGrabbing)
    {
        nRet = pBasicDemo->m_pcMyCamera->GetOneFrameTimeout(pData, nDataSize, &stImageInfo, 1000);
        if(MV_OK == nRet)
        {
            if(NULL == pBasicDemo->m_pImageBuffer)//Allocates the cache needed to transform the image，Mono8 and RGB are no need to allocate memory
            {
                if(PixelType_Gvsp_RGB8_Packed != stImageInfo.enPixelType && PixelType_Gvsp_Mono8 != stImageInfo.enPixelType)
                {
                    pBasicDemo->m_pImageBuffer = (unsigned char *)malloc(sizeof(unsigned char) * stImageInfo.nWidth * stImageInfo.nHeight * 3);
                }
            }

            if (IsBayer8PixelFormat(stImageInfo.enPixelType))
            {
                nRet = ConvertBayer8ToHalcon(&Hobj, stImageInfo.nHeight, stImageInfo.nWidth, stImageInfo.enPixelType, pData);
                if (MV_OK != nRet)
                {
                    return -1;
                }
            }
            else if(IsColorPixelFormat(stImageInfo.enPixelType))//Color
            {
                if(NULL == pDataSeparate)
                {
                    pDataSeparate = (unsigned char *)malloc(sizeof(unsigned char) * stImageInfo.nHeight * stImageInfo.nWidth * 3);
                }
                if (PixelType_Gvsp_RGB8_Packed == stImageInfo.enPixelType)
                {
                    pTemp = pData;
                }
                else
                {
                    nRet = ConvertToRGB(pBasicDemo->m_pcMyCamera, pData , stImageInfo.nHeight, stImageInfo.nWidth, stImageInfo.enPixelType, pBasicDemo->m_pImageBuffer);
                    if (MV_OK != nRet)
                    {
                        return -1;
                    }
                    pTemp = pBasicDemo->m_pImageBuffer;
                }

                nRet = ConvertRGBToHalcon(&Hobj, stImageInfo.nHeight, stImageInfo.nWidth, pTemp, pDataSeparate);
                if (MV_OK != nRet)
                {
                    return -1;
                }
            }
            else if(IsMonoPixelFormat(stImageInfo.enPixelType))//Mono
            {
                if(PixelType_Gvsp_Mono8 == stImageInfo.enPixelType)
                {
                    pTemp = pData;
                }
                else
                {
                    nRet = ConvertToMono8(pBasicDemo->m_pcMyCamera, pData, pBasicDemo->m_pImageBuffer, stImageInfo.nHeight, stImageInfo.nWidth, stImageInfo.enPixelType);
                    if (MV_OK != nRet)
                    {
                        return -1;
                    }
                    pTemp = pBasicDemo->m_pImageBuffer;
                }

                nRet = ConvertMono8ToHalcon(&Hobj, stImageInfo.nHeight, stImageInfo.nWidth, pTemp);
                if (MV_OK != nRet)
                {
                    return -1;
                }
            }
            else
            {
                continue;
            }
            HalconDisplay(hWindow, Hobj, stImageInfo.nHeight, stImageInfo.nWidth);
        }
        else
        {
            continue;
        }
    }

    if(pDataSeparate)
    {
        free(pDataSeparate);
        pDataSeparate = NULL;
    }

    if(pBasicDemo->m_pImageBuffer)
    {
        free(pBasicDemo->m_pImageBuffer);
        pBasicDemo->m_pImageBuffer = NULL;
    }

    if(pData)
    {
        free(pData);
        pData = NULL;
    }

    return 0;
}

// ch: 按下开始采集按钮 || press button stop grabbing
void CRaw2Himage_CDlg::OnBnClickedStartGrabbingButton()
{
    if (FALSE == m_bCreateDevice || FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
    if (NULL != m_pcMyCamera)
    {
        nRet = m_pcMyCamera->StartGrabbing();
        if (nRet != MV_OK)
        {
            MessageBox(TEXT("Start Grabbing Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        }
        m_bStartGrabbing = TRUE;

        //begin the thread for grabbing
        unsigned int nThreadID = 0;
        void* hThreadHandle = (void*) _beginthreadex( NULL , 0 , WorkThread , this, 0 , &nThreadID );
        if (NULL == hThreadHandle)
        {
            m_pcMyCamera->Close();
            MessageBox(TEXT("Start Thread Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        }
    }
    else
    {
        m_bRetStatus = STATUS_ERROR;
        return ;
    }

    if (MV_OK != nRet)
    {
        return ;
    }
    EnableWindowWhenStart();
    return;
}

// ch: 按下结束采集按钮 || en: click button to get parameters
void CRaw2Himage_CDlg::OnBnClickedStopGrabbingButton()
{
    if (FALSE == m_bCreateDevice || FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing)
    {
        return;
    }

    int nRet = MV_OK;
    if (NULL != m_pcMyCamera)
    {
        nRet = m_pcMyCamera->StopGrabbing();
    }
    else
    {
        m_bRetStatus = STATUS_ERROR;
        return ;
    }

    if (MV_OK != nRet)
    {
        return ;
    }
    m_bStartGrabbing = FALSE;
    EnableWindowWhenStop();
    return;
}

// ch: 按下获取参数按钮 || en: click button to get parameters
void CRaw2Himage_CDlg::OnBnClickedGetParameterButton()
{
    int nRet = GetTriggerMode();
    if (nRet != STATUS_OK)
    {
        MessageBox(TEXT("Get Trigger Mode Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        return;
    }

    nRet = GetTriggerSource();
    if (nRet != STATUS_OK)
    {
        MessageBox(TEXT("Get Trigger Source Fail"), TEXT("warning"), MB_OK | MB_ICONWARNING);
        return;
    }

    return;
}

// ch: 按下软触发按钮 || en: click software trigger button
void CRaw2Himage_CDlg::OnBnClickedSoftwareTriggerCheck()
{
    int nRet = SetTriggerSource();
    if (nRet != STATUS_OK)
    {
        return;
    }
    return;
}

// ch: 按下软触发一次按钮 || en: click button to software trigger once
void CRaw2Himage_CDlg::OnBnClickedSoftwareOnceButton()
{
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }

    int nRet = m_pcMyCamera->CommandExecute("TriggerSoftware");
    if (MV_OK != nRet)
    {
        return;
    }

    return;
}

// ch: 右上角退出 || en: quit by clicking top right corner
void CRaw2Himage_CDlg::OnClose()
{
    PostQuitMessage(0);
    DestroyHandle();
    CDialog::OnClose();
}

BOOL CRaw2Himage_CDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
    {
        // ch:如果消息是键盘按下事件，且是Esc键，执行以下代码（什么都不做，你可以自己添加想要的代码）
        // en:If the message is a keyboard press event and a Esc key, execute the following code (nothing is done, you can add the code you want)
        return TRUE;
    }
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        // ch:如果消息是键盘按下事件，且是Entert键，执行以下代码（什么都不做，你可以自己添加想要的代码）
        // en:If the message is a keyboard press event and a Esc key, execute the following code (nothing is done, you can add the code you want)
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}

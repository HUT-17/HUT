
// MultipleCameraDlg.cpp : implementation file
#include "stdafx.h"
#include "MultipleCamera.h"
#include "MultipleCameraDlg.h"

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

// CMultipleCameraDlg dialog
CMultipleCameraDlg::CMultipleCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultipleCameraDlg::IDD, pParent)
    , m_nOnlineNumEdit(0)
    , m_nUseNumEdit(0)
    , m_dExpouseEdit(0)
    , m_dGainEdit(0)
    , m_bContinusModeRadio(FALSE)
    , m_bTriggerModeRadio(FALSE)
    , m_bSelectCamera1Check(FALSE)
    , m_bSelectCamera2Check(FALSE)
    , m_bSelectCamera3Check(FALSE)
    , m_bSelectCamera4Check(FALSE)
    , m_nFrameCount1Edit(0)
    , m_nFrameCount2Edit(0)
    , m_nFrameCount3Edit(0)
    , m_nFrameCount4Edit(0)
    , m_nLostFrame1Edit(0)
    , m_nLostFrame2Edit(0)
    , m_nLostFrame3Edit(0)
    , m_nLostFrame4Edit(0)
    // ch:自己定义的变量初始化 | en:User defined variable*
    , m_bCreateDevice(0)                          // 是否创建设备 | en:Create
    , m_bOpenDevice(FALSE)                        // 是否打开 | en:Open
    , m_bStartGrabbing(FALSE)                     // 是否开始抓图 | en:Start grabbing
    , m_nTriggerMode(TRIGGER_NOT_SET)             // 触发模式 | en:Trigger mode
    , m_nSelectDeviceNum(0)                       // 选中相机个数 | en:Selected camera number
    , m_nUsingCameraNum(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));    
    for (int i = 0; i < MAX_DEVICE_NUM; i++)
    {
        m_hwndDisplay[i] = NULL;
        m_cwmdDisplay[i] = NULL;
        m_pcMyCamera[i] = NULL;
        m_hGetOneFrameHandle[i] = NULL;
    }
}

void CMultipleCameraDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ONLINE_NUM_EDIT, m_nOnlineNumEdit);
    DDX_Text(pDX, IDC_USE_NUM_EDIT, m_nUseNumEdit);
    DDX_Radio(pDX, IDC_CONTINUS_MODE_RADIO, m_bContinusModeRadio);
    DDX_Radio(pDX, IDC_TRIGGER_MODE_RADIO, m_bTriggerModeRadio);
    DDX_Check(pDX, IDC_SELECT_CAMERA1_CHECK, m_bSelectCamera1Check);
    DDX_Check(pDX, IDC_SELECT_CAMERA2_CHECK, m_bSelectCamera2Check);
    DDX_Check(pDX, IDC_SELECT_CAMERA3_CHECK, m_bSelectCamera3Check);
    DDX_Check(pDX, IDC_SELECT_CAMERA4_CHECK, m_bSelectCamera4Check);
    DDX_Text(pDX, IDC_FRAME_COUNT1_EDIT, m_nFrameCount1Edit);
    DDX_Text(pDX, IDC_FRAME_COUNT2_EDIT, m_nFrameCount2Edit);
    DDX_Text(pDX, IDC_FRAME_COUNT3_EDIT, m_nFrameCount3Edit);
    DDX_Text(pDX, IDC_FRAME_COUNT4_EDIT1, m_nFrameCount4Edit);
    DDX_Text(pDX, IDC_LOST_FRAME1_EDIT, m_nLostFrame1Edit);
    DDX_Text(pDX, IDC_LOST_FRAME2_EDIT, m_nLostFrame2Edit);
    DDX_Text(pDX, IDC_LOST_FRAME3_EDIT, m_nLostFrame3Edit);
    DDX_Text(pDX, IDC_LOST_FRAME4_EDIT, m_nLostFrame4Edit);
}

BEGIN_MESSAGE_MAP(CMultipleCameraDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// }}AFX_MSG_MAP
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_INIT_DEVICE_BUTTON, &CMultipleCameraDlg::OnBnClickedInitDeviceButton)
    ON_BN_CLICKED(IDC_SET_EXPOUSE_GAIN_BUTTON, &CMultipleCameraDlg::OnBnClickedSetExpouseGainButton)
    ON_BN_CLICKED(IDC_START_GRABBING_BUTTON, &CMultipleCameraDlg::OnBnClickedStartGrabbingButton)
    ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, &CMultipleCameraDlg::OnBnClickedStopGrabbingButton)
    ON_BN_CLICKED(IDC_CONTINUS_MODE_RADIO, &CMultipleCameraDlg::OnBnClickedContinusModeRadio)
    ON_BN_CLICKED(IDC_TRIGGER_MODE_RADIO, &CMultipleCameraDlg::OnBnClickedTriggerModeRadio)
    ON_BN_CLICKED(IDC_SOFTWARE_MODE_BUTTON, &CMultipleCameraDlg::OnBnClickedSoftwareModeButton)
    ON_BN_CLICKED(IDC_HARDWARE_MODE_BUTTON, &CMultipleCameraDlg::OnBnClickedHardwareModeButton)
    ON_BN_CLICKED(IDC_SOFTWARE_ONCE_BUTTON, &CMultipleCameraDlg::OnBnClickedSoftwareOnceButton)
    ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CMultipleCameraDlg::OnBnClickedCloseButton)
    ON_BN_CLICKED(IDC_SELECT_CAMERA1_CHECK, &CMultipleCameraDlg::OnBnClickedSelectCamera1Check)
    ON_BN_CLICKED(IDC_SELECT_CAMERA2_CHECK, &CMultipleCameraDlg::OnBnClickedSelectCamera2Check)
    ON_BN_CLICKED(IDC_SELECT_CAMERA3_CHECK, &CMultipleCameraDlg::OnBnClickedSelectCamera3Check)
    ON_BN_CLICKED(IDC_SELECT_CAMERA4_CHECK, &CMultipleCameraDlg::OnBnClickedSelectCamera4Check)
    ON_BN_CLICKED(IDC_CLEAR_COUNT_BUTTON, &CMultipleCameraDlg::OnBnClickedClearCountButton)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

// ch:工作线程 | en:Working thread
unsigned int    __stdcall   WorkThread(void* pUser)
{
    if (NULL == pUser)
    {
        return -1;
    }

    CMultipleCameraDlg* pCam = (CMultipleCameraDlg*)pUser;
    if (NULL == pCam)
    {
        return -1;
    }
    int nUsingCameraNum = pCam->m_nUsingCameraNum;
    pCam->m_nUsingCameraNum = -1;       
    pCam->ThreadFun(nUsingCameraNum);

    return 0;
}

// CMultipleCameraDlg message handlers
BOOL CMultipleCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	int nRet = DisplayWindowInitial();
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    nRet = HwndHandleInit();
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMultipleCameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CMultipleCameraDlg::OnPaint()
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
HCURSOR CMultipleCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Get window handle  
int CMultipleCameraDlg::HwndHandleInit(void)
{
    m_cwmdDisplay[0] = GetDlgItem(IDC_DISPLAY1_STATIC);
    m_cwmdDisplay[1] = GetDlgItem(IDC_DISPLAY2_STATIC);
    m_cwmdDisplay[2] = GetDlgItem(IDC_DISPLAY3_STATIC);
    m_cwmdDisplay[3] = GetDlgItem(IDC_DISPLAY4_STATIC);
    if (NULL == m_cwmdDisplay[0] || 
        NULL == m_cwmdDisplay[1] || 
        NULL == m_cwmdDisplay[2] ||
        NULL == m_cwmdDisplay[3])
    {
        return STATUS_ERROR;
    }

    for (int i = 0; i < MAX_DEVICE_NUM; i++)
    {
        m_hwndDisplay[i] = m_cwmdDisplay[i]->GetSafeHwnd();
        if (NULL == m_hwndDisplay[i])
        {
            return STATUS_ERROR;
        }
    }
    return MV_OK;
}

// ch:显示框初始化,最开始的初始化 | en:Window initialization
int CMultipleCameraDlg::DisplayWindowInitial(void)
{
    EnableControls(TRUE);

    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }
    m_nOnlineNumEdit = m_stDevList.nDeviceNum;
    m_nUseNumEdit = m_stDevList.nDeviceNum;
    UpdateData(FALSE);
    return nRet;
}

// ch:按钮使能 | en:Enable control
int CMultipleCameraDlg::EnableControls(BOOL bIsCameraReady)
{
    // ch:相机初始化区域 | en:Initialization area
    GetDlgItem(IDC_ONLINE_NUM_EDIT)->EnableWindow(FALSE);                                        // 在线个数 | en:Online number
    GetDlgItem(IDC_EXPOUSE_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);                    // 曝光时间 | en:Exposure time
    GetDlgItem(IDC_GAIN_EDIT)->EnableWindow(m_bOpenDevice ? m_bOpenDevice : FALSE);              // 增益 | en:Gain
    GetDlgItem(IDC_SET_EXPOUSE_GAIN_BUTTON)->EnableWindow(m_bOpenDevice ? m_bOpenDevice : FALSE);// 设置曝光增益按钮 | en:Set exposure and gain button
    GetDlgItem(IDC_USE_NUM_EDIT)->EnableWindow(m_bOpenDevice ? FALSE : TRUE);                    // 选择使用相机个数 | en:Select used camera number

    // ch:采集控制区域 | en:Acquisition control area
    GetDlgItem(IDC_START_GRABBING_BUTTON)->EnableWindow(m_bStartGrabbing ? FALSE : m_bOpenDevice);// 开始采集 | en:Start grabbing
    GetDlgItem(IDC_STOP_GRABBING_BUTTON)->EnableWindow(m_bStartGrabbing ? m_bOpenDevice : FALSE); // 停止采集 | en:Stop grabbing
    GetDlgItem(IDC_SOFTWARE_MODE_BUTTON)->EnableWindow(m_nTriggerMode ? m_bOpenDevice : FALSE);   // 软触发采集 | en:Software trigger
    GetDlgItem(IDC_HARDWARE_MODE_BUTTON)->EnableWindow(m_nTriggerMode ? m_bOpenDevice : FALSE);   // 硬触发采集 | en:Hardware trigger
    GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow(m_nTriggerSource == SOFTWAREMODE && m_nTriggerMode == TRIGGER_ON ? m_bOpenDevice : FALSE);// 软触发一次 | en:Software trigger once
    GetDlgItem(IDC_CLOSE_BUTTON)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);                     // 关闭 | en:Close
    GetDlgItem(IDC_CONTINUS_MODE_RADIO)->EnableWindow(m_bOpenDevice ? m_bOpenDevice : FALSE);     // 连续模式 | en:Continuous mode
    GetDlgItem(IDC_TRIGGER_MODE_RADIO)->EnableWindow(m_bOpenDevice ? m_bOpenDevice : FALSE);      // 触发模式 | en:Trigger mode

    // ch:采集帧数、丢帧和图像显示控制区域 | en:Display control area
    if (!m_bOpenDevice)
    {
        GetDlgItem(IDC_SELECT_CAMERA1_CHECK)->EnableWindow(FALSE);                                // 选中相机 | en:Select camera
        GetDlgItem(IDC_SELECT_CAMERA2_CHECK)->EnableWindow(FALSE);
        GetDlgItem(IDC_SELECT_CAMERA3_CHECK)->EnableWindow(FALSE);
        GetDlgItem(IDC_SELECT_CAMERA4_CHECK)->EnableWindow(FALSE);
        GetDlgItem(IDC_FRAME_COUNT1_EDIT)->EnableWindow(FALSE);                                   // 相机总共采集帧数 | en:Total frames camera acquired
        GetDlgItem(IDC_FRAME_COUNT2_EDIT)->EnableWindow(FALSE);
        GetDlgItem(IDC_FRAME_COUNT3_EDIT)->EnableWindow(FALSE);
        GetDlgItem(IDC_FRAME_COUNT4_EDIT1)->EnableWindow(FALSE);
        GetDlgItem(IDC_LOST_FRAME1_EDIT)->EnableWindow(FALSE);                                    // 相机总共丢帧数 | en:Total frames camera lost
        GetDlgItem(IDC_LOST_FRAME2_EDIT)->EnableWindow(FALSE);
        GetDlgItem(IDC_LOST_FRAME3_EDIT)->EnableWindow(FALSE);
        GetDlgItem(IDC_LOST_FRAME4_EDIT)->EnableWindow(FALSE);
    }
    GetDlgItem(IDC_CLEAR_COUNT_BUTTON)->EnableWindow(m_bStartGrabbing ? FALSE : m_bOpenDevice);   // 计数 | en:Counting

    return MV_OK;
}

// ch:按下初始化按钮时的按钮颜色 | en:Button color after press initialization
int CMultipleCameraDlg::EnableWindowWhenInitCamera(void)
{
    EnableControls(TRUE);

    m_nSelectDeviceNum = m_nUseNumEdit;
    // ch:根据使用相机个数来打亮统计区 | en:Enable statistical area according to number of camera used
    if (m_nUseNumEdit >= 1)
    {
        GetDlgItem(IDC_SELECT_CAMERA1_CHECK)->EnableWindow(TRUE);
        m_bSelectCamera1Check = TRUE;
    }
    if (m_nUseNumEdit >= 2)
    {
        GetDlgItem(IDC_SELECT_CAMERA2_CHECK)->EnableWindow(TRUE);
        m_bSelectCamera2Check = TRUE;
    }
    if (m_nUseNumEdit >= 3)
    {
        GetDlgItem(IDC_SELECT_CAMERA3_CHECK)->EnableWindow(TRUE);
        m_bSelectCamera3Check = TRUE;
    }
    if (m_nUseNumEdit >= 4)
    {
        GetDlgItem(IDC_SELECT_CAMERA4_CHECK)->EnableWindow(TRUE);
        m_bSelectCamera4Check = TRUE;
    }
    UpdateData(FALSE);

    return MV_OK;
}

// ch:显示错误信息 | en:Show error message
void CMultipleCameraDlg::ShowErrorMsg(CString csMessage, int nErrorNum)
{
    CString errorMsg;
    if (nErrorNum == 0)
    {
        errorMsg.Format(_T("%s"), csMessage);
    }
    else
    {
        errorMsg.Format(_T("%s: Error = %x: "), csMessage, nErrorNum);
    }

    switch(nErrorNum)
    {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }

    MessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}

// ch:打开设备 | en:Open Device
int CMultipleCameraDlg::OpenDevice(void)
{
    if (TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }

    // ch:参数检测 | en:Parameter check
    if (m_nUseNumEdit <= 0)
    {
        m_nUseNumEdit = 1;
    }
    if (m_nUseNumEdit > 4)
    {
        m_nUseNumEdit = 4;
    }

    int nCanOpenDeviceNum = 0;
    int nRet = MV_OK;

    for (unsigned int i = 0, j = 0; j < m_stDevList.nDeviceNum; j++, i++)
    {
        if (NULL == m_pcMyCamera[i])
        {
            m_pcMyCamera[i] = new CMvCamera;
        }

        nRet = m_pcMyCamera[i]->Open(m_stDevList.pDeviceInfo[j]);
        if (MV_OK != nRet)
        {
            delete(m_pcMyCamera[i]);
            m_pcMyCamera[i] = NULL;
            i--;
            continue;
        }
        else
        {
            nCanOpenDeviceNum++;
            // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
            if (m_stDevList.pDeviceInfo[j]->nTLayerType == MV_GIGE_DEVICE)
            {
                unsigned int nPacketSize = 0;
                nRet = m_pcMyCamera[i]->GetOptimalPacketSize(&nPacketSize);
                if (nPacketSize > 0)
                {
                    nRet = m_pcMyCamera[i]->SetIntValue("GevSCPSPacketSize",nPacketSize);
                    if(nRet != MV_OK)
                    {
                        ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
                    }
                }
                else
                {
                    ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nPacketSize);
                }
            }
            if (nCanOpenDeviceNum == m_nUseNumEdit)
            {
                break;
            }
        }
    }

    if (0 == nCanOpenDeviceNum)
    {
        ShowErrorMsg(TEXT("No camera can be opened"), 0);
        EnableControls(TRUE);
        m_bCreateDevice = FALSE;
        m_nUseNumEdit = m_stDevList.nDeviceNum;
        UpdateData(FALSE);
        return STATUS_ERROR;
    }
    m_nUseNumEdit = nCanOpenDeviceNum;

    m_bCreateDevice = TRUE;
    m_bOpenDevice = TRUE;

    UpdateData(FALSE);
    return MV_OK;
}

// ch:设置触发模式 | en:Set trigger mode
int CMultipleCameraDlg::SetTriggerMode(void)
{
    if (FALSE == m_bOpenDevice)
    {        
        return STATUS_ERROR;
    }
    int nRet = MV_OK;
    int i = 0;

    for (i = 0; i < m_nUseNumEdit; i++)
    {
        nRet = m_pcMyCamera[i]->SetEnumValue("TriggerMode", m_nTriggerMode);
    }
    
    return MV_OK;
}

// ch:设置曝光时间 | en:Set exposure time
int CMultipleCameraDlg::SetExposureTime(void)
{
    if (FALSE == m_bOpenDevice)
    {        
        return STATUS_ERROR;
    }
    int nRet = MV_OK;
    int i = 0;

    for (i = 0; i < m_nUseNumEdit; i++)
    {
        nRet = m_pcMyCamera[i]->SetEnumValue("ExposureMode", 0);

        nRet = m_pcMyCamera[i]->SetFloatValue("ExposureTime", (float)m_dExpouseEdit);
    }

    return MV_OK;
}

// ch:设置增益 | en:Set gain
int CMultipleCameraDlg::SetGain(void)
{
    if (FALSE == m_bOpenDevice)
    {        
        return STATUS_ERROR;
    }
    int nRet = MV_OK;
    int i = 0;

    for (i = 0; i < m_nUseNumEdit; i++)
    {
        nRet = m_pcMyCamera[i]->SetEnumValue("GainAuto", 0);
        nRet = m_pcMyCamera[i]->SetFloatValue("Gain", (float)m_dGainEdit);
    }
    return MV_OK;
}

// ch:设置触发源 | en:Set trigger source
int CMultipleCameraDlg::SetTriggerSource(void)
{
    if (FALSE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }
    int nRet = MV_OK;
    int i = 0;

    for (i = 0; i < m_nUseNumEdit; i++)
    {
        nRet = m_pcMyCamera[i]->SetEnumValue("TriggerSource", m_nTriggerSource);
    }
    return MV_OK;
}

// ch:软触发一次 | en:Software trigger once
int CMultipleCameraDlg::DoSoftwareOnce(void)
{
    if (FALSE == m_bOpenDevice)
    {        
        return STATUS_ERROR;
    }
    int nRet = MV_OK;
    int i = 0;

    for (i = 0; i < m_nUseNumEdit; i++)
    {
        nRet = m_pcMyCamera[i]->CommandExecute("TriggerSoftware");
    }
    return MV_OK;
}

// ch:开始采集 | en:Start grabbing
int CMultipleCameraDlg::StartGrabbing(void)
{
    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing)
    {        
        return STATUS_ERROR;
    }

    int nRet = MV_OK;

    SetTimer(WM_TIMER_GRAB_INFO,100,NULL);
    for (int i = 0; i < m_nSelectDeviceNum; i++)
    {
        nRet = m_pcMyCamera[i]->StartGrabbing();

        m_nFrameCount1Edit = 0;
        m_nFrameCount2Edit = 0;
        m_nFrameCount3Edit = 0;
        m_nFrameCount4Edit = 0;
        m_nLostFrame1Edit = 0;
        m_nLostFrame2Edit = 0;
        m_nLostFrame3Edit = 0;
        m_nLostFrame4Edit = 0;
        UpdateData(FALSE);

        m_bStartGrabbing = TRUE;

        // ch:开始采集之后才创建workthread线程 | en:Create workthread after start grabbing
        unsigned int nCount = 0;
        while(-1 != m_nUsingCameraNum)
        {
            nCount++;
            if (nCount > 50)
            {
                return STATUS_ERROR;
            }

            Sleep(2);
        }
        unsigned int nThreadID = 0;
        m_nUsingCameraNum = i;
        m_hGetOneFrameHandle[i] = (void*) _beginthreadex( NULL , 0 , WorkThread , this, 0 , &nThreadID );
        if (NULL == m_hGetOneFrameHandle[i])
        {
            ShowErrorMsg(TEXT("Create thread fail"), 0);
        }
    }

    return MV_OK;
}

// ch:结束采集 | en:Stop grabbing
int CMultipleCameraDlg::StopGrabbing(void)
{
    KillTimer(WM_TIMER_GRAB_INFO);

    int nRet = MV_OK;
    if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing)
    {        
        return STATUS_ERROR;
    }

    for (int i = 0; i < m_nSelectDeviceNum; i++)
    {
        nRet = m_pcMyCamera[i]->StopGrabbing();
    }

    m_bStartGrabbing = FALSE;
    for (int i = 0; i < m_nSelectDeviceNum; i++)
    {
        if (m_hGetOneFrameHandle[i])
        {
            WaitForSingleObject(m_hGetOneFrameHandle[i], INFINITE);
            CloseHandle(m_hGetOneFrameHandle[i]);
            m_hGetOneFrameHandle[i] = NULL;
        }
    }
    return nRet;
}

// ch:关闭设备 | en:Close device
int CMultipleCameraDlg::CloseDevice(void)
{
    int nRet = MV_OK;

    for (int i = 0; i < m_nSelectDeviceNum; i++)
    {
        nRet = m_pcMyCamera[i]->Close();
    }
    m_bStartGrabbing = FALSE;
    m_bOpenDevice = FALSE;
    return nRet;
}

void CMultipleCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
    if(nIDEvent == WM_TIMER_GRAB_INFO)
    {
        int nRet = 0;
        CString pos;
        int nPos = 0;
        pos.Format(_T("%d"), nPos);

        unsigned int nLostFrame = 0;
        unsigned int nFrameCount = 0;
        MV_MATCH_INFO_NET_DETECT stMatchInfoNetDetect = {0};
        MV_MATCH_INFO_USB_DETECT stMatchInfoUSBDetect = {0};

        for (int i = 0; i < m_nSelectDeviceNum; i++)
        {
            MV_CC_DEVICE_INFO stDevInfo = {0};
            m_pcMyCamera[i]->GetDeviceInfo(&stDevInfo);

            if (stDevInfo.nTLayerType == MV_GIGE_DEVICE)
            {
                nRet = m_pcMyCamera[i]->GetGevAllMatchInfo(&stMatchInfoNetDetect);
                nLostFrame = stMatchInfoNetDetect.nLostFrameCount;
                nFrameCount = stMatchInfoNetDetect.nNetRecvFrameCount;
            }
            else if (stDevInfo.nTLayerType == MV_USB_DEVICE)
            {
                nRet = m_pcMyCamera[i]->GetU3VAllMatchInfo(&stMatchInfoUSBDetect);
                nLostFrame = stMatchInfoUSBDetect.nErrorFrameCount;
                nFrameCount = stMatchInfoUSBDetect.nReceivedFrameCount;
            }
            else
            {
                return;
            }

            if (MV_OK == nRet)
            {
                switch(i)
                {
                case 0:
                    {
                        m_nFrameCount1Edit = nFrameCount;
                        m_nLostFrame1Edit = nLostFrame;
                    }
                    break;
                case 1:
                    {
                        m_nFrameCount2Edit = nFrameCount;
                        m_nLostFrame2Edit = nLostFrame;
                    }
                    break;
                case 2:
                    {
                        m_nFrameCount3Edit = nFrameCount;
                        m_nLostFrame3Edit = nLostFrame;
                    }
                    break;
                case 3:
                    {
                        m_nFrameCount4Edit = nFrameCount;
                        m_nLostFrame4Edit = nLostFrame;
                    }
                    break;
                default:
                    break;
                }
                UpdateData(FALSE);
            }
        }
    }

    CDialog::OnTimer(nIDEvent);
}

int CMultipleCameraDlg::ThreadFun(int nUsingDeviceNum)
{
    MVCC_INTVALUE_EX stIntValue = {0};

    int nRet = m_pcMyCamera[nUsingDeviceNum]->GetIntValue("PayloadSize", &stIntValue);
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("failed in get PayloadSize"), nRet);
        return nRet;
    }
    unsigned int nDataSize = (unsigned int)stIntValue.nCurValue;

    unsigned char* pGrabBuf = (unsigned char *)malloc(sizeof(unsigned char) * nDataSize);
    if (pGrabBuf == NULL)
    {
        return 0;
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    MV_DISPLAY_FRAME_INFO stDisplayInfo = {0};
    while(m_bStartGrabbing)
    {
        nRet = m_pcMyCamera[nUsingDeviceNum]->GetOneFrameTimeout(pGrabBuf, nDataSize, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            stDisplayInfo.hWnd = m_hwndDisplay[nUsingDeviceNum];
            stDisplayInfo.pData = pGrabBuf;
            stDisplayInfo.nDataLen = stImageInfo.nFrameLen;
            stDisplayInfo.nWidth = stImageInfo.nWidth;
            stDisplayInfo.nHeight = stImageInfo.nHeight;
            stDisplayInfo.enPixelType = stImageInfo.enPixelType;

            m_pcMyCamera[nUsingDeviceNum]->DisplayOneFrame(&stDisplayInfo);
        }
        else
        {
            if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
            {
                Sleep(5);
            }
        }
    }

    free(pGrabBuf);
    pGrabBuf = NULL;

    return MV_OK;
}

// ch:初始化相机，有打开相机操作 | en:Initialzation, include opening device
void CMultipleCameraDlg::OnBnClickedInitDeviceButton()
{
    UpdateData(TRUE);

    int nRet = OpenDevice();
    if (MV_OK != nRet)
    {
        return;
    }

    OnBnClickedContinusModeRadio();
    EnableWindowWhenInitCamera();

    return;
}

// ch:设置曝光和增益 | en:Set exposure and gain
void CMultipleCameraDlg::OnBnClickedSetExpouseGainButton()
{
    UpdateData(TRUE);
    m_dExpouseEdit = GetDlgItemInt(IDC_EXPOUSE_EDIT);
    m_dGainEdit = GetDlgItemInt(IDC_GAIN_EDIT);
    if(m_dExpouseEdit < 0 || m_dGainEdit < 0)
    {
        ShowErrorMsg(TEXT("Set ExposureTime or Gain fail,Because ExposureTime or Gain less than zero"),0);
        return;
    }
    bool bIsSetOK = true;
    int nRet = SetExposureTime();
    if (MV_OK != nRet)
    {
        bIsSetOK = false;
        ShowErrorMsg(TEXT("Set exposure fail"), nRet);
    }
    nRet = SetGain();
    if (MV_OK != nRet)
    {
        bIsSetOK = false;
        ShowErrorMsg(TEXT("Set gain fail"), nRet);
    }
    if (true == bIsSetOK)
    {
        ShowErrorMsg(TEXT("Set exposure and gain succeed"), nRet);
    }

    return;
}

// ch:开始抓图 | en:Start grabbing
void CMultipleCameraDlg::OnBnClickedStartGrabbingButton()
{
    UpdateData(TRUE);
    if (FALSE == m_bSelectCamera1Check &&
        FALSE == m_bSelectCamera2Check &&
        FALSE == m_bSelectCamera3Check &&
        FALSE == m_bSelectCamera4Check)
    {
        ShowErrorMsg(TEXT("Please select camera"), 0);
        return;
    }

    int nRet = StartGrabbing();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Start grabbing fail"), nRet);
    }

    //EnableWindowWhenStart();
    EnableControls(TRUE);
    return;
}

// ch:结束抓图 | en:Stop grabbing
void CMultipleCameraDlg::OnBnClickedStopGrabbingButton()
{
    int nRet = StopGrabbing();

    EnableControls(TRUE);
    //EnableWindowWhenStop();

    return;
}

// ch:设置连续模式 | en:Set continuous mode
void CMultipleCameraDlg::OnBnClickedContinusModeRadio()
{
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(FALSE);
    m_nTriggerMode = TRIGGER_OFF;
    int nRet = SetTriggerMode();
    if (MV_OK != nRet)
    {
        return;
    }
    GetDlgItem(IDC_SOFTWARE_MODE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_HARDWARE_MODE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow(FALSE);

    UpdateData(TRUE);
    UpdateData(FALSE);
    return;
}

// ch:设置触发模式 | en:Set trigger mode
void CMultipleCameraDlg::OnBnClickedTriggerModeRadio()
{
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(FALSE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(TRUE);
    m_nTriggerMode = TRIGGER_ON;
    int nRet = SetTriggerMode();
    if (MV_OK != nRet)
    {
        return;
    }
    GetDlgItem(IDC_SOFTWARE_MODE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_HARDWARE_MODE_BUTTON)->EnableWindow(TRUE);

    UpdateData(TRUE);
    UpdateData(FALSE);
    return;
}

// ch:触发模式为软件触发 | en:Software trigger
void CMultipleCameraDlg::OnBnClickedSoftwareModeButton()
{
    m_nTriggerSource = SOFTWAREMODE;
    // ch:设置为软触发模式 | en:Set trigger mode as software trigger
    int nRet = SetTriggerSource();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set software trigger fail"), nRet);
    }

    EnableControls(TRUE);
    return;
}

// ch:触发模式为硬件触发 | en:Hardware trigger
void CMultipleCameraDlg::OnBnClickedHardwareModeButton()
{
    m_nTriggerSource = HAREWAREMODE;
    // ch:设置为硬触发模式 | en:Set trigger mode as hardware trigger
    int nRet = SetTriggerSource();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set hardware trigger fail"), nRet);
    }

    EnableControls(TRUE);
    return;
}

// ch:软触发一次 | en:Software trigger
void CMultipleCameraDlg::OnBnClickedSoftwareOnceButton()
{
    if (FALSE == m_bStartGrabbing)
    {
    	ShowErrorMsg(TEXT("Please start grabbing first!"), 0);
        return;
    }

    int nRet = DoSoftwareOnce();

    return;
}

// ch:关闭，包含销毁句柄 | en:Close, include destroy handle
void CMultipleCameraDlg::OnBnClickedCloseButton()
{
    KillTimer(WM_TIMER_GRAB_INFO);
    m_bStartGrabbing = FALSE;
    for (int i = 0; i < MAX_DEVICE_NUM; i++)
    {
        if (m_hGetOneFrameHandle[i])
        {
            WaitForSingleObject(m_hGetOneFrameHandle[i], INFINITE);
            CloseHandle(m_hGetOneFrameHandle[i]);
            m_hGetOneFrameHandle[i] = NULL;
        }
    }
    
    CloseDevice();
    EnableControls(TRUE);

    // ch:关闭之后再枚举一遍 | en:Enumerate after close
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        return;
    }
    m_nOnlineNumEdit = m_stDevList.nDeviceNum;
    m_nUseNumEdit = m_stDevList.nDeviceNum;
    UpdateData(FALSE);

    return;
}

// ch:选择相机 | en:Select camera
void CMultipleCameraDlg::OnBnClickedSelectCamera1Check()
{
    UpdateData(TRUE);
    if (FALSE == m_bSelectCamera1Check)
    {
        m_nSelectDeviceNum--;
    }
    if (TRUE == m_bSelectCamera1Check)
    {
        m_nSelectDeviceNum++;
    }
    return;
}

void CMultipleCameraDlg::OnBnClickedSelectCamera2Check()
{
    UpdateData(TRUE);
    if (FALSE == m_bSelectCamera2Check)
    {
        m_nSelectDeviceNum--;
    }
    if (TRUE == m_bSelectCamera2Check)
    {
        m_nSelectDeviceNum++;
    }
    return;
}

void CMultipleCameraDlg::OnBnClickedSelectCamera3Check()
{
    UpdateData(TRUE);
    if (FALSE == m_bSelectCamera3Check)
    {
        m_nSelectDeviceNum--;
    }
    if (TRUE == m_bSelectCamera3Check)
    {
        m_nSelectDeviceNum++;
    }
    return;
}

void CMultipleCameraDlg::OnBnClickedSelectCamera4Check()
{
    UpdateData(TRUE);
    if (FALSE == m_bSelectCamera4Check)
    {
        m_nSelectDeviceNum--;
    }
    if (TRUE == m_bSelectCamera4Check)
    {
        m_nSelectDeviceNum++;
    }
    return;
}

// ch:清零 | en:Zero clear
void CMultipleCameraDlg::OnBnClickedClearCountButton()
{
    UpdateData(TRUE);
    m_nFrameCount1Edit = 0;
    m_nFrameCount2Edit = 0;
    m_nFrameCount3Edit = 0;
    m_nFrameCount4Edit = 0;

    m_nLostFrame1Edit = 0;
    m_nLostFrame2Edit = 0;
    m_nLostFrame3Edit = 0;
    m_nLostFrame4Edit = 0;

    UpdateData(FALSE);
}

void CMultipleCameraDlg::OnClose()
{
    KillTimer(WM_TIMER_GRAB_INFO);
    m_bStartGrabbing = FALSE;
    for (int i = 0; i < MAX_DEVICE_NUM; i++)
    {
        if (m_hGetOneFrameHandle[i])
        {
            WaitForSingleObject(m_hGetOneFrameHandle[i], INFINITE);
            CloseHandle(m_hGetOneFrameHandle[i]);
            m_hGetOneFrameHandle[i] = NULL;
        }
        if (m_pcMyCamera[i])
        {
            delete(m_pcMyCamera[i]);
            m_pcMyCamera[i] = NULL;
        }
    }

    CDialog::OnClose();
}

BOOL CMultipleCameraDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN &&pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}


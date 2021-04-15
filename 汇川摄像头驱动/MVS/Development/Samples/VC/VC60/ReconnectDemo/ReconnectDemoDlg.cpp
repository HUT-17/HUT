// ReconnectDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReconnectDemo.h"
#include "ReconnectDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReconnectDemoDlg dialog

CReconnectDemoDlg::CReconnectDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReconnectDemoDlg::IDD, pParent)
	, m_pcMyCamera(NULL)
    , m_nDeviceCombo(0)
    , m_bOpenDevice(FALSE)
    , m_bStartGrabbing(FALSE)
    , m_hGrabThread(NULL)
    , m_bThreadState(FALSE)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    , m_pGrabBuf(NULL)
    , m_nGrabBufSize(0)
{
	//{{AFX_DATA_INIT(CReconnectDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReconnectDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPEN_BUTTON, m_ctrlOpenButton);
    DDX_Control(pDX, IDC_CLOSE_BUTTON, m_ctrlCloseButton);
    DDX_Control(pDX, IDC_START_GRABBING_BUTTON, m_ctrlStartGrabbingButton);
    DDX_Control(pDX, IDC_STOP_GRABBING_BUTTON, m_ctrlStopGrabbingButton);
    DDX_Control(pDX, IDC_CONTINUS_MODE_RADIO, m_ctrlContinusModeRadio);
    DDX_Control(pDX, IDC_TRIGGER_MODE_RADIO, m_ctrlTriggerModeRadio);
    DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
	//{{AFX_DATA_MAP(CReconnectDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReconnectDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CReconnectDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENUM_BUTTON, OnBnClickedEnumButton)
	ON_BN_CLICKED(IDC_OPEN_BUTTON, OnBnClickedOpenButton)
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnBnClickedCloseButton)
	ON_BN_CLICKED(IDC_CONTINUS_MODE_RADIO, OnBnClickedContinusModeRadio)
	ON_BN_CLICKED(IDC_TRIGGER_MODE_RADIO, OnBnClickedTriggerModeRadio)
	ON_BN_CLICKED(IDC_START_GRABBING_BUTTON, OnBnClickedStartGrabbingButton)
	ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, OnBnClickedStopGrabbingButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ch:取流线程 | en:Grabbing thread
unsigned int __stdcall GrabThread(void* pUser)
{
    if (NULL == pUser)
    {
        return -1;
    }
    
    CReconnectDemoDlg* pCam = (CReconnectDemoDlg*)pUser;
    
    pCam->GrabThreadProcess();
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CReconnectDemoDlg message handlers
BOOL CReconnectDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	DisplayWindowInitial();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReconnectDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CReconnectDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CReconnectDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ch:按下查找设备按钮 | en:Click Find Device button:Enumeration
void CReconnectDemoDlg::OnBnClickedEnumButton() 
{
    CString strMsg;

    // 1. 清除设备列表框中的信息
    m_ctrlDeviceCombo.ResetContent();

    // 2. 初始化设备信息列表
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // 3. 枚举子网内所有设备
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);

    if (MV_OK != nRet)
    {
        return;
    }

    /*将值加入到信息列表框中并显示出来*/
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

			strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, 
                pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName, nIp1, nIp2, nIp3, nIp4);
			m_ctrlDeviceCombo.AddString(strMsg);

        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
			strMsg.Format(_T("[%d]UsbV3:  %s"), i, pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
			m_ctrlDeviceCombo.AddString(strMsg);
        }
        else
        {
        }
        //m_ctrlDeviceCombo.AddString(strMsg);
    }
    m_ctrlDeviceCombo.SetCurSel(CB_ERR);

    /*枚举到设备之后要显示出来*/
    UpdateData(FALSE);
    
    if (0 == m_stDevList.nDeviceNum)
    {
        MessageBox(TEXT("无设备"));
        return;
    }
    /*将打开设备的按钮显现出来*/
    m_ctrlOpenButton.EnableWindow(TRUE);
    return;
}

// ch:按下结束采集按钮 | en:Click Stop button
void CReconnectDemoDlg::OnBnClickedStopGrabbingButton()
{
    if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing || NULL == m_pcMyCamera)
    {
        return;
    }

    m_bThreadState = FALSE;
    if (m_hGrabThread)
    {
        WaitForSingleObject(m_hGrabThread, INFINITE);
        CloseHandle(m_hGrabThread);
        m_hGrabThread = NULL;
    }

    int nRet = m_pcMyCamera->StopGrabbing();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Stop grabbing fail"), nRet);
        return;
    }
    m_bStartGrabbing = FALSE;
    EnableWindowWhenStop();

    return;
}

// ch:按下打开设备按钮：打开设备 | en:Click Open button: Open Device
void CReconnectDemoDlg::OnBnClickedOpenButton() 
{
	int nRet = OpenDevice();
    if (STATUS_ERROR == nRet)
    {
        ShowErrorMsg(TEXT("Open Fail"), nRet);
        return;
    }
	
    GetTriggerMode();
    EnableWindowWhenOpenNotStart();
    return;
}

// ch:按下关闭设备按钮：关闭设备 | en:Click Close button: Close Device
void CReconnectDemoDlg::OnBnClickedCloseButton() 
{
		CloseDevice();
		EnableWindowWhenClose();
        return;
}

// ch:按下连续模式按钮 | en:Click Continues button
void CReconnectDemoDlg::OnBnClickedContinusModeRadio() 
{
	((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(FALSE);
	m_nTriggerMode = MV_TRIGGER_MODE_OFF;
	int nRet = SetTriggerMode();
	if (STATUS_OK != nRet)
	{
		return;
	}
	
    return;	
}

// ch:按下触发模式按钮 | en:Click Trigger Mode button
void CReconnectDemoDlg::OnBnClickedTriggerModeRadio() 
{
    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(FALSE);
    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(TRUE);
    m_nTriggerMode = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode();
    if (STATUS_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set Trigger Mode Fail"), nRet);
        return;
    }
	
    return;	
}

// ch:按下开始采集按钮 | en:Click Start button
void CReconnectDemoDlg::OnBnClickedStartGrabbingButton() 
{
    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing || NULL == m_pcMyCamera)
    {
        return;
    }

    m_bThreadState = TRUE;
    unsigned int nThreadID = 0;
    m_hGrabThread = (void*)_beginthreadex( NULL , 0 , GrabThread , this, 0 , &nThreadID );
    if (NULL == m_hGrabThread)
    {
        m_bThreadState = FALSE;
        ShowErrorMsg(TEXT("Create thread fail"), 0);
        return;
    }

    int nRet = m_pcMyCamera->StartGrabbing();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Start grabbing fail"), nRet);
        return;
    }

    m_bStartGrabbing = TRUE;
    EnableWindowWhenStart();

    return;
}

// ch:初始化时的窗口显示 | en:Window display when Initial
Status CReconnectDemoDlg::EnableWindowInitial(void)
{
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(FALSE);
    m_ctrlStartGrabbingButton.EnableWindow(FALSE);
    m_ctrlStopGrabbingButton.EnableWindow(FALSE);
    m_ctrlContinusModeRadio.EnableWindow(FALSE);
    m_ctrlTriggerModeRadio.EnableWindow(FALSE);
    return STATUS_OK;
}

// ch:关闭设备时的窗口显示 | en:Window display when closing device
Status CReconnectDemoDlg::EnableWindowWhenClose(void)
{
    m_ctrlOpenButton.EnableWindow(TRUE);
    m_ctrlCloseButton.EnableWindow(FALSE);
    m_ctrlStartGrabbingButton.EnableWindow(FALSE);
    m_ctrlStopGrabbingButton.EnableWindow(FALSE);
    m_ctrlContinusModeRadio.EnableWindow(FALSE);
    m_ctrlTriggerModeRadio.EnableWindow(FALSE);
    return STATUS_OK;
}

// ch:打开设备但不开始抓图 | en:Open device but does not start grabbing*/
Status CReconnectDemoDlg::EnableWindowWhenOpenNotStart(void)
{
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(TRUE);
    m_ctrlStartGrabbingButton.EnableWindow(TRUE);
    m_ctrlContinusModeRadio.EnableWindow(TRUE);
    m_ctrlTriggerModeRadio.EnableWindow(TRUE);
    return STATUS_OK;
}

// ch:按下开始采集按钮时的按钮颜色 | en:Button color when the start grabbing button is pressed
Status CReconnectDemoDlg::EnableWindowWhenStart(void)
{
    m_ctrlStopGrabbingButton.EnableWindow(TRUE);
    m_ctrlStartGrabbingButton.EnableWindow(FALSE);

    return STATUS_OK;
}

// ch:按下结束采集时的按钮颜色 | en:Button color when the stop grabbing button is pressed
Status CReconnectDemoDlg::EnableWindowWhenStop(void)
{
    m_ctrlStopGrabbingButton.EnableWindow(FALSE);
    m_ctrlStartGrabbingButton.EnableWindow(TRUE);

    return STATUS_OK;
}

// ch:最开始时的窗口初始化 | en:Initial window initialization
Status CReconnectDemoDlg::DisplayWindowInitial(void)
{
    CWnd *pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    if (NULL == pWnd)
    {
        return STATUS_ERROR;
    }
    m_hwndDisplay = pWnd->GetSafeHwnd();
    if (NULL == m_hwndDisplay)
    {
        return STATUS_ERROR;
    }

    EnableWindowInitial();

    return STATUS_OK;
}

// ch:显示错误信息 | en:Show error message
void CReconnectDemoDlg::ShowErrorMsg(CString csMessage, int nErrorNum)
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
Status CReconnectDemoDlg::OpenDevice(void)
{
    if (TRUE == m_bOpenDevice)
    {
        return STATUS_ERROR;
    }
    UpdateData(TRUE);

    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg(TEXT("Please select devic"), 0);
        return STATUS_ERROR;
    }

    // ch:由设备信息创建设备实例 | en:Device instance created by device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowErrorMsg(TEXT("Device does not exist!"), 0);
        return STATUS_ERROR;
    }

    if (NULL != m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    m_pcMyCamera = new CMvCamera;
    if (NULL == m_pcMyCamera)
    {
        return STATUS_ERROR;
    }

    int nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        return STATUS_ERROR;
    }

	// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
    if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        unsigned int nPacketSize = 0;
        nRet = m_pcMyCamera->GetOptimalPacketSize(&nPacketSize);
        if (nPacketSize > 0)
        {
            nRet = m_pcMyCamera->SetIntValue("GevSCPSPacketSize",nPacketSize);
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

    m_bOpenDevice = TRUE;
    memcpy(&m_stDevInfo, m_stDevList.pDeviceInfo[nIndex], sizeof(MV_CC_DEVICE_INFO));
    m_pcMyCamera->RegisterExceptionCallBack(ReconnectDevice, this);

    return STATUS_OK;
}

// ch:关闭设备 | en:Close Device
Status CReconnectDemoDlg::CloseDevice(void)
{
    m_bThreadState = FALSE;
    if (m_hGrabThread)
    {
        WaitForSingleObject(m_hGrabThread, INFINITE);
        CloseHandle(m_hGrabThread);
        m_hGrabThread = NULL;
    }

    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bStartGrabbing = FALSE;
    m_bOpenDevice = FALSE;

    if (m_pGrabBuf)
    {
        free(m_pGrabBuf);
        m_pGrabBuf = NULL;
    }
    m_nGrabBufSize = 0;

    return STATUS_OK;
}

// ch:获取触发模式 | en:Get Trigger Mode
Status CReconnectDemoDlg::GetTriggerMode(void)
{
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

    return STATUS_OK;
}

// ch:设置触发模式 | en:Set Trigger Mode
Status CReconnectDemoDlg::SetTriggerMode(void)
{
    int nRet = m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
    if (MV_OK != nRet)
    {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

int CReconnectDemoDlg::GrabThreadProcess()
{
    // ch:从相机中获取一帧图像大小 | en:Get size of one frame from camera
    MVCC_INTVALUE_EX stIntValue = {0};
    
    int nRet = m_pcMyCamera->GetIntValue("PayloadSize", &stIntValue);
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("failed in get PayloadSize"), nRet);
        return nRet;
    }
    unsigned int nDataSize = (unsigned int)stIntValue.nCurValue;
    
    if (NULL == m_pGrabBuf || nDataSize > m_nGrabBufSize)
    {
        if (m_pGrabBuf)
        {
            free(m_pGrabBuf);
            m_pGrabBuf = NULL;
        }
        
        m_pGrabBuf = (unsigned char *)malloc(sizeof(unsigned char) * nDataSize);
        if (m_pGrabBuf == NULL)
        {
            return 0;
        }
        m_nGrabBufSize = nDataSize;
    }
    
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    MV_DISPLAY_FRAME_INFO stDisplayInfo = {0};
    
    while(m_bThreadState)
    {
        nRet = m_pcMyCamera->GetOneFrameTimeout(m_pGrabBuf, m_nGrabBufSize, &stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            stDisplayInfo.hWnd = m_hwndDisplay;
            stDisplayInfo.pData = m_pGrabBuf;
            stDisplayInfo.nDataLen = stImageInfo.nFrameLen;
            stDisplayInfo.nWidth = stImageInfo.nWidth;
            stDisplayInfo.nHeight = stImageInfo.nHeight;
            stDisplayInfo.enPixelType = stImageInfo.enPixelType;
            
            m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);
        }
    }
    
    return MV_OK;
}

// ch:右上角退出 | en:Exit from upper right corner
void CReconnectDemoDlg::OnClose()
{
    PostQuitMessage(0);
    CloseDevice();
    CDialog::OnClose();
}

void __stdcall CReconnectDemoDlg::ReconnectDevice(unsigned int nMsgType, void* pUser)
{
	if(nMsgType == MV_EXCEPTION_DEV_DISCONNECT)
    {
		CReconnectDemoDlg* pThis = (CReconnectDemoDlg*)pUser;

		pThis->EnableWindowInitial();
		if (pThis->m_bOpenDevice)
		{
			pThis->m_pcMyCamera->Close();

			BOOL bConnected = FALSE;
			while (1)
			{
                int nRet = pThis->m_pcMyCamera->Open(&pThis->m_stDevInfo);
				if (MV_OK == nRet)
				{
					pThis->m_pcMyCamera->RegisterExceptionCallBack(ReconnectDevice, pUser);
					bConnected = TRUE;
					pThis->EnableWindowWhenOpenNotStart();
					break;
				}
				else
				{
					Sleep(100);
				}
			}

			if (bConnected && pThis->m_bStartGrabbing)
			{
				pThis->m_pcMyCamera->StartGrabbing();
				pThis->EnableWindowWhenStart();
			}
		}
	}
}

BOOL CReconnectDemoDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        return TRUE;
    }
	
    return CDialog::PreTranslateMessage(pMsg);
}



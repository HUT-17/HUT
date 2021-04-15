// ForceIpDemoDlg.cpp : implementation file
#include "stdafx.h"
#include "ForceIpDemo.h"
#include "ForceIpDemoDlg.h"

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
// CForceIpDemoDlg dialog

CForceIpDemoDlg::CForceIpDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CForceIpDemoDlg::IDD, pParent)
	, m_nDeviceCombo(0)
	, m_dwIpaddress(0)
	, m_dwNetworkMask(0)
    , m_dwDefaultGateway(0)
{
	//{{AFX_DATA_INIT(CForceIpDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CForceIpDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
    DDX_Control(pDX, IDC_ENUM_DEVICE_BUTTON, m_ctrlEnumDeviceButton);
    DDX_Control(pDX, IDC_SET_IP_BUTTON, m_ctrlSetIpButton);
}

BEGIN_MESSAGE_MAP(CForceIpDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CForceIpDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENUM_DEVICE_BUTTON, OnEnumDeviceButton)
	ON_BN_CLICKED(IDC_SET_IP_BUTTON, OnSetIpButton)
	ON_CBN_SELCHANGE(IDC_DEVICE_COMBO, OnSelchangeDeviceCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForceIpDemoDlg message handlers

BOOL CForceIpDemoDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CForceIpDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CForceIpDemoDlg::OnPaint() 
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
HCURSOR CForceIpDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


int CForceIpDemoDlg::DisplayDeviceIp()
{
    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg(TEXT("Please select device"), 0);
        return -1;
    }

	char * ipaddr=NULL;
	char addr[20];
	in_addr inaddr;

	inaddr.s_addr = m_stDevList.pDeviceInfo[nIndex]->SpecialInfo.stGigEInfo.nCurrentIp;
	ipaddr= inet_ntoa(inaddr);
	strcpy(addr,ipaddr);  
	m_dwIpaddress = inet_addr(ipaddr);
	CIPAddressCtrl* pIP = (CIPAddressCtrl *) GetDlgItem(IDC_IPADDRESS1);
    pIP->SetAddress(m_dwIpaddress);

	char * defaultGateWay = NULL;
	char chDeftGatWay[20];
	in_addr DefultGateWay;
	DefultGateWay.s_addr = m_stDevList.pDeviceInfo[nIndex]->SpecialInfo.stGigEInfo.nDefultGateWay;
	defaultGateWay = inet_ntoa(DefultGateWay);
	strcpy(chDeftGatWay,defaultGateWay);  
	m_dwDefaultGateway = inet_addr(defaultGateWay);
	CIPAddressCtrl* pDefultGateWay = (CIPAddressCtrl *) GetDlgItem(IDC_IPADDRESS2);
    pDefultGateWay->SetAddress(m_dwDefaultGateway);

	char * subMask = NULL;
	char chSubMask[20];
	in_addr SubNetMask;
	SubNetMask.s_addr = m_stDevList.pDeviceInfo[nIndex]->SpecialInfo.stGigEInfo.nCurrentSubNetMask;
	subMask= inet_ntoa(SubNetMask);
	strcpy(chSubMask,subMask);  
	m_dwNetworkMask = inet_addr(subMask);
	CIPAddressCtrl* pSubNetMask = (CIPAddressCtrl *) GetDlgItem(IDC_IPADDRESS3);
    pSubNetMask->SetAddress(m_dwNetworkMask);

    UpdateData(FALSE);

	return 0;
}

// ch:显示错误信息 | en:Show error message
void CForceIpDemoDlg::ShowErrorMsg(CString csMessage, int nErrorNum)
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
    case MV_E_ACCESS_DENIED:    errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }
	
    MessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}

void CForceIpDemoDlg::OnEnumDeviceButton() 
{
	// TODO: Add your control notification handler code here
	    CString strMsg;

    // ch:清除设备列表框中的信息 | en:clear the information in the device list
    m_ctrlDeviceCombo.ResetContent();

    // ch:初始化设备信息列表 | en:initialize device list
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:枚举子网内所有设备 | en:enumerate all the devices in the subnetworks
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &m_stDevList);

    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("No device"), nRet);
        return;
    }

    // ch:将值加入到信息列表框中并显示出来 | en:add value to the information list and display it
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
    }
    m_ctrlDeviceCombo.SetCurSel(CB_ERR);

    DisplayDeviceIp();

    UpdateData(FALSE);

    if (0 == m_stDevList.nDeviceNum)
    {
        ShowErrorMsg(TEXT("No device"), 0);
        return;
    }
    
    m_ctrlSetIpButton.EnableWindow(TRUE);

    return;
}

void CForceIpDemoDlg::OnSetIpButton() 
{
    // ch:先读取ip框数据 | en:Read ip data first
    UpdateData(TRUE);
	
    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg(TEXT("Please select device"), 0);
        return;
    }
	
    // ch:由设备信息创建设备实例 | en:create example of device from the device list
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowErrorMsg(TEXT("Device does not exist"), 0);
        return;
    }
    
    int nRet = MV_CC_CreateHandle(&m_handle, m_stDevList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Create handle fail"), nRet);
        return;
    }

	BYTE f1, f2, f3, f4;
	CString str;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(f1, f2, f3, f4);
	str.Format(TEXT("%d.%d.%d.%d"), f1, f2, f3, f4);
	m_dwIpaddress = (DWORD)ntohl(inet_addr(str));

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS2))->GetAddress(f1, f2, f3, f4);
	str.Format(TEXT("%d.%d.%d.%d"), f1, f2, f3, f4);
	m_dwDefaultGateway = (DWORD)ntohl(inet_addr(str));

	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS3))->GetAddress(f1, f2, f3, f4);
	str.Format(TEXT("%d.%d.%d.%d"), f1, f2, f3, f4);
	m_dwNetworkMask = (DWORD)ntohl(inet_addr(str));

    nRet = MV_GIGE_ForceIpEx(m_handle, m_dwIpaddress, m_dwNetworkMask, m_dwDefaultGateway);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set forceIp fail"), nRet);
        MV_CC_DestroyHandle(m_handle);
        return;
    }
	
    ShowErrorMsg(TEXT("Set forceIp succeed"), 0);
	
    nRet = MV_CC_DestroyHandle(m_handle);
	
    return;
	
}

BOOL CForceIpDemoDlg::PreTranslateMessage(MSG* pMsg)
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

void CForceIpDemoDlg::OnSelchangeDeviceCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    DisplayDeviceIp();
}

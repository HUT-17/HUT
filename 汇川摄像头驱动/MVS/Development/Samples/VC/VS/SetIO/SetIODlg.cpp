
// SetIODlg.cpp : implementation file
#include "stdafx.h"
#include "SetIO.h"
#include "SetIODlg.h"

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


// CSetIODlg dialog
CSetIODlg::CSetIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIODlg::IDD, pParent)
    , m_nDeviceCombo(0)
    , m_nLineSelectorCombo(0)
    , m_nLinemodeCombo(0)
{
    m_pcMyCamera = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSetIODlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
    DDX_Control(pDX, IDC_OPEN_BUTTON, m_ctrlOpenButton);
    DDX_Control(pDX, IDC_CLOSE_BUTTON, m_ctrlCloseButton);
    DDX_Control(pDX, IDC_GET_LINESEL_BUTTON, m_ctrlGetLineselButton);
    DDX_Control(pDX, IDC_SET_LINESEL_BUTTON, m_ctrlSetLinesetButton);
    DDX_Control(pDX, IDC_GET_LINEMODE_BUTTON, m_ctrlGetLinemodeButton);
    DDX_Control(pDX, IDC_SET_LINEMODE_BUTTON, m_ctrlSetLinemodeButton);
    DDX_Control(pDX, IDC_LINE_SELECTOR_COMBO, m_ctrlLineSelectCombo);
    DDX_Control(pDX, IDC_LINEMODE_COMBO, m_ctrlLinemodeCombo);
    DDX_Control(pDX, IDC_ENUM_BUTTON, m_ctrlEnumButton);
    DDX_CBIndex(pDX, IDC_LINE_SELECTOR_COMBO, m_nLineSelectorCombo);
    DDX_CBIndex(pDX, IDC_LINEMODE_COMBO, m_nLinemodeCombo);
}

BEGIN_MESSAGE_MAP(CSetIODlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ENUM_BUTTON, &CSetIODlg::OnBnClickedEnumButton)
    ON_BN_CLICKED(IDC_OPEN_BUTTON, &CSetIODlg::OnBnClickedOpenButton)
    ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CSetIODlg::OnBnClickedCloseButton)
    ON_BN_CLICKED(IDC_GET_LINESEL_BUTTON, &CSetIODlg::OnBnClickedGetLineselButton)
    ON_BN_CLICKED(IDC_SET_LINESEL_BUTTON, &CSetIODlg::OnBnClickedSetLineselButton)
    ON_BN_CLICKED(IDC_GET_LINEMODE_BUTTON, &CSetIODlg::OnBnClickedGetLinemodeButton)
    ON_BN_CLICKED(IDC_SET_LINEMODE_BUTTON, &CSetIODlg::OnBnClickedSetLinemodeButton)
END_MESSAGE_MAP()

// CSetIODlg message handlers
BOOL CSetIODlg::OnInitDialog()
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

	EnableWindowWhenInitial();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSetIODlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CSetIODlg::OnPaint()
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
HCURSOR CSetIODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ch:��ʾ���ʼ��,�ʼ�ĳ�ʼ�� | en:Window display when closing device
Status CSetIODlg::EnableWindowWhenInitial(void)
{
    m_ctrlEnumButton.EnableWindow(TRUE);
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(FALSE);
    m_ctrlGetLineselButton.EnableWindow(FALSE);
    m_ctrlSetLinesetButton.EnableWindow(FALSE);
    m_ctrlGetLinemodeButton.EnableWindow(FALSE);
    m_ctrlSetLinemodeButton.EnableWindow(FALSE);
    m_ctrlLineSelectCombo.EnableWindow(FALSE);
    m_ctrlLinemodeCombo.EnableWindow(FALSE);
    return STATUS_OK;
}

// ch:�ر�ʱ�İ�ť��ɫ���൱���ʼ��ʼ�� | en:Open device but does not start grabbing
Status CSetIODlg::EnableWindowWhenClose(void)
{
    m_ctrlEnumButton.EnableWindow(TRUE);
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(FALSE);
    m_ctrlGetLineselButton.EnableWindow(FALSE);
    m_ctrlSetLinesetButton.EnableWindow(FALSE);
    m_ctrlGetLinemodeButton.EnableWindow(FALSE);
    m_ctrlSetLinemodeButton.EnableWindow(FALSE);
    m_ctrlLineSelectCombo.EnableWindow(FALSE);
    m_ctrlLinemodeCombo.EnableWindow(FALSE);
    return STATUS_OK;
}
// ch:���´��豸��ťʱ�İ�ť��ɫ | en:Button color when the start grabbing button is pressed
Status CSetIODlg::EnableWindowWhenOpen(void)
{
    m_ctrlOpenButton.EnableWindow(FALSE);
    m_ctrlCloseButton.EnableWindow(TRUE);
    m_ctrlGetLineselButton.EnableWindow(TRUE);
    m_ctrlSetLinesetButton.EnableWindow(TRUE);
    m_ctrlGetLinemodeButton.EnableWindow(TRUE);
    m_ctrlSetLinemodeButton.EnableWindow(TRUE);
    m_ctrlLineSelectCombo.EnableWindow(TRUE);
    m_ctrlLinemodeCombo.EnableWindow(TRUE);
    return STATUS_OK;
}

// ch:��ʾ������Ϣ | en:Show error message
void CSetIODlg::ShowErrorMsg(CString csMessage, int nErrorNum)
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

// ch:���豸 | en:Open Device
Status CSetIODlg::OpenDevice(void)
{
    UpdateData(TRUE);
    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg(TEXT("Please select device"), 0);
        return STATUS_ERROR;
    }

    // ch: ���豸��Ϣ�����豸ʵ�� | en:Device instance created by device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowErrorMsg(TEXT("Device does not exist"), 0);
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
        return nRet;
    }

    return nRet;
}

// ch:��ȡIO�������ѡ�� | en:Get IO Input and Output Select
Status CSetIODlg::GetLineSelector(void)
{
    int nRet = MV_OK;
    CString strMsg;
    MVCC_ENUMVALUE stSelector;
    nRet = m_pcMyCamera->GetEnumValue("LineSelector", &stSelector);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Get LineSelector Fail"), nRet);
        return STATUS_ERROR;
    }

    m_ctrlLineSelectCombo.ResetContent();

    for (unsigned int i = 0; i < stSelector.nSupportedNum; i++ )
    {
        strMsg.Format(_T("Line%d"), stSelector.nSupportValue[i]);
        m_ctrlLineSelectCombo.AddString(strMsg);
    }
    m_nLineSelectorCombo = stSelector.nCurValue;
    UpdateData(FALSE);

    return STATUS_OK;
}

// ch:����IO�������ѡ�� | en:Set IO Input and Output Select
Status CSetIODlg::SetLineSelector(void)
{
    UpdateData(TRUE);
    int nRet = MV_OK;
    if (m_nLineSelectorCombo < 0)
    {
        ShowErrorMsg(TEXT("Please select"), 0);
        return STATUS_ERROR;
    }

    unsigned int nValue = 0;
    CString str;
    ((CComboBox*)GetDlgItem(IDC_LINE_SELECTOR_COMBO))->GetWindowText(str); 
    if (str == CString( _T("Line0")))
    {
        nValue = 0;
    }
    else if (str == CString( _T("Line1")))
    {
        nValue = 1;
    }
    else if (str == CString( _T("Line2")))
    {
        nValue = 2;
    }
    else if (str == CString( _T("Line3")))
    {
        nValue = 3;
    }
    else if (str == CString( _T("Line4")))
    {
        nValue = 4;
    }
    else
    {
        ShowErrorMsg(TEXT("Set LineSelector Fail"), 0);
    }

    nRet = m_pcMyCamera->SetEnumValue("LineSelector", nValue);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set LineSelector Fail"), nRet);
        return STATUS_ERROR;
    }
    ShowErrorMsg(TEXT("Set LineSelector Succeed"), 0);
    return MV_OK;
}

// ch:��ȡIOģʽ | en:Get IO Mode
Status CSetIODlg::GetLineMode(void)
{
    int nRet = MV_OK;
    CString strMsg;
    MVCC_ENUMVALUE stSelector;
    nRet = m_pcMyCamera->GetEnumValue("LineMode", &stSelector);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Get LineMode Fail"), nRet);
        return STATUS_ERROR;
    }

    m_ctrlLinemodeCombo.ResetContent();
    m_nLinemodeCombo = 0;

    for (unsigned int i = 0; i < stSelector.nSupportedNum; i++ )
    {
        if (0 == stSelector.nSupportValue[i])
        {
            strMsg.Format(_T("Input"));
            m_ctrlLinemodeCombo.AddString(strMsg);
        }
        if (1 == stSelector.nSupportValue[i])
        {
            strMsg.Format(_T("Output"));
            m_ctrlLinemodeCombo.AddString(strMsg);
        }
        if (2 == stSelector.nSupportValue[i])
        {
            strMsg.Format(_T("Trigger"));
            m_ctrlLinemodeCombo.AddString(strMsg);
        }
        if (8 == stSelector.nSupportValue[i])
        {
            strMsg.Format(_T("Strobe"));
            m_ctrlLinemodeCombo.AddString(strMsg);
        }

        if (stSelector.nSupportValue[i] == stSelector.nCurValue)
        {
            m_nLinemodeCombo = i;
        }
    }

    UpdateData(FALSE);

    return STATUS_OK;
}   

// ch:����IOģʽ | en:Set IO Mode
Status CSetIODlg::SetLineMode(void)
{
    UpdateData(TRUE);
    int nRet = MV_OK;
    if (m_nLineSelectorCombo < 0)
    {
        ShowErrorMsg(TEXT("Please select"), 0);
        return STATUS_ERROR;
    }

    unsigned int nValue = 0;
    CString str;
    ((CComboBox*)GetDlgItem(IDC_LINEMODE_COMBO))->GetWindowText(str); 
    if (str == CString( _T("Input")))
    {
        nValue = 0;
    }
    else if (str == CString( _T("Output")))
    {
        nValue = 1;
    }
    else if (str == CString( _T("Trigger")))
    {
        nValue = 2;
    }
    else if (str == CString( _T("Strobe")))
    {
        nValue = 8;
    }
    else
    {
        ShowErrorMsg(TEXT("Set LineMode Fail"), 0);
    }
    
    nRet = m_pcMyCamera->SetEnumValue("LineMode", nValue);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set LineMode Fail"), nRet);
        return STATUS_ERROR;
    }
    ShowErrorMsg(TEXT("Set LineMode Succeed"), 0);

    return STATUS_OK;
}

// ch:�ر��豸 | en:Close Device
Status CSetIODlg::CloseDevice(void)
{
    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    return STATUS_OK;
}

void CSetIODlg::OnBnClickedEnumButton()
{
    CString strMsg;

    // ch:����豸�б���е���Ϣ | en:Clear Device List Information
    m_ctrlDeviceCombo.ResetContent();

    // ch:��ʼ���豸��Ϣ�б� | en:Device Information List Initialization
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:ö�������������豸 | en:Enumerate all devices within subnet
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        return;
    }

    for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }

        wchar_t* pUserName = NULL;
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            int nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            int nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            int nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            int nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

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
            strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, pUserName, nIp1, nIp2, nIp3, nIp4);
        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256];
                sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
        }
        m_ctrlDeviceCombo.AddString(strMsg);

        if (NULL != pUserName)
        {
            delete[] pUserName;
            pUserName = NULL;
        }
    }

    m_ctrlDeviceCombo.SetCurSel(0);
    if (0 == m_stDevList.nDeviceNum)
    {
        ShowErrorMsg(TEXT("No device"), 0);
        return;
    }
    m_ctrlOpenButton.EnableWindow(TRUE);
    return;
}

void CSetIODlg::OnBnClickedOpenButton()
{
    int nRet = OpenDevice();
    if (STATUS_OK != nRet)
    {
        ShowErrorMsg(TEXT("Open Fail"), nRet);
        return;
    }

    EnableWindowWhenOpen();
    return;
}

void CSetIODlg::OnBnClickedCloseButton()
{
    CloseDevice();
    EnableWindowWhenInitial();
    m_ctrlOpenButton.EnableWindow(TRUE);
}

void CSetIODlg::OnBnClickedGetLineselButton()
{
    GetLineSelector();
}

void CSetIODlg::OnBnClickedSetLineselButton()
{
    SetLineSelector();
}

void CSetIODlg::OnBnClickedGetLinemodeButton()
{
    GetLineMode();
}

void CSetIODlg::OnBnClickedSetLinemodeButton()
{
    SetLineMode();
}

BOOL CSetIODlg::PreTranslateMessage(MSG* pMsg)
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

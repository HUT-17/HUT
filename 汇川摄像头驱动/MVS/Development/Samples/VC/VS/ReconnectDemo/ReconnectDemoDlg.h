
// BasicDemoDlg.h : header file
#pragma once
#include "afxwin.h" 
#include "MvCamera.h"

// ch:���������붨�� | en:Function return code definition
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

// CBasicDemoDlg dialog
class CBasicDemoDlg : public CDialog
{
// Construction
public:
	CBasicDemoDlg(CWnd* pParent = NULL);	// Standard constructor

// Dialog Data
	enum { IDD = IDD_BasicDemo_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

// ch:�ڲ����� | en:Built-in function
private:
    // ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Window initialization
    Status DisplayWindowInitial(void);          // ch:��ʾ���ʼ��,�ʼ�ĳ�ʼ�� | en:Display window initialization
    void ShowErrorMsg(CString csMessage, int nErrorNum);
    // ch:��ť������ʹ�� | en:Button bright dark enable
    Status EnableWindowInitial(void);
    Status EnableWindowWhenClose(void);         // ch:�ر�ʱ�İ�ť��ɫ���൱���ʼ��ʼ�� | en:Button color when Close, equivalent to the initialization
    Status EnableWindowWhenOpenNotStart(void);  // ch:���´��豸��ťʱ�İ�ť��ɫ | en:Button color when click Open 
    Status EnableWindowWhenStart(void);         // ch:���¿�ʼ�ɼ�ʱ�İ�ť��ɫ | en:Button color when click Start
    Status EnableWindowWhenStop(void);          // ch:���½����ɼ�ʱ�İ�ť��ɫ | en:Button color when click Stop
    // ch:��ʼ��������� | en:Initialization
    Status OpenDevice(void);                    // ch:����� | en:Open Device
    // ch:�ر����ٲ��� | en:Close and deatroy operation
    Status CloseDevice(void);                   // ch:�ر��豸 | en:Close Device
    Status GetTriggerMode(void);
    static void __stdcall ReconnectDevice(unsigned int nMsgType, void* pUser);
    // ch:�ؼ���Ӧ���� | en:Control corresponding variable

private:
    // ch:��ʼ�� | en:Initialization
    CButton m_ctrlOpenButton;                   // ch:���豸 | en:Open device
    CButton m_ctrlCloseButton;                  // ch:�ر��豸 | en:Close device

    // ch:ͼ��ɼ� | en:Image Acquisition
    CButton m_ctrlStartGrabbingButton;          // ch:��ʼץͼ | en:Start grabbing
    CButton m_ctrlStopGrabbingButton;           // ch:����ץͼ | en:Stop grabbing

    // ch:�豸��ʾ������ | en:Device display drop-down box
    CComboBox m_ctrlDeviceCombo;                // ch:ö�ٵ����豸 | en:Enumerated device
    int     m_nDeviceCombo;

private:
    BOOL  m_bOpenDevice;                        // ch:�Ƿ���豸 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
    CMvCamera*      m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:��ʾ��� | en:Display Handle
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    MV_CC_DEVICE_INFO m_stDevInfo;
    unsigned char*  m_pGrabBuf;            // ch:���ڴ�������ȡͼ��Ļ��� | en:Buffer to get image from driver
    unsigned int    m_nGrabBufSize;
    void*           m_hGrabThread;              // ch:ȡ���߳̾�� | en:Grab thread handle
    BOOL            m_bThreadState;

public:
    afx_msg void OnBnClickedEnumButton();               // ch:�����豸 | en:Find Devices
    afx_msg void OnBnClickedOpenButton();               // ch:���豸 | en:Open Devices
    afx_msg void OnBnClickedCloseButton();              // ch:�ر��豸 | en:Close Devices
   
    // ch:ͼ��ɼ� | en:Image Acquisition
    afx_msg void OnBnClickedStartGrabbingButton();      // ch:��ʼ�ɼ� | en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       // ch:�����ɼ� | en:Stop Grabbing
    afx_msg void OnClose();                             // ch:���Ͻ��˳� | en:Exit from upper right corner
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int GrabThreadProcess();
};

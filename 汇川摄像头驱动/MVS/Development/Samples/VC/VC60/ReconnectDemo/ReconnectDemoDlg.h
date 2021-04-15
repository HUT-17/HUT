// ReconnectDemoDlg.h : header file
//

#if !defined(AFX_RECONNECTDEMODLG_H__54DA0D6A_583D_4252_AB8C_4401A23131A4__INCLUDED_)
#define AFX_RECONNECTDEMODLG_H__54DA0D6A_583D_4252_AB8C_4401A23131A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MvCamera.h"
#include <process.h>

// ch:���������붨�� | en:Function return code definition
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1
/////////////////////////////////////////////////////////////////////////////
// CReconnectDemoDlg dialog

class CReconnectDemoDlg : public CDialog
{
// Construction
public:
	CReconnectDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CReconnectDemoDlg)
	enum { IDD = IDD_RECONNECTDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReconnectDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CReconnectDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedEnumButton();               // ch:�����豸 | en:Find Devices
    afx_msg void OnBnClickedOpenButton();               // ch:���豸 | en:Open Devices
    afx_msg void OnBnClickedCloseButton();              // ch:�ر��豸 | en:Close Devices
    afx_msg void OnBnClickedContinusModeRadio();        // ch:����ģʽ | en:Continus Mode
    afx_msg void OnBnClickedTriggerModeRadio();         // ch:����ģʽ | en:Trigger Mode
    afx_msg void OnBnClickedStartGrabbingButton();      // ch:��ʼ�ɼ� | en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       // ch:�����ɼ� | en:Stop Grabbing
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		// ch:�ؼ���Ӧ���� | en:Control corresponding variable
private:
    // ch:��ʼ�� | en:Initialization
    CButton m_ctrlOpenButton;                   // ch:���豸 | en:Open device
    CButton m_ctrlCloseButton;                  // ch:�ر��豸 | en:Close device
	
    // ch:ͼ��ɼ� | en:Image Acquisition
    CButton m_ctrlContinusModeRadio;            // ch:����ģʽ | en:Continus Mode
    CButton m_ctrlTriggerModeRadio;             // ch:����ģʽ | en:Trigger mode
    CButton m_ctrlStartGrabbingButton;          // ch:��ʼץͼ | en:Start grabbing
    CButton m_ctrlStopGrabbingButton;           // ch:����ץͼ | en:Stop grabbing
	
    // ch:�豸��ʾ������ | en:Device display drop-down box
    CComboBox m_ctrlDeviceCombo;                // ch:ö�ٵ����豸 | en:Enumerated device
    int     m_nDeviceCombo;
	
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
	
    // ch:���á���ȡ�������� | en:Set and get parameters operation
    Status SetTriggerMode(void);                // ch:���ô���ģʽ | en:Set Trigger Mode
    Status GetTriggerMode(void);
	
    // ch:�ر����ٲ��� | en:Close and deatroy operation
    Status CloseDevice(void);                   // ch:�ر��豸 | en:Close Device
	
    static void __stdcall ReconnectDevice(unsigned int nMsgType, void* pUser);
	
private:
    BOOL  m_bOpenDevice;                        // ch:�Ƿ���豸 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
    int   m_nTriggerMode;                       // ch:����ģʽ | en:Trigger Mode
	
    CMvCamera*      m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:��ʾ��� | en:Display Handle
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    MV_CC_DEVICE_INFO m_stDevInfo;
	
    unsigned char*  m_pGrabBuf;            // ch:���ڴ�������ȡͼ��Ļ��� | en:Buffer to get image from driver
    unsigned int    m_nGrabBufSize;

    void*           m_hGrabThread;              // ch:ȡ���߳̾�� | en:Grab thread handle
    BOOL            m_bThreadState;	
	
public:    
    afx_msg void OnClose();                             // ch:���Ͻ��˳� | en:Exit from upper right corner
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    int GrabThreadProcess();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECONNECTDEMODLG_H__54DA0D6A_583D_4252_AB8C_4401A23131A4__INCLUDED_)

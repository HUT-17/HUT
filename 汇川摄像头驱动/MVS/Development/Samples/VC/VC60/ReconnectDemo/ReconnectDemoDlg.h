// ReconnectDemoDlg.h : header file
//

#if !defined(AFX_RECONNECTDEMODLG_H__54DA0D6A_583D_4252_AB8C_4401A23131A4__INCLUDED_)
#define AFX_RECONNECTDEMODLG_H__54DA0D6A_583D_4252_AB8C_4401A23131A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MvCamera.h"
#include <process.h>

// ch:函数返回码定义 | en:Function return code definition
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
	afx_msg void OnBnClickedEnumButton();               // ch:查找设备 | en:Find Devices
    afx_msg void OnBnClickedOpenButton();               // ch:打开设备 | en:Open Devices
    afx_msg void OnBnClickedCloseButton();              // ch:关闭设备 | en:Close Devices
    afx_msg void OnBnClickedContinusModeRadio();        // ch:连续模式 | en:Continus Mode
    afx_msg void OnBnClickedTriggerModeRadio();         // ch:触发模式 | en:Trigger Mode
    afx_msg void OnBnClickedStartGrabbingButton();      // ch:开始采集 | en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       // ch:结束采集 | en:Stop Grabbing
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		// ch:控件对应变量 | en:Control corresponding variable
private:
    // ch:初始化 | en:Initialization
    CButton m_ctrlOpenButton;                   // ch:打开设备 | en:Open device
    CButton m_ctrlCloseButton;                  // ch:关闭设备 | en:Close device
	
    // ch:图像采集 | en:Image Acquisition
    CButton m_ctrlContinusModeRadio;            // ch:连续模式 | en:Continus Mode
    CButton m_ctrlTriggerModeRadio;             // ch:触发模式 | en:Trigger mode
    CButton m_ctrlStartGrabbingButton;          // ch:开始抓图 | en:Start grabbing
    CButton m_ctrlStopGrabbingButton;           // ch:结束抓图 | en:Stop grabbing
	
    // ch:设备显示下拉框 | en:Device display drop-down box
    CComboBox m_ctrlDeviceCombo;                // ch:枚举到的设备 | en:Enumerated device
    int     m_nDeviceCombo;
	
	// ch:内部函数 | en:Built-in function
private:
    // ch:最开始时的窗口初始化 | en:Window initialization
    Status DisplayWindowInitial(void);          // ch:显示框初始化,最开始的初始化 | en:Display window initialization
    void ShowErrorMsg(CString csMessage, int nErrorNum);
	
    // ch:按钮的亮暗使能 | en:Button bright dark enable
    Status EnableWindowInitial(void);
    Status EnableWindowWhenClose(void);         // ch:关闭时的按钮颜色，相当于最开始初始化 | en:Button color when Close, equivalent to the initialization
    Status EnableWindowWhenOpenNotStart(void);  // ch:按下打开设备按钮时的按钮颜色 | en:Button color when click Open 
    Status EnableWindowWhenStart(void);         // ch:按下开始采集时的按钮颜色 | en:Button color when click Start
    Status EnableWindowWhenStop(void);          // ch:按下结束采集时的按钮颜色 | en:Button color when click Stop
    
    // ch:初始化相机操作 | en:Initialization
    Status OpenDevice(void);                    // ch:打开相机 | en:Open Device
	
    // ch:设置、获取参数操作 | en:Set and get parameters operation
    Status SetTriggerMode(void);                // ch:设置触发模式 | en:Set Trigger Mode
    Status GetTriggerMode(void);
	
    // ch:关闭销毁操作 | en:Close and deatroy operation
    Status CloseDevice(void);                   // ch:关闭设备 | en:Close Device
	
    static void __stdcall ReconnectDevice(unsigned int nMsgType, void* pUser);
	
private:
    BOOL  m_bOpenDevice;                        // ch:是否打开设备 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
    int   m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
	
    CMvCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    MV_CC_DEVICE_INFO m_stDevInfo;
	
    unsigned char*  m_pGrabBuf;            // ch:用于从驱动获取图像的缓存 | en:Buffer to get image from driver
    unsigned int    m_nGrabBufSize;

    void*           m_hGrabThread;              // ch:取流线程句柄 | en:Grab thread handle
    BOOL            m_bThreadState;	
	
public:    
    afx_msg void OnClose();                             // ch:右上角退出 | en:Exit from upper right corner
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    int GrabThreadProcess();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECONNECTDEMODLG_H__54DA0D6A_583D_4252_AB8C_4401A23131A4__INCLUDED_)

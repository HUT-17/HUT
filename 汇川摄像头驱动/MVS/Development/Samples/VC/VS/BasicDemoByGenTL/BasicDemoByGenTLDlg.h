
// BasicDemoByGenTLDlg.h : header file
#pragma once
#include "afxwin.h" 
#include "MvCamera.h"

#define STATUS_ERROR            -1
#define IMAGE_NAME_LEN          256

// CBasicDemoByGenTLDlg dialog
class CBasicDemoByGenTLDlg : public CDialog
{
// Construction
public:
	CBasicDemoByGenTLDlg(CWnd* pParent = NULL);	// Standard constructor

    // Dialog Data
	enum { IDD = IDD_BasicDemoByGenTL_DIALOG };

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

/*ch:控件对应变量 | en:Control corresponding variable*/
private:
    /*ch:设备显示下拉框 | en:Device display drop-down box*/
    CComboBox m_ctrlInterfaceCombo;                 // ch:枚举到的Interface | en:Enumerated Interface
    CComboBox m_ctrlDeviceCombo;                    // ch:枚举到的设备 | en:Enumerated device
    int       m_nDeviceCombo;

    /*ch:初始化 | en:Initialization*/
    CButton m_ctrlOpenButton;                   // ch:打开设备 | en:Open device
    CButton m_ctrlCloseButton;                  // ch:关闭设备 | en:Close device

    /*ch:图像采集 | en:Image Acquisition*/
    CButton m_ctrlContinusModeRadio;            // ch:连续模式 | en:Continus Mode
    CButton m_ctrlTriggerModeRadio;             // ch:触发模式 | en:Trigger mode
    CButton m_ctrlStartGrabbingButton;          // ch:开始抓图 | en:Start grabbing
    CButton m_ctrlStopGrabbingButton;           // ch:结束抓图 | en:Stop grabbing
    CButton m_ctrlSoftwareTriggerCheck;         // ch:软触发 | en:Software trigger
    BOOL    m_bSoftWareTriggerCheck;
    CButton m_ctrlSoftwareOnceButton;           // ch:软触发一次 | en:Software Trigger Once

/*ch:内部函数 | en:Built-in function*/
private:
    /*ch:最开始时的窗口初始化 | en:Window initialization*/
    int DisplayWindowInitial(void);

    int EnableControls(BOOL bIsCameraReady);
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    /*ch:初始化相机操作 | en:Initialization*/
    int OpenDevice(void);                    // ch:打开相机 | en:Open Device

    /*ch:设置、获取参数操作 | en:Set and get parameters operation*/
    int SetTriggerMode(void);                // ch:设置触发模式 | en:Set Trigger Mode
    int GetTriggerMode(void);
    int GetTriggerSource(void);              // ch:设置触发源 | en:Set Trigger Source
    int SetTriggerSource(void);

    /*ch:关闭销毁操作 | en:Close and deatroy operation*/
    int CloseDevice(void);                   // ch:关闭设备 | en:Close Device

    // ch:去除自定义的像素格式 | en:Remove custom pixel formats
    bool RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);

private:
    /*ch:状态 | en:Status*/
    BOOL  m_bOpenDevice;                        // ch:是否打开设备 | en:Whether to open device
    BOOL  m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
    int   m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
    int   m_nTriggerSource;                     // ch:触发源 | en:Trigger Source

    /*ch:设备相关 | en:Device Related*/
    CMvCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
    HWND  m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
    MV_GENTL_DEV_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
    MV_GENTL_IF_INFO_LIST  m_stIFList;          // ch:Interface结构体列表，存储Interface
                                                //en:Device information list structure variable used to store device list

    void*           m_hGrabThread;              // ch:取流线程句柄 | en:Grab thread handle
    BOOL            m_bThreadState;

public:
    /*ch:枚举 | en:Enumerate*/
    afx_msg void OnBnClickedEnumIfButton();             // ch:枚举Interface | en:Enum Interface
    afx_msg void OnBnClickedEnumDevButton();            // ch:枚举设备 | en:Enum Device

    /*ch:初始化 | en:Initialization*/
    afx_msg void OnBnClickedOpenButton();               // ch:打开设备 | en:Open Devices
    afx_msg void OnBnClickedCloseButton();              // ch:关闭设备 | en:Close Devices
   
    /*ch:图像采集 | en:Image Acquisition*/
    afx_msg void OnBnClickedContinusModeRadio();        // ch:连续模式 | en:Continus Mode
    afx_msg void OnBnClickedTriggerModeRadio();         // ch:触发模式 | en:Trigger Mode
    afx_msg void OnBnClickedStartGrabbingButton();      // ch:开始采集 | en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       // ch:结束采集 | en:Stop Grabbing
    afx_msg void OnBnClickedSoftwareTriggerCheck();     // ch:软触发 | en:Software Trigger
    afx_msg void OnBnClickedSoftwareOnceButton();       // ch:软触发一次 | en:Software Trigger Execute Once
  
    /*ch:参数设置获取 | en:Parameters Get and Set*/
    afx_msg void OnBnClickedGetParameterButton();       // ch:获取参数 | en:Get Parameter

    afx_msg void OnClose();

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int GrabThreadProcess();
    

};

// MultipleCameraDlg.h : header file
//

#if !defined(AFX_MULTIPLECAMERADLG_H__9FD73533_3815_4FC3_929E_C7A264723723__INCLUDED_)
#define AFX_MULTIPLECAMERADLG_H__9FD73533_3815_4FC3_929E_C7A264723723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMultipleCameraDlg dialog

#include <stdio.h>
#include "MultipleCameraDefine.h"

#include "MvCamera.h"

#define IMAGE_NAME_LEN          64
#define WM_TIMER_GRAB_INFO      111

class CMultipleCameraDlg : public CDialog
{
// Construction
public:
	CMultipleCameraDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMultipleCameraDlg)
	enum { IDD = IDD_MULTIPLECAMERA_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultipleCameraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMultipleCameraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedInitDeviceButton();
	afx_msg void OnBnClickedSetExpouseGainButton();
	afx_msg void OnBnClickedStartGrabbingButton();
	afx_msg void OnBnClickedStopGrabbingButton();
	afx_msg void OnBnClickedContinusModeRadio();
	afx_msg void OnBnClickedTriggerModeRadio();
	afx_msg void OnBnClickedSoftwareModeButton();
	afx_msg void OnBnClickedHardwareModeButton();
	afx_msg void OnBnClickedSoftwareOnceButton();
	afx_msg void OnBnClickedCloseButton();
	afx_msg void OnBnClickedClearCountButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// ch:控件对应变量 | en:Control corresponding variable
private:        
    int m_nOnlineNumEdit;
    int m_nUseNumEdit;
    double m_dExpouseEdit;
    double m_dGainEdit;
    BOOL m_bContinusModeRadio;
    BOOL m_bTriggerModeRadio;

	CButton m_RBContinusModeRadio;
	CButton m_RBTriggerModeRadio;

    int m_nFrameCount1Edit;
    int m_nFrameCount2Edit;
    int m_nFrameCount3Edit;
    int m_nFrameCount4Edit;

    int m_nLostFrame1Edit;
    int m_nLostFrame2Edit;
    int m_nLostFrame3Edit;
    int m_nLostFrame4Edit;

private:      
    // ch:状态 | en:
    BOOL  m_bCreateDevice;                          // ch:是否创建设备 | en:Create
    BOOL  m_bOpenDevice;                            // ch:是否打开 | en:Open
    BOOL  m_bStartGrabbing;                         // ch:是否开始抓图 | en:Start grabbing
    int   m_nTriggerMode;                           // ch:触发模式 | en:Trigger mode
    int   m_nTriggerSource;                         // ch:触发源 | en:Trigger source

    int          m_nSelectDeviceNum;                // ch:选中相机个数 | en:Slected camera number
    void*        m_hGetOneFrameHandle[MAX_DEVICE_NUM]; // ch:getoneframe的线程句柄 | en:getoneframe thread handle

public:
    int          m_nUsingCameraNum;
    CMvCamera*   m_pcMyCamera[MAX_DEVICE_NUM];      // ch:CMyCamera封装了常用接口 | en:CMyCamera packed normal used interface

    // ch:标志设备 | en:
    HWND  m_hwndDisplay[MAX_DEVICE_NUM];            // ch:显示句柄 | en:Display window
    CWnd* m_cwmdDisplay[MAX_DEVICE_NUM];

    MV_CC_DEVICE_INFO_LIST m_stDevList;             // ch:设备信息列表结构体变量，用来存储设备列表
                                                    // en:Device information list structure variable, to save device list
public:
    afx_msg void OnClose();                     // ch:右上角退出 | en:Exit from top right corner

private:
    // ch:最开始时的窗口初始化 | en:Window initialization
    int HwndHandleInit(void);                // ch:获取窗口句柄 | en:Get window handle 
    int DisplayWindowInitial(void);          // ch:显示框初始化,最开始的初始化 | en:Display window initialization
    
    // ch:按钮的亮暗使能 | en:Button enable
    int EnableControls(BOOL bIsCameraReady);
    int EnableWindowWhenInitCamera(void);    // ch:按下初始化按钮时的按钮颜色 | en:Button color after press initialization
    void ShowErrorMsg(CString csMessage, int nErrorNum);
    int OpenDevice(void);                     // ch:打开设备 | en:Open Device

    // ch:设置参数操作 | en:Set parameter
    int SetTriggerMode(void);                    // ch:设置触发模式 | en:
    int SetExposureTime(void);                   // ch:设置曝光时间 | en:
    int SetGain(void);                           // ch:设置增益 | en:
    int SetTriggerSource(void);                  // ch:设置触发源 | en:
    int DoSoftwareOnce(void);                    // ch:软触发一次 | en:

    // ch:相机基本功能操作 | en:Basic function
    int StartGrabbing(void);                     // ch:开始抓图 | en:
    int StopGrabbing(void);                      // ch:结束抓图 | en:

    // ch:关闭销毁操作 | en:Close and destroy
    int CloseDevice(void);                       // ch:关闭设备 | en:
    int DestroyDevice(void);                     // ch:销毁设备 | en:

public:
    int ThreadFun(int nUsingDeviceNum);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLECAMERADLG_H__9FD73533_3815_4FC3_929E_C7A264723723__INCLUDED_)

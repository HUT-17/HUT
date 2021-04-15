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

// ch:�ؼ���Ӧ���� | en:Control corresponding variable
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
    // ch:״̬ | en:
    BOOL  m_bCreateDevice;                          // ch:�Ƿ񴴽��豸 | en:Create
    BOOL  m_bOpenDevice;                            // ch:�Ƿ�� | en:Open
    BOOL  m_bStartGrabbing;                         // ch:�Ƿ�ʼץͼ | en:Start grabbing
    int   m_nTriggerMode;                           // ch:����ģʽ | en:Trigger mode
    int   m_nTriggerSource;                         // ch:����Դ | en:Trigger source

    int          m_nSelectDeviceNum;                // ch:ѡ��������� | en:Slected camera number
    void*        m_hGetOneFrameHandle[MAX_DEVICE_NUM]; // ch:getoneframe���߳̾�� | en:getoneframe thread handle

public:
    int          m_nUsingCameraNum;
    CMvCamera*   m_pcMyCamera[MAX_DEVICE_NUM];      // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed normal used interface

    // ch:��־�豸 | en:
    HWND  m_hwndDisplay[MAX_DEVICE_NUM];            // ch:��ʾ��� | en:Display window
    CWnd* m_cwmdDisplay[MAX_DEVICE_NUM];

    MV_CC_DEVICE_INFO_LIST m_stDevList;             // ch:�豸��Ϣ�б�ṹ������������洢�豸�б�
                                                    // en:Device information list structure variable, to save device list
public:
    afx_msg void OnClose();                     // ch:���Ͻ��˳� | en:Exit from top right corner

private:
    // ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Window initialization
    int HwndHandleInit(void);                // ch:��ȡ���ھ�� | en:Get window handle 
    int DisplayWindowInitial(void);          // ch:��ʾ���ʼ��,�ʼ�ĳ�ʼ�� | en:Display window initialization
    
    // ch:��ť������ʹ�� | en:Button enable
    int EnableControls(BOOL bIsCameraReady);
    int EnableWindowWhenInitCamera(void);    // ch:���³�ʼ����ťʱ�İ�ť��ɫ | en:Button color after press initialization
    void ShowErrorMsg(CString csMessage, int nErrorNum);
    int OpenDevice(void);                     // ch:���豸 | en:Open Device

    // ch:���ò������� | en:Set parameter
    int SetTriggerMode(void);                    // ch:���ô���ģʽ | en:
    int SetExposureTime(void);                   // ch:�����ع�ʱ�� | en:
    int SetGain(void);                           // ch:�������� | en:
    int SetTriggerSource(void);                  // ch:���ô���Դ | en:
    int DoSoftwareOnce(void);                    // ch:����һ�� | en:

    // ch:����������ܲ��� | en:Basic function
    int StartGrabbing(void);                     // ch:��ʼץͼ | en:
    int StopGrabbing(void);                      // ch:����ץͼ | en:

    // ch:�ر����ٲ��� | en:Close and destroy
    int CloseDevice(void);                       // ch:�ر��豸 | en:
    int DestroyDevice(void);                     // ch:�����豸 | en:

public:
    int ThreadFun(int nUsingDeviceNum);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLECAMERADLG_H__9FD73533_3815_4FC3_929E_C7A264723723__INCLUDED_)

// ForceIpDemoDlg.h : header file
//

#if !defined(AFX_FORCEIPDEMODLG_H__5ACA97CB_4641_4529_89C8_D9F8EF149751__INCLUDED_)
#define AFX_FORCEIPDEMODLG_H__5ACA97CB_4641_4529_89C8_D9F8EF149751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment(lib, "MvSdkExport.lib")

#include <stdio.h>
#include <WINSOCK2.H>
#include "MvSdkExport.h"

/////////////////////////////////////////////////////////////////////////////
// CForceIpDemoDlg dialog

class CForceIpDemoDlg : public CDialog
{
// Construction
public:
	CForceIpDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CForceIpDemoDlg)
	enum { IDD = IDD_FORCEIPDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForceIpDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CForceIpDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEnumDeviceButton();
	afx_msg void OnSetIpButton();
	afx_msg void OnSelchangeDeviceCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_ctrlDeviceCombo;
    int m_nDeviceCombo;
    CButton m_ctrlEnumDeviceButton;
    CButton m_ctrlSetIpButton;
	DWORD m_dwIpaddress;
	
    MV_CC_DEVICE_INFO_LIST m_stDevList;
    void *                 m_handle;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    void ShowErrorMsg(CString csMessage, int nErrorNum);
private:
    DWORD m_dwNetworkMask;
    DWORD m_dwDefaultGateway;
	
private:
    int DisplayDeviceIp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORCEIPDEMODLG_H__5ACA97CB_4641_4529_89C8_D9F8EF149751__INCLUDED_)

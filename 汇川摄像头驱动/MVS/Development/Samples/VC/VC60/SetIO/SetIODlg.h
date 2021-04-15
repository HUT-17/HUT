// SetIODlg.h : header file
//

#if !defined(AFX_SETIODLG_H__F8A0419E_1AAB_4C95_A0F9_E5DF4970850D__INCLUDED_)
#define AFX_SETIODLG_H__F8A0419E_1AAB_4C95_A0F9_E5DF4970850D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MvCamera.h"


typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1
/////////////////////////////////////////////////////////////////////////////
// CSetIODlg dialog

class CSetIODlg : public CDialog
{
// Construction
public:
	CSetIODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSetIODlg)
	enum { IDD = IDD_SETIO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetIODlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSetIODlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedEnumButton();
	afx_msg void OnBnClickedOpenButton();
	afx_msg void OnBnClickedCloseButton();
	afx_msg void OnBnClickedGetLineselButton();
	afx_msg void OnBnClickedSetLineselButton();
	afx_msg void OnBnClickedGetLinemodeButton();
	afx_msg void OnBnClickedSetLinemodeButton();
    afx_msg void OnClose();                             // ch:���Ͻ��˳� | en:Exit from upper right corner
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:�豸��Ϣ�б�ṹ������������洢�豸�б�
	// en:Device information list structure variable used to store device list
    CMvCamera*      m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
	
private:
    // ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Initialization
    Status EnableWindowWhenInitial(void);          // ch:��ʾ���ʼ��,�ʼ�ĳ�ʼ�� | en:Display window initialization
	
    // ch:��ť������ʹ�� |en:Button bright dark enable
    Status EnableWindowWhenClose(void);         // ch:�ر�ʱ�İ�ť��ɫ���൱���ʼ��ʼ��
	// en:Button color when Close, equivalent to the initialization
    Status EnableWindowWhenOpen(void);  // ch:���´��豸��ťʱ�İ�ť��ɫ | en:Button color when click Open Device
	
    void  ShowErrorMsg(CString csMessage, int nErrorNum); // ch:��ʾ������Ϣ | en:Show error message
    Status OpenDevice(void);
	
    // ch:IO��ز��� | en:IO related operation
    Status GetLineSelector(void);
    Status SetLineSelector(void);
    Status GetLineMode(void);
    Status SetLineMode(void);
	
    // ch:�ر����ٲ��� | en:Close and Destroy operation
    Status CloseDevice(void);                   // ch:�ر��豸 | en:Close device
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    CComboBox m_ctrlDeviceCombo;
    int m_nDeviceCombo;
    CButton m_ctrlOpenButton;
    CButton m_ctrlCloseButton;
    CButton m_ctrlGetLineselButton;
    CButton m_ctrlSetLinesetButton;
    CButton m_ctrlGetLinemodeButton;
    CButton m_ctrlSetLinemodeButton;
    CComboBox m_ctrlLineSelectCombo;
    CComboBox m_ctrlLinemodeCombo;
    CButton m_ctrlEnumButton;
    int m_nLineSelectorCombo;
    int m_nLinemodeCombo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETIODLG_H__F8A0419E_1AAB_4C95_A0F9_E5DF4970850D__INCLUDED_)

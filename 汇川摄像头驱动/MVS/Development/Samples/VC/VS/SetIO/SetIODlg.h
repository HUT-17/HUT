
// SetIODlg.h : header file
//

#pragma once

#include "MvCamera.h"
#include "afxwin.h"

typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

// CSetIODlg dialog
class CSetIODlg : public CDialog
{
// Construction
public:
	CSetIODlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SETIO_DIALOG };

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
    afx_msg void OnBnClickedEnumButton();
    afx_msg void OnBnClickedOpenButton();
    afx_msg void OnBnClickedCloseButton();
    afx_msg void OnBnClickedGetLineselButton();
    afx_msg void OnBnClickedSetLineselButton();
    afx_msg void OnBnClickedGetLinemodeButton();
    afx_msg void OnBnClickedSetLinemodeButton();
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


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
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // ch:设备信息列表结构体变量，用来存储设备列表
                                                // en:Device information list structure variable used to store device list
    CMvCamera*      m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface

private:
    // ch:最开始时的窗口初始化 | en:Initialization
    Status EnableWindowWhenInitial(void);          // ch:显示框初始化,最开始的初始化 | en:Display window initialization

    // ch:按钮的亮暗使能 |en:Button bright dark enable
    Status EnableWindowWhenClose(void);         // ch:关闭时的按钮颜色，相当于最开始初始化
                                                // en:Button color when Close, equivalent to the initialization
    Status EnableWindowWhenOpen(void);  // ch:按下打开设备按钮时的按钮颜色 | en:Button color when click Open Device

    void  ShowErrorMsg(CString csMessage, int nErrorNum); // ch:显示错误信息 | en:Show error message
    Status OpenDevice(void);

    // ch:IO相关操作 | en:IO related operation
    Status GetLineSelector(void);
    Status SetLineSelector(void);
    Status GetLineMode(void);
    Status SetLineMode(void);

    // ch:关闭销毁操作 | en:Close and Destroy operation
    Status CloseDevice(void);                   // ch:关闭设备 | en:Close device

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

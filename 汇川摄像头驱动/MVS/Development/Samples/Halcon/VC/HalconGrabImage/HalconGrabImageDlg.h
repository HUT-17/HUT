
// HalconGrabImageDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MvCameraControl.h"
#include "HalconCpp.h" 

using namespace Halcon;

/*���������붨��*/
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

// CHalconGrabImageDlg �Ի���
class CHalconGrabImageDlg : public CDialog
{
// ����
public:
	CHalconGrabImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HALCONGRABIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedEnumButton();
    afx_msg void OnBnClickedOpenButton();
    afx_msg void OnBnClickedCloseButton();
    afx_msg void OnBnClickedContinusModeRadio();
    afx_msg void OnBnClickedTriggerModeRadio();
    afx_msg void OnBnClickedStartGrabbingButton();
    afx_msg void OnBnClickedGetOneFrameButton();
    afx_msg void OnBnClickedSoftwareTriggerCheck();
    afx_msg void OnBnClickedSoftwareOnceButton();
    afx_msg void OnBnClickedGetParameterButton();
    afx_msg void OnBnClickedSetParameterButton();
    afx_msg void OnCbnSelchangeDeviceCombo();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    //Handler HalconExceptionHandler(const HException& except);

private:
    CComboBox m_ctrlDeviceCombo;
    double m_dExposureEdit;
    double m_dGainEdit;
    double m_dFrameRateEdit;
    BOOL m_bSoftwareTriggerCheck;
    int m_nDeviceCombo;
    CButton m_ctrlOpenButton;
    CButton m_ctrlCloseButton;
    CButton m_ctrlContinusModeRadio;
    CButton m_ctrlTriggerModeRadio;
    CButton m_ctrlStartGrabbingButton;
    CButton m_ctrlGrabImageButton;
    CButton m_ctrlSoftwareTriggerCheck;
    CButton m_ctrlSoftwareOnceButton;
    CEdit m_ctrlExposureEdit;
    CEdit m_ctrlGainEdit;
    CEdit m_ctrlFrameRateEdit;
    CButton m_ctrlGetParameterButton;
    CButton m_ctrlSetParameterButton;

    /*�ڲ�����*/
private:
    /*�ʼʱ�Ĵ��ڳ�ʼ��*/
    Status DisplayWindowInitial(void);          // ��ʾ���ʼ��,�ʼ�ĳ�ʼ�� 

    /*��ť������ʹ��*/
    Status EnableWindowWhenClose(void);         // �ر�ʱ�İ�ť��ɫ���൱���ʼ��ʼ��
    Status EnableWindowWhenOpenNotStart(void);  // ���´��豸��ťʱ�İ�ť��ɫ  
    Status EnableWindowWhenStart(void);         // ���¿�ʼ�ɼ�ʱ�İ�ť��ɫ 
    Status EnableWindowWhenStop(void);          // ���½����ɼ�ʱ�İ�ť��ɫ

    /*��ʼ���������*/
    Status OpenDevice(void);                    // �����

    /*���á���ȡ��������*/
    Status SetTriggerMode(void);                // ���ô���ģʽ
    Status GetTriggerMode(void);
    Status GetExposureTime(void);               // �����ع�ʱ��
    Status SetExposureTime(void); 
    Status GetGain(void);                       // ��������
    Status SetGain(void);
    Status GetFrameRate(void);                  // ����֡��
    Status SetFrameRate(void);
    Status GetTriggerSource(void);              // ���ô���Դ
    Status SetTriggerSource(void);
    /*�ر����ٲ���*/
    Status CloseDevice(void);                   // �ر��豸

    /*�Լ��������*/
private:
    /*״̬*/
    BOOL  m_bRetStatus;                         // ��ʾ��������״̬
    BOOL  m_bCreateDevice;                      // �Ƿ񴴽��豸
    BOOL  m_bOpenDevice;                        // �Ƿ���豸
    char m_chTriggerMode[64];          // ����ģʽ

    /*�豸���*/
    HWND  m_hwndDisplay;                        // ��ʾ���
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // �豸��Ϣ�б�ṹ������������洢�豸�б�

    unsigned char*  m_pBufForSaveImage;         // ���ڱ���ͼ��Ļ���
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver;            // ���ڴ�������ȡͼ��Ļ���
    unsigned int    m_nBufSizeForDriver;

    void*        m_hGetOneFrameHandle;          // getoneframe���߳̾��
    void*        m_hOnlyGetOneFrameHandle;      //��ȡ��֡�̵߳ľ��
public:
    /*Halcon��ر���*/
    HTuple          m_htupHalconHandle;
    HTuple          m_hWindowID;
    BOOL            m_bStartGrabbing;                     // �Ƿ�ʼץͼ
    bool            m_bIsAlwaysGrab;                 //ȡ��֡����һֱȡ�ı�־λ
};

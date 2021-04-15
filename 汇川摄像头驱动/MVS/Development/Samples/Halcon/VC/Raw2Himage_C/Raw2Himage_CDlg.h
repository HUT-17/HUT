
// Raw2Himage_CDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h" 

#include <stdio.h>

#include "MvCamera.h"
#include "ConvertPixel.h"

/*���Ⱥ궨��*/
#define MAX_BUF_SIZE            (1024 * 1024 * 50)
#define MAX_XML_FILE_SIZE       (1024 * 1024 * 3)

/*���������붨��*/
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

// CRaw2Himage_CDlg �Ի���
class CRaw2Himage_CDlg : public CDialog
{
// ����
public:
	CRaw2Himage_CDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RAW2HIMAGE_C_DIALOG };

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

    /*�ؼ���Ӧ����*/
private:
    /*��ʼ��*/
    CButton m_ctrlOpenButton;                   //���豸
    CButton m_ctrlCloseButton;                  //�ر��豸

    /*ͼ��ɼ�*/
    CButton m_ctrlContinusModeRadio;            //����ģʽ             
    CButton m_ctrlTriggerModeRadio;             //����ģʽ
    CButton m_ctrlStartGrabbingButton;          //��ʼץͼ          
    CButton m_ctrlStopGrabbingButton;           //����ץͼ
    CButton m_ctrlSoftwareTriggerCheck;         //����
    BOOL    m_bSoftWareTriggerCheck;
    CButton m_ctrlSoftwareOnceButton;           //����һ��

    /*�豸��ʾ������*/
    CComboBox m_ctrlDeviceCombo;                //ö�ٵ����豸
    int     m_nDeviceCombo;

    /*�ڲ�����*/
private:
    /*�ʼʱ�Ĵ��ڳ�ʼ��*/
    Status DisplayWindowInitial(void);          //��ʾ���ʼ��,�ʼ�ĳ�ʼ�� 

    /*��ť������ʹ��*/
    Status EnableWindowWhenClose(void);         //�ر�ʱ�İ�ť��ɫ���൱���ʼ��ʼ��
    Status EnableWindowWhenOpenNotStart(void);  //���´��豸��ťʱ�İ�ť��ɫ  
    Status EnableWindowWhenStart(void);         //���¿�ʼ�ɼ�ʱ�İ�ť��ɫ 
    Status EnableWindowWhenStop(void);          //���½����ɼ�ʱ�İ�ť��ɫ

    /*��ʼ���������*/
    Status OpenDevice(void);                    //�����

    /*���á���ȡ��������*/
    Status SetTriggerMode(void);                //���ô���ģʽ
    Status GetTriggerMode(void);
    Status SetGain(void);
    Status SetFrameRate(void);
    Status GetTriggerSource(void);              //���ô���Դ
    Status SetTriggerSource(void);

    /*�ر����ٲ���*/
    Status CloseDevice(void);                   //�ر��豸
    Status DestroyHandle(void);                 //�����豸

    /*�Լ��������*/
private:
    /*״̬*/
    BOOL  m_bRetStatus;                         //��ʾ��������״̬
    BOOL  m_bCreateDevice;                      //�Ƿ񴴽��豸
    BOOL  m_bOpenDevice;                        //�Ƿ���豸
    BOOL  m_bStartGrabbing;                     //�Ƿ�ʼץͼ
    int   m_nTriggerMode;                       //����ģʽ
    int   m_nTriggerSource;                     //����Դ
    unsigned char*       m_pImageBuffer;        //������Ϊ����ڵ�

    /*�豸���*/
    CMvCamera*      m_pcMyCamera;               //CMyCamera��װ�˳��ýӿ�
    //HWND  m_hwndDisplay;                        //��ʾ���
    HTuple  m_hWindowID;                        //Halcon��ʾ���
    MV_CC_DEVICE_INFO_LIST m_stDevList;         //�豸��Ϣ�б�ṹ������������洢�豸�б�
public:
    /*��ʼ��*/
    afx_msg void OnBnClickedEnumButton();               //�����豸
    afx_msg void OnBnClickedOpenButton();               //���豸
    afx_msg void OnBnClickedCloseButton();              //�ر��豸

    /*ͼ��ɼ�*/
    afx_msg void OnBnClickedContinusModeRadio();        //����ģʽ
    afx_msg void OnBnClickedTriggerModeRadio();         //����ģʽ
    afx_msg void OnBnClickedStartGrabbingButton();      //��ʼ�ɼ�
    afx_msg void OnBnClickedStopGrabbingButton();       //�����ɼ�
    afx_msg void OnBnClickedSoftwareTriggerCheck();     //����
    afx_msg void OnBnClickedSoftwareOnceButton();       //����һ��

    /*�������û�ȡ*/
    afx_msg void OnBnClickedGetParameterButton();       //��ȡ����
    afx_msg void OnBnClickedSetParameterButton();       //���ò���

public:    
    afx_msg void OnClose();                             //���Ͻ��˳�
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    static unsigned int __stdcall WorkThread(void* pUser);
};

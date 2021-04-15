
// HalconGrabImageDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MvCameraControl.h"
#include "HalconCpp.h" 

using namespace Halcon;

/*函数返回码定义*/
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

// CHalconGrabImageDlg 对话框
class CHalconGrabImageDlg : public CDialog
{
// 构造
public:
	CHalconGrabImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HALCONGRABIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

    /*内部函数*/
private:
    /*最开始时的窗口初始化*/
    Status DisplayWindowInitial(void);          // 显示框初始化,最开始的初始化 

    /*按钮的亮暗使能*/
    Status EnableWindowWhenClose(void);         // 关闭时的按钮颜色，相当于最开始初始化
    Status EnableWindowWhenOpenNotStart(void);  // 按下打开设备按钮时的按钮颜色  
    Status EnableWindowWhenStart(void);         // 按下开始采集时的按钮颜色 
    Status EnableWindowWhenStop(void);          // 按下结束采集时的按钮颜色

    /*初始化相机操作*/
    Status OpenDevice(void);                    // 打开相机

    /*设置、获取参数操作*/
    Status SetTriggerMode(void);                // 设置触发模式
    Status GetTriggerMode(void);
    Status GetExposureTime(void);               // 设置曝光时间
    Status SetExposureTime(void); 
    Status GetGain(void);                       // 设置增益
    Status SetGain(void);
    Status GetFrameRate(void);                  // 设置帧率
    Status SetFrameRate(void);
    Status GetTriggerSource(void);              // 设置触发源
    Status SetTriggerSource(void);
    /*关闭销毁操作*/
    Status CloseDevice(void);                   // 关闭设备

    /*自己定义变量*/
private:
    /*状态*/
    BOOL  m_bRetStatus;                         // 表示函数返回状态
    BOOL  m_bCreateDevice;                      // 是否创建设备
    BOOL  m_bOpenDevice;                        // 是否打开设备
    char m_chTriggerMode[64];          // 触发模式

    /*设备相关*/
    HWND  m_hwndDisplay;                        // 显示句柄
    MV_CC_DEVICE_INFO_LIST m_stDevList;         // 设备信息列表结构体变量，用来存储设备列表

    unsigned char*  m_pBufForSaveImage;         // 用于保存图像的缓存
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char*  m_pBufForDriver;            // 用于从驱动获取图像的缓存
    unsigned int    m_nBufSizeForDriver;

    void*        m_hGetOneFrameHandle;          // getoneframe的线程句柄
    void*        m_hOnlyGetOneFrameHandle;      //获取单帧线程的句柄
public:
    /*Halcon相关变量*/
    HTuple          m_htupHalconHandle;
    HTuple          m_hWindowID;
    BOOL            m_bStartGrabbing;                     // 是否开始抓图
    bool            m_bIsAlwaysGrab;                 //取单帧还是一直取的标志位
};

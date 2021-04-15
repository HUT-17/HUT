//---------------------------------------------------------------------------

#ifndef BasicDemoH
#define BasicDemoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include <stdio.h>
#include "MvCameraControl.h"
/*长度宏定义*/
#define MAX_BUF_SIZE            (1024*1024*58)
#define MAX_XML_FILE_SIZE       (1024*1024*58)

/*函数返回码定义*/
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox2;
        TGroupBox *GroupBox3;
        TButton *SetPara;
        TButton *GetPara;
        TButton *SaveJPG;
        TButton *SaveBMP;
        TButton *SoftwareOnce;
        TButton *StopGrabbing;
        TButton *StartGrabbing;
        TButton *Close;
        TButton *Open;
        TButton *Enum;
        TEdit *GainEdit;
        TEdit *ExpouseEdit;
        TRadioButton *ContinuesMode;
        TRadioButton *TriggerMode;
        TCheckBox *Software;
        TComboBox *DeviceList;
        TPanel *Display;
        TGroupBox *GroupBox4;
        TLabel *Label1;
        TLabel *Label2;
        void __fastcall EnumClick(TObject *Sender);
        void __fastcall OpenClick(TObject *Sender);
        void __fastcall CloseClick(TObject *Sender);
        void __fastcall ContinuesModeClick(TObject *Sender);
        void __fastcall TriggerModeClick(TObject *Sender);
        void __fastcall StartGrabbingClick(TObject *Sender);
        void __fastcall StopGrabbingClick(TObject *Sender);
        void __fastcall SoftwareClick(TObject *Sender);
        void __fastcall SoftwareOnceClick(TObject *Sender);
        void __fastcall SaveBMPClick(TObject *Sender);
        void __fastcall SaveJPGClick(TObject *Sender);
        void __fastcall GetParaClick(TObject *Sender);
        void __fastcall SetParaClick(TObject *Sender);
        
private:	// User declarations
         /*状态*/
        BOOL  m_bRetStatus;                         //表示函数返回状态
        BOOL  m_bCreateDevice;                      //是否创建设备
        BOOL  m_bOpenDevice;                        //是否打开设备
        BOOL  m_bStartGrabbing;                     //是否开始抓图
        int   m_nTriggerMode;                       //触发模式
        int   m_nTriggerSource;                     //触发源
        MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      //保存图像格式

        /*设备相关*/
        void *m_hHandle;
        HWND  m_hwndDisplay;                        //显示句柄
		MV_CC_DEVICE_INFO_LIST m_stDevList;         //设备信息列表结构体变量，用来存储设备列表

		unsigned char*  m_pBufForSaveImage;         // 用于保存图像的缓存
		unsigned int    m_nBufSizeForSaveImage;
        unsigned char*  m_pBufForDriver;            // 用于从驱动获取图像的缓存
        unsigned int    m_nBufSizeForDriver;

        private:
    /*最开始时的窗口初始化*/
	Status __fastcall DisplayWindowInitial(void);          //显示框初始化,最开始的初始化

	Status  __fastcall EnableWindowWhenClose(void);
	Status __fastcall EnableWindowWhenOpenNotStart(void);
	Status __fastcall EnableWindowWhenStart(void);
	Status __fastcall EnableWindowWhenStop(void);

	/*初始化相机操作*/
	Status __fastcall EnumDevice();
	Status __fastcall OpenDevice(void);                    //打开相机

    /*设置、获取参数操作*/
	Status __fastcall SetTriggerMode(void);                //设置触发模式
	Status __fastcall GetTriggerMode(void);
	Status __fastcall GetExposureTime(void);               //设置曝光时间
	Status __fastcall SetExposureTime(void);
	Status __fastcall GetGain(void);                       //设置增益
	Status __fastcall SetGain(void);
	Status __fastcall GetFrameRate(void);                  //设置帧率
	Status __fastcall SetFrameRate(void);
	Status __fastcall GetTriggerSource(void);              //设置触发源
	Status __fastcall SetTriggerSource(void);

    /*图片保存*/
	Status __fastcall SaveImage(void);                     //保存图片

    /*关闭销毁操作*/
	Status __fastcall CloseDevice(void);                   //关闭设备

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

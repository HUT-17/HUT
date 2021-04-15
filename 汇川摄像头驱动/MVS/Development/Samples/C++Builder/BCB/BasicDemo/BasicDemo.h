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
/*���Ⱥ궨��*/
#define MAX_BUF_SIZE            (1024*1024*58)
#define MAX_XML_FILE_SIZE       (1024*1024*58)

/*���������붨��*/
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
         /*״̬*/
        BOOL  m_bRetStatus;                         //��ʾ��������״̬
        BOOL  m_bCreateDevice;                      //�Ƿ񴴽��豸
        BOOL  m_bOpenDevice;                        //�Ƿ���豸
        BOOL  m_bStartGrabbing;                     //�Ƿ�ʼץͼ
        int   m_nTriggerMode;                       //����ģʽ
        int   m_nTriggerSource;                     //����Դ
        MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      //����ͼ���ʽ

        /*�豸���*/
        void *m_hHandle;
        HWND  m_hwndDisplay;                        //��ʾ���
		MV_CC_DEVICE_INFO_LIST m_stDevList;         //�豸��Ϣ�б�ṹ������������洢�豸�б�

		unsigned char*  m_pBufForSaveImage;         // ���ڱ���ͼ��Ļ���
		unsigned int    m_nBufSizeForSaveImage;
        unsigned char*  m_pBufForDriver;            // ���ڴ�������ȡͼ��Ļ���
        unsigned int    m_nBufSizeForDriver;

        private:
    /*�ʼʱ�Ĵ��ڳ�ʼ��*/
	Status __fastcall DisplayWindowInitial(void);          //��ʾ���ʼ��,�ʼ�ĳ�ʼ��

	Status  __fastcall EnableWindowWhenClose(void);
	Status __fastcall EnableWindowWhenOpenNotStart(void);
	Status __fastcall EnableWindowWhenStart(void);
	Status __fastcall EnableWindowWhenStop(void);

	/*��ʼ���������*/
	Status __fastcall EnumDevice();
	Status __fastcall OpenDevice(void);                    //�����

    /*���á���ȡ��������*/
	Status __fastcall SetTriggerMode(void);                //���ô���ģʽ
	Status __fastcall GetTriggerMode(void);
	Status __fastcall GetExposureTime(void);               //�����ع�ʱ��
	Status __fastcall SetExposureTime(void);
	Status __fastcall GetGain(void);                       //��������
	Status __fastcall SetGain(void);
	Status __fastcall GetFrameRate(void);                  //����֡��
	Status __fastcall SetFrameRate(void);
	Status __fastcall GetTriggerSource(void);              //���ô���Դ
	Status __fastcall SetTriggerSource(void);

    /*ͼƬ����*/
	Status __fastcall SaveImage(void);                     //����ͼƬ

    /*�ر����ٲ���*/
	Status __fastcall CloseDevice(void);                   //�ر��豸

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

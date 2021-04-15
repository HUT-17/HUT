/************************************************************************/
/* ��C++�ӿ�Ϊ�������Գ��ú������ж��η�װ�������û�ʹ��                */
/************************************************************************/

#ifndef _MY_CAMERA_H_
#define _MY_CAMERA_H_

#include <stdio.h>

#include "MvErrorDefine.h"
#include "CameraParams.h"
#include "MvCameraControl.h"

#define MY_OK           0
#define MY_FAIL         -1

class CMyCamera
{
public:
    // ���캯��
    CMyCamera();
    // ��������
    ~CMyCamera();

    static int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);

    // ���豸
    int     Open(MV_CC_DEVICE_INFO* pstDeviceInfo);

    // �ر��豸
    int     Close();

    // ����ץͼ
    int     StartGrabbing();

    // ֹͣץͼ
    int     StopGrabbing();

    // ������ȡһ֡ͼ������
    int     GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);

    // ������ʾ���ھ��
    int     Display(void* hWnd);

    // ����ͼƬ
    int     SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam);

    // ע��ͼ�����ݻص�
    int     RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                    void* pUser);

    // ע����Ϣ�쳣�ص�
    int     RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser),
                                        void* pUser);

    // ���úͻ�ȡInt�Ͳ������� Width��Height����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
    int     GetIntValue(IN const char* strKey, OUT unsigned int *pnValue);
    int     SetIntValue(IN const char* strKey, IN unsigned int nValue);

    // ���úͻ�ȡFloat�Ͳ������� ExposureTime��Gain����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
    int     GetFloatValue(IN const char* strKey, OUT float *pfValue);
    int     SetFloatValue(IN const char* strKey, IN float fValue);

    // ���úͻ�ȡEnum�Ͳ������� PixelFormat����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
    int     GetEnumValue(IN const char* strKey, OUT unsigned int *pnValue);
    int     SetEnumValue(IN const char* strKey, IN unsigned int nValue);

    // ���úͻ�ȡBool�Ͳ������� ReverseX����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
    int     GetBoolValue(IN const char* strKey, OUT bool *pbValue);
    int     SetBoolValue(IN const char* strKey, IN bool bValue);

    // ���úͻ�ȡString�Ͳ������� DeviceUserID����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�UserSetSave
    int     GetStringValue(IN const char* strKey, IN OUT char* strValue, IN unsigned int nSize);
    int     SetStringValue(IN const char* strKey, IN const char * strValue);

    // ִ��һ��Command������� UserSetSave����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
    int     CommandExecute(IN const char* strKey);

private:

    void*               m_hDevHandle;


};

#endif

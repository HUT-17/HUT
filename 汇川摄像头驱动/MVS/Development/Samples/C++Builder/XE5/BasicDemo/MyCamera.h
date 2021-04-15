/************************************************************************/
/* 以C++接口为基础，对常用函数进行二次封装，方便用户使用                */
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
    // 构造函数
    CMyCamera();
    // 析构函数
    ~CMyCamera();

    static int EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList);

    // 打开设备
    int     Open(MV_CC_DEVICE_INFO* pstDeviceInfo);

    // 关闭设备
    int     Close();

    // 开启抓图
    int     StartGrabbing();

    // 停止抓图
    int     StopGrabbing();

    // 主动获取一帧图像数据
    int     GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);

    // 设置显示窗口句柄
    int     Display(void* hWnd);

    // 保存图片
    int     SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam);

    // 注册图像数据回调
    int     RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                    void* pUser);

    // 注册消息异常回调
    int     RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser),
                                        void* pUser);

    // 设置和获取Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int     GetIntValue(IN const char* strKey, OUT unsigned int *pnValue);
    int     SetIntValue(IN const char* strKey, IN unsigned int nValue);

    // 设置和获取Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int     GetFloatValue(IN const char* strKey, OUT float *pfValue);
    int     SetFloatValue(IN const char* strKey, IN float fValue);

    // 设置和获取Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int     GetEnumValue(IN const char* strKey, OUT unsigned int *pnValue);
    int     SetEnumValue(IN const char* strKey, IN unsigned int nValue);

    // 设置和获取Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int     GetBoolValue(IN const char* strKey, OUT bool *pbValue);
    int     SetBoolValue(IN const char* strKey, IN bool bValue);

    // 设置和获取String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
    int     GetStringValue(IN const char* strKey, IN OUT char* strValue, IN unsigned int nSize);
    int     SetStringValue(IN const char* strKey, IN const char * strValue);

    // 执行一次Command型命令，如 UserSetSave，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int     CommandExecute(IN const char* strKey);

private:

    void*               m_hDevHandle;


};

#endif

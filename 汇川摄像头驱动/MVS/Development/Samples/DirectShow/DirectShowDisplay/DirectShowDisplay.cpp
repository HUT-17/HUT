#include <afxwin.h>
#include <dshow.h>
#include <initguid.h>
#include <conio.h>
#include "MvCameraDS.h"

void WaitForKeyPress(void)
{
    while(!_kbhit())
    {
        Sleep(10);
    }
    _getch();
}

void __stdcall ImageCallBackEx(double dblSampleTime, BYTE* pBuffer, long lBufferSize)
{
     printf("Get One Frame: SampleTime[%f], BufferSize[%ld]\n", dblSampleTime, lBufferSize);
}

int main()
{
    CWinApp lApp; 
    AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);

    // 1.初始化COM库
    ::CoInitialize( NULL );

    //仅获取相机数目
    int nCamCount = CMvCameraDS::CameraCount();
    printf("There are %d cameras.\n", nCamCount);

    if(nCamCount == 0)
    {
        return (-1);
    }

    //获取相机的名称
    for(int i = 0; i < nCamCount; i++)
    {
        char szCamName[512];
        int nRet = CMvCameraDS::CameraName(i, szCamName, sizeof(szCamName));

        if(nRet > 0)
        {
            printf("Camera index [%d]: '%s'.\n", i, szCamName);
        }
        else
        {
            printf("Can not get Camera %d's name.\n", i);
        }
    }

    CMvCameraDS m_CamDS;
    HRESULT hr = S_OK;

    // 2.选择一个相机打开
    do
    {
        unsigned int nCamNum = 0;
        printf("Select a Camera [Index]:\n");
        scanf_s("%d", &nCamNum);
        hr = m_CamDS.OpenCamera(nCamNum, false);
        if(FAILED(hr))
        {
            //打开相机失败
            printf("Open failed, Error code: [%x]\n", hr);
            break;
        }

        //获取相机配置
        m_CamDS.GetCameraControl();

        //获取图像属性配置
        m_CamDS.GetVideoProcAmp();

        //调用属性页面
        m_CamDS.ShowFilterProperty();

        //注册回调
        printf("RegisterImageCallback\n");
        m_CamDS.RegisterImageCallback(ImageCallBackEx);

        //开始取图
        printf("Start grab\n");
        m_CamDS.StartGrab();

    }while(0);

    //关闭相机
    printf("Close Camera\n");
    m_CamDS.CloseCamera();

    // 释放库资源
    ::CoUninitialize();
    AfxWinTerm();

    printf("Press a key to exit\n");
    WaitForKeyPress();
    return 0;
}
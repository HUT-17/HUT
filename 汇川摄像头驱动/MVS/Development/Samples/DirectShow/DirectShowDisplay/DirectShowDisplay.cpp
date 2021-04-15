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

    // 1.��ʼ��COM��
    ::CoInitialize( NULL );

    //����ȡ�����Ŀ
    int nCamCount = CMvCameraDS::CameraCount();
    printf("There are %d cameras.\n", nCamCount);

    if(nCamCount == 0)
    {
        return (-1);
    }

    //��ȡ���������
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

    // 2.ѡ��һ�������
    do
    {
        unsigned int nCamNum = 0;
        printf("Select a Camera [Index]:\n");
        scanf_s("%d", &nCamNum);
        hr = m_CamDS.OpenCamera(nCamNum, false);
        if(FAILED(hr))
        {
            //�����ʧ��
            printf("Open failed, Error code: [%x]\n", hr);
            break;
        }

        //��ȡ�������
        m_CamDS.GetCameraControl();

        //��ȡͼ����������
        m_CamDS.GetVideoProcAmp();

        //��������ҳ��
        m_CamDS.ShowFilterProperty();

        //ע��ص�
        printf("RegisterImageCallback\n");
        m_CamDS.RegisterImageCallback(ImageCallBackEx);

        //��ʼȡͼ
        printf("Start grab\n");
        m_CamDS.StartGrab();

    }while(0);

    //�ر����
    printf("Close Camera\n");
    m_CamDS.CloseCamera();

    // �ͷſ���Դ
    ::CoUninitialize();
    AfxWinTerm();

    printf("Press a key to exit\n");
    WaitForKeyPress();
    return 0;
}
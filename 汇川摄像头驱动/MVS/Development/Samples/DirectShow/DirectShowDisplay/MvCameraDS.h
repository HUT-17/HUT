#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include <afxwin.h>
#include <atlbase.h>
#include "qedit.h"
#include "dshow.h"
#include "MvDSSource2.h"
#include "MvErrorDefine.h"

class CSampleGrabberCB : public ISampleGrabberCB
{
public:
    //����ӿ�
    CSampleGrabberCB()
    {
        m_cbImageOutput = NULL;
    }

    void SetImageCB(void(__stdcall* cbImageCB)(double dblSampleTime, BYTE* pBuffer, long lBufferSize))
    {
        m_cbImageOutput = cbImageCB;
    }

private:
    void(__stdcall* m_cbImageOutput)(double dblSampleTime, BYTE* pBuffer, long lBufferSize);

public:
    //�麯��ʵ��
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {  
        //�ûص���֧��
        return S_OK;
    }

    STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
    {
        if(m_cbImageOutput)
        {
            m_cbImageOutput(dblSampleTime, pBuffer, lBufferSize);
        }
        return S_OK;
    }

    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {  
        if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
        {  
            *ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
            return NOERROR;
        }      
        return E_NOINTERFACE;
    }
};

class CMvCameraDS
{
private:

    MvDSSource2* m_pDSSource2;

    bool m_bConnected;  //���������

    int m_nWidth, m_nHeight;

    CComPtr<IMoniker>       m_pMoniker;             //��ǰ�����COM����
    CComPtr<IGraphBuilder>  m_pGraph;               //Filter������
    CComPtr<IBaseFilter>    m_pCameraFilter;        //ԴFilter
    CComPtr<IBaseFilter>    m_pSampleGrabberFilter; //ȡͼFilter
    CComPtr<IBaseFilter>    m_pRenderFilter;        //��ȾFilter
    CComPtr<ISampleGrabber> m_pSampleGrabber;       //ȡͼ����

    CComPtr<IMediaControl>  m_pMediaControl;        //��ý�����

    CComPtr<IPin> m_pCameraOutputPin;   //������
    CComPtr<IPin> m_pGrabberInput;      //ȡͼ����
    CComPtr<IPin> m_pGrabberOutput;     //ȡͼ���
    CComPtr<IPin> m_pRenderInputPin;    //��Ⱦ����

    CSampleGrabberCB m_CB;              //ץͼ�ص��ӿ�, �ڻص��пɽ����Լ��Ĵ���

private:

    //��ȡָ����������
    IPin* GetPins( IBaseFilter *pFilter, PIN_DIRECTION isOutput );

    HRESULT BindFilter(int nCamIDX, IBaseFilter **pFilter);

public:

    CMvCameraDS();
    virtual ~CMvCameraDS();

    //��������ͷ����Ŀ
    //���Բ��ô���CCameraDSʵ��, ����CCameraDS::CameraCount()�õ����
    static int CameraCount(); 

    //��������ͷ�ı�ŷ�������ͷ������
    //nCamID: ����ͷ���
    //pcName: ���ڴ������ͷ���ֵ�����
    //nBufferSize: pcName�Ĵ�С
    //���Բ��ô���CCameraDSʵ��, ����CCameraDS::CameraName()�õ����
    static int CameraName(int nCamID, char* pcName, int nBufferSize);

    //����ȡͼ�ص�
    void RegisterImageCallback(void(__stdcall* cbImageCB)(double dblSampleTime, BYTE* pBuffer, long lBufferSize));

    //������ͷ��nCamIDָ�����ĸ�����ͷ��ȡֵ����Ϊ0,1,2,
    //bDisplayPropertiesָʾ�Ƿ��Զ���������ͷ����ҳ
    HRESULT OpenCamera(int nCamID, bool bDisplayProperties = true);

    //��ʾ����ҳ��
    HRESULT ShowFilterProperty();

    //��ʾ����ҳ��
    HRESULT ShowPinProperty();

    //��ȡ�������
    HRESULT GetCameraControl();

    //��ȡͼ�������ֵ
    HRESULT GetVideoProcAmp();

    //����ͼ����
    int GetWidth();

    //����ͼ��߶�
    int GetHeight();

    //��ʼץͼ
    int StartGrab();

    //�ر�����ͷ�������������Զ������������
    void CloseCamera();
};

#endif 

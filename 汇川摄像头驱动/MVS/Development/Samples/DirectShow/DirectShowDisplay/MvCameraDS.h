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
    //对外接口
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
    //虚函数实现
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {  
        //该回调不支持
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

    bool m_bConnected;  //相机已连接

    int m_nWidth, m_nHeight;

    CComPtr<IMoniker>       m_pMoniker;             //当前相机的COM对象
    CComPtr<IGraphBuilder>  m_pGraph;               //Filter管理器
    CComPtr<IBaseFilter>    m_pCameraFilter;        //源Filter
    CComPtr<IBaseFilter>    m_pSampleGrabberFilter; //取图Filter
    CComPtr<IBaseFilter>    m_pRenderFilter;        //渲染Filter
    CComPtr<ISampleGrabber> m_pSampleGrabber;       //取图控制

    CComPtr<IMediaControl>  m_pMediaControl;        //多媒体控制

    CComPtr<IPin> m_pCameraOutputPin;   //相机输出
    CComPtr<IPin> m_pGrabberInput;      //取图输入
    CComPtr<IPin> m_pGrabberOutput;     //取图输出
    CComPtr<IPin> m_pRenderInputPin;    //渲染输入

    CSampleGrabberCB m_CB;              //抓图回调接口, 在回调中可进行自己的处理

private:

    //获取指定方向的针脚
    IPin* GetPins( IBaseFilter *pFilter, PIN_DIRECTION isOutput );

    HRESULT BindFilter(int nCamIDX, IBaseFilter **pFilter);

public:

    CMvCameraDS();
    virtual ~CMvCameraDS();

    //返回摄像头的数目
    //可以不用创建CCameraDS实例, 采用CCameraDS::CameraCount()得到结果
    static int CameraCount(); 

    //根据摄像头的编号返回摄像头的名字
    //nCamID: 摄像头编号
    //pcName: 用于存放摄像头名字的数组
    //nBufferSize: pcName的大小
    //可以不用创建CCameraDS实例, 采用CCameraDS::CameraName()得到结果
    static int CameraName(int nCamID, char* pcName, int nBufferSize);

    //设置取图回调
    void RegisterImageCallback(void(__stdcall* cbImageCB)(double dblSampleTime, BYTE* pBuffer, long lBufferSize));

    //打开摄像头，nCamID指定打开哪个摄像头，取值可以为0,1,2,
    //bDisplayProperties指示是否自动弹出摄像头属性页
    HRESULT OpenCamera(int nCamID, bool bDisplayProperties = true);

    //显示属性页面
    HRESULT ShowFilterProperty();

    //显示属性页面
    HRESULT ShowPinProperty();

    //获取相机设置
    HRESULT GetCameraControl();

    //获取图像各属性值
    HRESULT GetVideoProcAmp();

    //返回图像宽度
    int GetWidth();

    //返回图像高度
    int GetHeight();

    //开始抓图
    int StartGrab();

    //关闭摄像头，析构函数会自动调用这个函数
    void CloseCamera();
};

#endif 

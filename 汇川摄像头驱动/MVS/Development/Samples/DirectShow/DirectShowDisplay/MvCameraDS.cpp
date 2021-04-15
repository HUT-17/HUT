#include "MvCameraDS.h"

#pragma comment(lib,"Strmiids.lib") 

// Forward declarations
BOOL CreateDisplayWindow();
void UpdateDisplayPosition();
LRESULT CALLBACK WndProc( HWND aHwnd, UINT aMsg, WPARAM wParam, LPARAM lParam );

// ��ʾ���� Globals
IVideoWindow *gVideoWindow = NULL;
HWND gWindowHandle = NULL;

CMvCameraDS::CMvCameraDS()
{
    m_bConnected = false;
    m_nWidth = m_nHeight = 0;

    m_pMoniker = NULL;
    m_pGraph = NULL;
    m_pCameraFilter = NULL;
    m_pRenderFilter = NULL;
    m_pSampleGrabberFilter = NULL;

    CoInitialize(NULL);
}

CMvCameraDS::~CMvCameraDS()
{
    CloseCamera();
    CoUninitialize();
}

int CMvCameraDS::CameraCount()
{
    int nCount = 0;
    CoInitialize(NULL);

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR) 
    {
        return nCount;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    while(hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
    {
        nCount++;
    }

    pCreateDevEnum = NULL;
    pEm = NULL;
    return nCount;
}

int CMvCameraDS::CameraName(int nCamID, char* pcCameraName, int nBufferSize)
{
    int nCount = 0;
    CoInitialize(NULL);

    // ����ö����
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR)
        return 0;

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    while(hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
    {
        if (nCount == nCamID)
        {
            IPropertyBag *pBag = 0;
            hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
            if(SUCCEEDED(hr))
            {
                VARIANT var;
                var.vt = VT_BSTR;
                hr = pBag->Read(L"FriendlyName", &var, NULL); //������������,��GUID, ������Ϣ�ȵ�
                if(hr == NOERROR)
                {
                    //��ȡ�豸����
                    WideCharToMultiByte(CP_ACP, 0, var.bstrVal, -1, pcCameraName, nBufferSize, "", NULL);
                    SysFreeString(var.bstrVal);
                }
                pBag->Release();
            }
            pM->Release();

            break;
        }
        nCount++;
    }

    pCreateDevEnum = NULL;
    pEm = NULL;

    return 1;
}
HRESULT CMvCameraDS::OpenCamera(int nCamID, bool bDisplayProperties)
{
    HRESULT hr = S_OK;
    CoInitialize(NULL);

    do 
    {
        // ����Filter Graph Manager
        hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGraph);
        // ��ȡ���ƽӿ�
        hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **) &m_pMediaControl);

        // �������Filter
        hr = BindFilter(nCamID, &m_pCameraFilter);
        if (FAILED(hr))
        {
            break;
        }
        //��ȡ���ƽӿ�
        m_pCameraFilter->QueryInterface(IID_IMvDSSource2, (void**)&m_pDSSource2);
        //������� m_pDSSource2�ɵ��÷�װ�õ�������ƽӿ�,����ӿ���鿴ͷ�ļ�
        MVCC_FLOATVALUE stFloatValue = {0};
        hr = m_pDSSource2->GetFloatValue("ExposureTime", &stFloatValue);

        // ����ȡͼFilter
        hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID *)&m_pSampleGrabberFilter);
        // ��ȡ���ƽӿ�
        hr = m_pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&m_pSampleGrabber);

        // ������ȾFilter
        hr = CoCreateInstance(CLSID_VideoRenderer, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*) &m_pRenderFilter);

        //��Filter��ӵ�Graph
        hr = m_pGraph->AddFilter(m_pCameraFilter, L"CameraFilter");
        if (FAILED(hr))
        {
            //...
            break;
        }
        m_pGraph->AddFilter(m_pSampleGrabberFilter, L"Grabber");
        if (FAILED(hr))
        {
            //...
            break;
        }
        hr = m_pGraph->AddFilter(m_pRenderFilter, L"VideoRender");
        if (FAILED(hr))
        {
            //...
            break;
        }

        //��ȡpin
        //printf("Get camera filter input pin\r\n");
        m_pCameraOutputPin = GetPins(m_pCameraFilter, PINDIR_OUTPUT);
        if (m_pCameraOutputPin == NULL)
        {
            //...
            break;
        }
        //printf("Get grabber filter input pin\r\n");
        m_pGrabberInput = GetPins(m_pSampleGrabberFilter, PINDIR_INPUT);
        if (m_pGrabberInput == NULL)
        {
            //...
            break;
        }
        //printf("Get grabber filter input pin\r\n");
        m_pGrabberOutput = GetPins(m_pSampleGrabberFilter, PINDIR_OUTPUT);
        if (m_pGrabberOutput == NULL)
        {
            //...
            break;
        }
        //printf("Get render filter input pin\r\n");
        m_pRenderInputPin = GetPins(m_pRenderFilter, PINDIR_INPUT);
        if (m_pRenderInputPin == NULL)
        {
            //...
            break;
        }

        //����Pin, Camera[OUT]-->[IN]Grabber[OUT]-->[IN]Render
        //printf("Connecting filter pins\r\n");
        hr = m_pGraph->Connect( m_pCameraOutputPin, m_pGrabberInput );
        if ( FAILED( hr ) )
        {
            // ...
            break;
        }
        hr = m_pGraph->Connect( m_pGrabberOutput, m_pRenderInputPin );
        if ( FAILED( hr ) )
        {
            // ...
            break;
        }

        //��ʾ����ҳ
        if (bDisplayProperties)
        {
            ShowFilterProperty();

            ShowPinProperty();
        }

        //��ȡ�������
        AM_MEDIA_TYPE stMediaType;
        hr = m_pSampleGrabber->GetConnectedMediaType(&stMediaType);
        if(FAILED(hr))
        {
            //...
        }

        VIDEOINFOHEADER *videoHeader;
        videoHeader = reinterpret_cast<VIDEOINFOHEADER*>(stMediaType.pbFormat);
        m_nWidth = videoHeader->bmiHeader.biWidth;
        m_nHeight = videoHeader->bmiHeader.biHeight;
        m_bConnected = true;

    } while (0);

    if (FAILED(hr))
    {
        CloseCamera();
    }

    return hr;
}

HRESULT CMvCameraDS::ShowFilterProperty()
{
    if (m_pCameraFilter == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    CComPtr<ISpecifyPropertyPages> pPages;

    //filter����ҳ
    hr = m_pCameraFilter->QueryInterface(IID_ISpecifyPropertyPages, (void**)&pPages);
    if(SUCCEEDED(hr))
    {
        PIN_INFO stPinInfo;
        m_pCameraOutputPin->QueryPinInfo(&stPinInfo);

        CAUUID caGUID;
        pPages->GetPages(&caGUID);

        OleCreatePropertyFrame(NULL, 0, 0,
            L"Property Sheet", 1,
            (IUnknown **)&(m_pCameraFilter.p),
            caGUID.cElems, caGUID.pElems,
            0, 0, NULL);

        CoTaskMemFree(caGUID.pElems);
        stPinInfo.pFilter->Release();
    }
    pPages = NULL;  //CComPtr����ʹ�������Ҫ��ΪNULL, �ͷ���Դ

    return S_OK;
}

HRESULT CMvCameraDS::ShowPinProperty()
{
    if (m_pCameraFilter == NULL || m_pCameraOutputPin == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    CComPtr<ISpecifyPropertyPages> pPages;

    //pin����ҳ
    hr = m_pCameraOutputPin->QueryInterface(IID_ISpecifyPropertyPages, (void**)&pPages);
    if (SUCCEEDED(hr))
    {
        PIN_INFO PinInfo;
        m_pCameraOutputPin->QueryPinInfo(&PinInfo);

        CAUUID caGUID;
        pPages->GetPages(&caGUID);

        OleCreatePropertyFrame(NULL, 0, 0,
            L"Property Sheet", 1,
            (IUnknown **)&(m_pCameraOutputPin.p),
            caGUID.cElems, caGUID.pElems,
            0, 0, NULL);

        CoTaskMemFree(caGUID.pElems);
        PinInfo.pFilter->Release();
    }
    pPages = NULL;

    return S_OK;
}

HRESULT CMvCameraDS::GetCameraControl()
{
    if (m_pCameraFilter == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    // Query the capture filter for the IAMVideoProcAmp interface.
    IAMCameraControl *pCameraControl = NULL;
    hr = m_pCameraFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
    if ( FAILED(hr) || (pCameraControl == NULL) )
    {
        // The device does not support IAMVideoProcAmp
        return hr;
    }
    else
    {
        long Min, Max, Step, Default, Flags, Val;

        // Get the range and default value
        hr = pCameraControl->GetRange(CameraControl_Exposure, &Min, &Max, &Step,
            &Default, &Flags);
        if (SUCCEEDED(hr))
        {
            // Get the current value.
            hr = pCameraControl->Get(CameraControl_Exposure, &Val, &Flags);
            if (SUCCEEDED(hr))
            {
                printf("VideoProcAmp_Gain[%d]\n", Val);
            }

            hr = pCameraControl->Set(CameraControl_Exposure, Val, Flags);
            if (SUCCEEDED(hr))
            {
                printf("VideoProcAmp_Gain[%d]\n", Val);
            }
        }

    }

    if (pCameraControl)
    {
        pCameraControl->Release();
    }

    return S_OK;
}

HRESULT CMvCameraDS::GetVideoProcAmp()
{
    if (m_pCameraFilter == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;

    // Query the capture filter for the IAMVideoProcAmp interface.
    IAMVideoProcAmp *pProcAmp = NULL;
    hr = m_pCameraFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&pProcAmp);
    if ( FAILED(hr) || (pProcAmp == NULL) )
    {
        // The device does not support IAMVideoProcAmp
        return hr;
    }
    else
    {
        long Min, Max, Step, Default, Flags, Val;

        // Get the range and default value
        hr = pProcAmp->GetRange(VideoProcAmp_Gain, &Min, &Max, &Step,
            &Default, &Flags);
        if (SUCCEEDED(hr))
        {
            // Get the current value.
            hr = pProcAmp->Get(VideoProcAmp_Gain, &Val, &Flags);
            if (SUCCEEDED(hr))
            {
                printf("VideoProcAmp_Gain[%d]\n", Val);
            }

            hr = pProcAmp->Set(VideoProcAmp_Gain, Val, Flags);
            if (SUCCEEDED(hr))
            {
                printf("VideoProcAmp_Gain[%d]\n", Val);
            }
        }
        
    }

    if (pProcAmp)
    {
        pProcAmp->Release();
    }

    return S_OK;
}

int CMvCameraDS::GetWidth()
{
    return m_nWidth;
}

int CMvCameraDS::GetHeight()
{
    return m_nHeight;
}

HRESULT CMvCameraDS::BindFilter(int nCamID, IBaseFilter **pFilter)
{
    if (nCamID < 0)
    {
        return MV_E_PARAMETER;
    }
 
    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum  = NULL; //ö����
    CComPtr<IEnumMoniker> pEm               = NULL; //��ö����
    CComPtr<IMoniker> pM                    = NULL; //��ǰ��
    CComPtr<IPropertyBag> pBag              = NULL; //����
    int nCount = 0;

    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)
    {
        return hr;
    }

    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR) 
    {
        return hr;
    }

    ULONG cFetched = 0;
    pEm->Reset();
    while(hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
    {
        if (nCount == nCamID)
        {
            hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag); //��ȡ���Լ�
            if(SUCCEEDED(hr)) 
            {
                //����ͨ����ȡGUID�Ŵ���Filter
                //hr = pBag->Read(L"CLSID", &var, NULL);
                //CLSID clsid;
                //hr = ::CLSIDFromString( var.bstrVal, &clsid);
                //if (hr == NOERROR) 
                //{
                //    CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)pFilter);
                //    SysFreeString(var.bstrVal);
                //}

                //Ҳ����ֱ�ӻ�ȡpM�󶨵�Filter
                hr = pM->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)pFilter);
                if (FAILED(hr))
                {
                    return hr;
                }
                pBag = NULL;
            }
        }
        pM = NULL;
        nCount++;
    }
    pEm = NULL;

    pCreateDevEnum = NULL;

    //û���ҵ�Filter
    if ((*pFilter) == NULL)
    {
        return VFW_E_NOT_FOUND;
    }

    return hr;
}

int CMvCameraDS::StartGrab()
{
    HRESULT hr = S_OK;

    // ��������
    //printf("Creating display window\r\n");
    BOOL lSuccess = CreateDisplayWindow();
    if ( !lSuccess )
    {
        return MV_E_RESOURCE;
    }

    // ����������Ϊ�������
    hr = m_pGraph->QueryInterface( IID_IVideoWindow, (void **)&gVideoWindow );
    if ( FAILED( hr ) )
    {
        // ...
    }

    // ��ʾ���ڰ����
    gVideoWindow->put_Owner( (OAHWND)gWindowHandle );
    gVideoWindow->put_MessageDrain( (OAHWND)gWindowHandle );
    gVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );
    UpdateDisplayPosition();

    //���ûص�����
    hr = m_pSampleGrabber->SetCallback(&m_CB, 1);
    if(FAILED(hr))
    {
        return hr;
    }

    m_pSampleGrabber->SetBufferSamples(TRUE);
    m_pSampleGrabber->SetOneShot(FALSE);

    hr = m_pMediaControl->Run();

    //��������Ϣ
    MSG lMsg;
    while ( ::GetMessage( &lMsg, NULL, 0, 0 ) > 0 )
    {
        TranslateMessage( &lMsg );
        DispatchMessage( &lMsg );
    }

    return hr;
}

//ע��ص�
void CMvCameraDS::RegisterImageCallback(void(__stdcall* cbImageCB)(double dblSampleTime, BYTE* pBuffer, long lBufferSize))
{
    return  m_CB.SetImageCB(cbImageCB);
}

// ��ȡFilter��ָ������Pin
IPin* CMvCameraDS::GetPins( IBaseFilter *pFilter, PIN_DIRECTION isOutput )
{
    // Get pin enumeration interface from the filter
    BOOL    bFound = FALSE;
    IPin*   pPin = NULL;
    ULONG   lCount = 0;
    IEnumPins* pEnum = NULL;

    HRESULT lResult = pFilter->EnumPins( &pEnum );
    if ( FAILED( lResult ) )
    {
        return NULL;
    }

    // ��Ϊ����Ŀǰֻ��һ��Pin, ֻ��ȡö�ٵ��ĵ�һ��(��������)Pin
    while(pEnum->Next(1, &pPin, &lCount) == S_OK)
    {
        PIN_DIRECTION enPinDir;
        pPin->QueryDirection(&enPinDir);
        if (bFound = (enPinDir == isOutput))
            break;
        pPin->Release();
    }
    pEnum->Release();
    return (bFound ? pPin : NULL);
}

void CMvCameraDS::CloseCamera()
{
    if(m_bConnected)
    {
        m_pMediaControl->Stop();
    }

    m_pDSSource2 = NULL;
    m_pMoniker = NULL;
    m_pGraph = NULL;
    m_pCameraFilter = NULL;
    m_pMediaControl = NULL;
    m_pSampleGrabberFilter = NULL;
    m_pSampleGrabber = NULL;
    m_pGrabberInput = NULL;
    m_pGrabberOutput = NULL;
    m_pCameraOutputPin = NULL;
    m_pRenderFilter = NULL;
    m_pRenderInputPin = NULL;

    m_bConnected = false;
    m_nWidth = m_nHeight = 0;
}

// ������ʾ����
BOOL CreateDisplayWindow()
{
    const wchar_t ClassName[] = L"DisplayWindowClass";

    WNDCLASSEX lWindowClass;
    lWindowClass.cbSize = sizeof(WNDCLASSEX);
    lWindowClass.style = 0;
    lWindowClass.lpfnWndProc = WndProc;
    lWindowClass.cbClsExtra = 0;
    lWindowClass.cbWndExtra = 0;
    lWindowClass.hInstance = ::AfxGetInstanceHandle();
    lWindowClass.hIcon = ::LoadIcon( NULL, IDI_APPLICATION );
    lWindowClass.hCursor = ::LoadCursor( NULL, IDC_ARROW );
    lWindowClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    lWindowClass.lpszMenuName = NULL;
    lWindowClass.lpszClassName = ClassName;
    lWindowClass.hIconSm = ::LoadIcon( NULL, IDI_APPLICATION );

    if( !::RegisterClassEx( &lWindowClass ) )
    {
        return FALSE;
    }

    gWindowHandle = ::CreateWindowEx(WS_EX_CLIENTEDGE,ClassName,L"DirectShowDisplay",WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,NULL, NULL, ::AfxGetInstanceHandle(), NULL );
    if ( gWindowHandle == NULL )
    {
        return FALSE;
    }

    ::ShowWindow( gWindowHandle, SW_SHOW );
    ::UpdateWindow( gWindowHandle );

    return TRUE;
}

// ���´���λ��
void UpdateDisplayPosition()
{
    RECT lRect;
    ::GetClientRect( gWindowHandle, &lRect );

    if ( gVideoWindow != NULL )
    {
        gVideoWindow->SetWindowPosition( 0, 0, lRect.right, lRect.bottom );
    }
}

// ��ʾ�����¼�������,���ڴ����ƶ�
LRESULT CALLBACK WndProc( HWND aHwnd, UINT aMsg, WPARAM wParam, LPARAM lParam )
{
    switch( aMsg )
    {
        case WM_CLOSE:
            DestroyWindow( aHwnd );
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_LBUTTONDBLCLK :
            if ( gVideoWindow != NULL )
            {
                LONG lCurrent = 0;
                gVideoWindow->get_FullScreenMode( &lCurrent );
                gVideoWindow->put_FullScreenMode( ( lCurrent == OATRUE ) ? OAFALSE : OATRUE );
            }
            break;

        case WM_SIZE:
            UpdateDisplayPosition();
            break;

        default:
            return DefWindowProc( aHwnd , aMsg, wParam, lParam );
    }

    return 0;
}

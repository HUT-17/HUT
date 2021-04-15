/***************************************************************************************************
* 
* ��Ȩ��Ϣ����Ȩ���� (c) 2016, ���ݺ����������ּ����ɷ����޹�˾, ��������Ȩ��
* 
* �ļ����ƣ�ConvertPixel.cpp
* ժ    Ҫ�����ݸ�ʽת��
*
* ��ǰ�汾��1.0.0.0
* ��    �ߣ���ΰ��
* ��    �ڣ�2017-11-07
* ��    ע���½�
***************************************************************************************************/
#include "stdafx.h"
#include "ConvertPixel.h"

using namespace Halcon;

/************************************************************************
 *  @fn     ConvertToMono8()
 *  @brief  ת��ΪMono8��ʽ����
 *  @param  pHandle                [IN]           ���
 *  @param  pInData                [IN]           Դ����
 *  @param  pOutData               [IN]           ��������ݻ���
 *  @param  nHeight                [IN]           ͼ��߶�
 *  @param  nWidth                 [IN]           ͼ����
 *  @param  nPixelType             [IN]           Դ���ݸ�ʽ
 *  @return �ɹ�������STATUS_OK�����󣬷���STATUS_ERROR 
 ************************************************************************/
Status ConvertToMono8(CMvCamera* m_pcMyCamera, IN OUT unsigned char *pInData, IN OUT unsigned char *pOutData,IN int nHeight,IN int nWidth,
                               MvGvspPixelType nPixelType)
{
    int nRet = STATUS_OK;

    if ( NULL == pInData || NULL == pOutData || NULL == m_pcMyCamera)
    {
        return MV_E_PARAMETER;
    }

    MV_CC_PIXEL_CONVERT_PARAM stPixelConvertParam;

    stPixelConvertParam.pSrcData = pInData;//Դ����
    if (NULL == stPixelConvertParam.pSrcData)
    {
        return STATUS_ERROR;
    }

    stPixelConvertParam.nWidth = nWidth;//ͼ����
    stPixelConvertParam.nHeight = nHeight;//ͼ��߶�
    stPixelConvertParam.enSrcPixelType = nPixelType;//Դ���ݵĸ�ʽ
    stPixelConvertParam.nSrcDataLen = (unsigned int)(nWidth * nHeight * ((((unsigned int)nPixelType) >> 16) & 0x00ff) >> 3);

    stPixelConvertParam.nDstBufferSize = (unsigned int)(nWidth * nHeight * ((((unsigned int)PixelType_Gvsp_RGB8_Packed) >> 16) & 0x00ff) >> 3);
    stPixelConvertParam.pDstBuffer = pOutData;//ת���������
    stPixelConvertParam.enDstPixelType = PixelType_Gvsp_Mono8;
    stPixelConvertParam.nDstBufferSize = nWidth * nHeight * 3;

    nRet = m_pcMyCamera->ConvertPixelType(&stPixelConvertParam);//��ʽת��
    if (STATUS_OK != nRet)
    {
        return STATUS_ERROR;
    }

    return nRet;
}

/************************************************************************
 *  @fn     ConvertToRGB()
 *  @brief  ת��ΪRGB��ʽ����
 *  @param  pHandle                [IN]           ���
 *  @param  pSrc                   [IN]           Դ����
 *  @param  nHeight                [IN]           ͼ��߶�
 *  @param  nWidth                 [IN]           ͼ����
 *  @param  nPixelType             [IN]           Դ���ݸ�ʽ
 *  @param  pDst                   [OUT]          ת��������
 *  @return �ɹ�������STATUS_OK�����󣬷���STATUS_ERROR 
 ************************************************************************/
Status ConvertToRGB(CMvCamera* m_pcMyCamera, unsigned char *pSrc, int nHeight, int nWidth,
                    MvGvspPixelType nPixelType, unsigned char *pDst)
{
    if(NULL == m_pcMyCamera || NULL == pSrc || NULL == pDst)
    {
        return MV_E_PARAMETER;
    }

    MV_CC_PIXEL_CONVERT_PARAM stPixelConvertParam;
    int nRet = STATUS_OK;

    stPixelConvertParam.pSrcData = pSrc;//Դ����
    if (NULL == stPixelConvertParam.pSrcData)
    {
        return STATUS_ERROR;
    }

    stPixelConvertParam.nWidth = nWidth;//ͼ����
    stPixelConvertParam.nHeight = nHeight;//ͼ��߶�
    stPixelConvertParam.enSrcPixelType = nPixelType;//Դ���ݵĸ�ʽ
    stPixelConvertParam.nSrcDataLen = (unsigned int)(nWidth * nHeight * ((((unsigned int)nPixelType) >> 16) & 0x00ff) >> 3);

    stPixelConvertParam.nDstBufferSize = (unsigned int)(nWidth * nHeight * ((((unsigned int)PixelType_Gvsp_RGB8_Packed) >> 16) & 0x00ff) >> 3);
    stPixelConvertParam.pDstBuffer = pDst;//ת���������
    stPixelConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;
    stPixelConvertParam.nDstBufferSize = nWidth * nHeight * 3;

    nRet = m_pcMyCamera->ConvertPixelType(&stPixelConvertParam);//��ʽת��
    if (STATUS_OK != nRet)
    {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

/************************************************************************
 *  @fn     ConvertMono8ToHalcon()
 *  @brief  Mono8ת��ΪHalcon��ʽ����
 *  @param  Hobj                   [OUT]          ת��������Hobject����
 *  @param  nHeight                [IN]           ͼ��߶�
 *  @param  nWidth                 [IN]           ͼ����
 *  @param  pData                  [IN]           Դ����
 *  @return �ɹ�������STATUS_OK�����󣬷���STATUS_ERROR 
 ************************************************************************/
Status ConvertMono8ToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth, unsigned char *pData)
{
    if(NULL == Hobj || NULL == pData)
    {
        return MV_E_PARAMETER;
    }

    gen_image1(Hobj, "byte", nWidth, nHeight, (Hlong)pData);
    return STATUS_OK;
}

/************************************************************************
 *  @fn     ConvertBayer8ToHalcon()
 *  @brief  Bayer8ת��ΪHalcon��ʽ����
 *  @param  Hobj                   [OUT]          ת��������Hobject����
 *  @param  nHeight                [IN]           ͼ��߶�
 *  @param  nWidth                 [IN]           ͼ����
 *  @param  nPixelType             [IN]           Դ���ݸ�ʽ
 *  @param  pData                  [IN]           Դ����
 *  @return �ɹ�������STATUS_OK�����󣬷���STATUS_ERROR 
 ************************************************************************/
Status ConvertBayer8ToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth, MvGvspPixelType nPixelType, unsigned char *pData)
{
    if(NULL == Hobj || NULL == pData)
    {
        return MV_E_PARAMETER;
    }

    gen_image1(Hobj, "byte", nWidth, nHeight, (Hlong)pData);
    cfa_to_rgb(*Hobj, Hobj, "bayer_gb", "bilinear");

    return STATUS_OK;
}

/************************************************************************
 *  @fn     ConvertRGBToHalcon()
 *  @brief  RGBת��ΪHalcon��ʽ����
 *  @param  Hobj                   [OUT]          ת��������Hobject����
 *  @param  nHeight                [IN]           ͼ��߶�
 *  @param  nWidth                 [IN]           ͼ����
 *  @param  pData                  [IN]           Դ����
 *  @param  pDataSeparate          [IN]           �洢������ɫԴ���ݵĻ�����
 *  @return �ɹ�������STATUS_OK�����󣬷���STATUS_ERROR 
 ************************************************************************/
Status ConvertRGBToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth,
                          unsigned char *pData, unsigned char *pDataSeparate)
{
    if(NULL == Hobj || NULL == pData || NULL == pDataSeparate)
    {
        return MV_E_PARAMETER;
    }

    unsigned char *dataRed = pDataSeparate;
    unsigned char *dataGreen = pDataSeparate + (nWidth * nHeight);
    unsigned char *dataBlue = pDataSeparate +(2 * nWidth * nHeight);
    int nSupWidth = (nWidth + 3)& 0xfffffffc;  //��Ȳ���Ϊ4�ı���

    for (int row = 0; row <nHeight; row++)
    {
        unsigned char* ptr = &pData[row * nWidth * 3];
        for (int col = 0; col < nWidth; col++)
        {
            dataRed[row * nSupWidth + col] = ptr[3 * col];
            dataGreen[row * nSupWidth + col] = ptr[3 * col + 1];
            dataBlue[row * nSupWidth + col] = ptr[3 * col + 2];
        }
    }

    gen_image3(Hobj, "byte", nWidth, nHeight, (Hlong)(dataRed), (Hlong)(dataGreen), (Hlong)(dataBlue));

    return STATUS_OK;
}

/************************************************************************
 *  @fn     IsBayer8PixelFormat()
 *  @brief  �ж��Ƿ���Bayer8��ʽ
 *  @param  enType                [IN]            ���ظ�ʽ
 *  @return �ǣ�����true���񣬷���false 
 ************************************************************************/
bool IsBayer8PixelFormat(MvGvspPixelType enType)
{
    switch(enType)
    {
        case PixelType_Gvsp_BayerGR8:
        case PixelType_Gvsp_BayerRG8:
        case PixelType_Gvsp_BayerGB8:
        case PixelType_Gvsp_BayerBG8:
            return true;
        default:
            return false;
    }
}

/************************************************************************
 *  @fn     IsColorPixelFormat()
 *  @brief  �ж��Ƿ��ǲ�ɫ��ʽ
 *  @param  enType                [IN]            ���ظ�ʽ
 *  @return �ǣ�����true���񣬷���false 
 ************************************************************************/
bool IsColorPixelFormat(MvGvspPixelType enType)
{
    switch(enType)
    {
        case PixelType_Gvsp_RGB8_Packed:
        case PixelType_Gvsp_BGR8_Packed:
        case PixelType_Gvsp_RGBA8_Packed:
        case PixelType_Gvsp_BGRA8_Packed:
        case PixelType_Gvsp_YUV422_Packed:
        case PixelType_Gvsp_YUV422_YUYV_Packed:
        case PixelType_Gvsp_BayerGB10:
        case PixelType_Gvsp_BayerGB10_Packed:
        case PixelType_Gvsp_BayerBG10:
        case PixelType_Gvsp_BayerBG10_Packed:
        case PixelType_Gvsp_BayerRG10:
        case PixelType_Gvsp_BayerRG10_Packed:
        case PixelType_Gvsp_BayerGR10:
        case PixelType_Gvsp_BayerGR10_Packed:
        case PixelType_Gvsp_BayerGB12:
        case PixelType_Gvsp_BayerGB12_Packed:
        case PixelType_Gvsp_BayerBG12:
        case PixelType_Gvsp_BayerBG12_Packed:
        case PixelType_Gvsp_BayerRG12:
        case PixelType_Gvsp_BayerRG12_Packed:
        case PixelType_Gvsp_BayerGR12:
        case PixelType_Gvsp_BayerGR12_Packed:
            return true;
        default:
            return false;
    }
}

/************************************************************************
 *  @fn     IsMonoPixelFormat()
 *  @brief  �ж��Ƿ��Ǻڰ׸�ʽ
 *  @param  enType                [IN]            ���ظ�ʽ
 *  @return �ǣ�����true���񣬷���false 
 ************************************************************************/
bool IsMonoPixelFormat(MvGvspPixelType enType)
{
    switch(enType)
    {
        case PixelType_Gvsp_Mono8:
        case PixelType_Gvsp_Mono10:
        case PixelType_Gvsp_Mono10_Packed:
        case PixelType_Gvsp_Mono12:
        case PixelType_Gvsp_Mono12_Packed:
            return true;
        default:
            return false;
    }
}

/************************************************************************
 *  @fn     HalconDisplay(HTuple *hWindow, const Halcon::Hobject &Hobj, HTuple hImageWidth, HTuple hImageHeight)
 *  @brief  Halconͼ����ʾ
 *  @param  hWindow               [IN]        - �������ݻ���
 *  @param  Hobj                  [IN]        - Halcon��ʽ����
 *  @param  hImageWidth           [IN]        - ͼ���
 *  @param  hImageHeight          [IN]        - ͼ���
 *  @return �ɹ�������STATUS_OK��
 ************************************************************************/
Status HalconDisplay(HTuple *hWindow, const Halcon::Hobject &Hobj, const Halcon::HTuple &hImageHeight,const Halcon::HTuple &hImageWidth)
{
    if(NULL == hWindow)
    {
        return MV_E_PARAMETER;
    }
    // ch: ��ʾ || en:display
    set_part(*hWindow, 0, 0, hImageHeight - 1, hImageWidth - 1);
    disp_obj((const Halcon::Hobject)Hobj, *hWindow);

    return STATUS_OK;
}

#include <HalconCpp.h>
#include <HalconCDefs.h>
#include <HProto.h>
#include <HWindow.h>
#include <stdio.h>
#include "MvCamera.h"

/*���������붨��*/
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

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
                            MvGvspPixelType nPixelType);

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
Status ConvertToRGB(CMvCamera* m_pcMyCamera, unsigned char *pSrc, int nHeight, int nWidth, MvGvspPixelType nPixelType, unsigned char *pDst);

/************************************************************************
 *  @fn     ConvertMono8ToHalcon()
 *  @brief  Mono8ת��ΪHalcon��ʽ����
 *  @param  Hobj                   [OUT]          ת��������Hobject����
 *  @param  nHeight                [IN]           ͼ��߶�
 *  @param  nWidth                 [IN]           ͼ����
 *  @param  pData                  [IN]           Դ����
 *  @return �ɹ�������STATUS_OK�����󣬷���STATUS_ERROR 
 ************************************************************************/
Status ConvertMono8ToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth, unsigned char *pData);

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
Status ConvertBayer8ToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth, MvGvspPixelType nPixelType, unsigned char *pData);

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
                          unsigned char *pData, unsigned char *pDataSeparate);

/************************************************************************
 *  @fn     IsBayer8PixelFormat()
 *  @brief  �ж��Ƿ���Bayer8��ʽ
 *  @param  enType                [IN]            ���ظ�ʽ
 *  @return �ǣ�����true���񣬷���false 
 ************************************************************************/
bool IsBayer8PixelFormat(MvGvspPixelType enType);

/************************************************************************
 *  @fn     IsColorPixelFormat()
 *  @brief  �ж��Ƿ��ǲ�ɫ��ʽ
 *  @param  enType                [IN]            ���ظ�ʽ
 *  @return �ǣ�����true���񣬷���false 
 ************************************************************************/
bool IsColorPixelFormat(MvGvspPixelType enType);

/************************************************************************
 *  @fn     IsMonoPixelFormat()
 *  @brief  �ж��Ƿ��Ǻڰ׸�ʽ
 *  @param  enType                [IN]            ���ظ�ʽ
 *  @return �ǣ�����true���񣬷���false 
 ************************************************************************/
bool IsMonoPixelFormat(MvGvspPixelType enType);

/************************************************************************
 *  @fn     HalconDisplay(HTuple *hWindow, const Halcon::Hobject &Hobj, HTuple hImageWidth, HTuple hImageHeight)
 *  @brief  Halconͼ����ʾ
 *  @param  hWindow               [IN]        - �������ݻ���
 *  @param  Hobj                  [IN]        - Halcon��ʽ����
 *  @param  hImageWidth           [IN]        - ͼ���
 *  @param  hImageHeight          [IN]        - ͼ���
 *  @return �ɹ�������STATUS_OK��
 ************************************************************************/
Status HalconDisplay(HTuple *hWindow, const Halcon::Hobject &Hobj, const Halcon::HTuple &hImageWidth, const Halcon::HTuple &hImageHeight);

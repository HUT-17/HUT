#include <HalconCpp.h>
#include <HalconCDefs.h>
#include <HProto.h>
#include <HWindow.h>
#include <stdio.h>
#include "MvCamera.h"

/*函数返回码定义*/
typedef int Status;
#define STATUS_OK               0
#define STATUS_ERROR            -1

using namespace Halcon;

/************************************************************************
 *  @fn     ConvertToMono8()
 *  @brief  转换为Mono8格式数据
 *  @param  pHandle                [IN]           句柄
 *  @param  pInData                [IN]           源数据
 *  @param  pOutData               [IN]           待输出数据缓存
 *  @param  nHeight                [IN]           图像高度
 *  @param  nWidth                 [IN]           图像宽度
 *  @param  nPixelType             [IN]           源数据格式
 *  @return 成功，返回STATUS_OK；错误，返回STATUS_ERROR 
 ************************************************************************/
Status ConvertToMono8(CMvCamera* m_pcMyCamera, IN OUT unsigned char *pInData, IN OUT unsigned char *pOutData,IN int nHeight,IN int nWidth,
                            MvGvspPixelType nPixelType);

/************************************************************************
 *  @fn     ConvertToRGB()
 *  @brief  转换为RGB格式数据
 *  @param  pHandle                [IN]           句柄
 *  @param  pSrc                   [IN]           源数据
 *  @param  nHeight                [IN]           图像高度
 *  @param  nWidth                 [IN]           图像宽度
 *  @param  nPixelType             [IN]           源数据格式
 *  @param  pDst                   [OUT]          转换后数据
 *  @return 成功，返回STATUS_OK；错误，返回STATUS_ERROR 
 ************************************************************************/
Status ConvertToRGB(CMvCamera* m_pcMyCamera, unsigned char *pSrc, int nHeight, int nWidth, MvGvspPixelType nPixelType, unsigned char *pDst);

/************************************************************************
 *  @fn     ConvertMono8ToHalcon()
 *  @brief  Mono8转换为Halcon格式数据
 *  @param  Hobj                   [OUT]          转换后的输出Hobject数据
 *  @param  nHeight                [IN]           图像高度
 *  @param  nWidth                 [IN]           图像宽度
 *  @param  pData                  [IN]           源数据
 *  @return 成功，返回STATUS_OK；错误，返回STATUS_ERROR 
 ************************************************************************/
Status ConvertMono8ToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth, unsigned char *pData);

/************************************************************************
 *  @fn     ConvertBayer8ToHalcon()
 *  @brief  Bayer8转换为Halcon格式数据
 *  @param  Hobj                   [OUT]          转换后的输出Hobject数据
 *  @param  nHeight                [IN]           图像高度
 *  @param  nWidth                 [IN]           图像宽度
 *  @param  nPixelType             [IN]           源数据格式
 *  @param  pData                  [IN]           源数据
 *  @return 成功，返回STATUS_OK；错误，返回STATUS_ERROR 
 ************************************************************************/
Status ConvertBayer8ToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth, MvGvspPixelType nPixelType, unsigned char *pData);

/************************************************************************
 *  @fn     ConvertRGBToHalcon()
 *  @brief  RGB转换为Halcon格式数据
 *  @param  Hobj                   [OUT]          转换后的输出Hobject数据
 *  @param  nHeight                [IN]           图像高度
 *  @param  nWidth                 [IN]           图像宽度
 *  @param  pData                  [IN]           源数据
 *  @param  pDataSeparate          [IN]           存储红绿蓝色源数据的缓冲区
 *  @return 成功，返回STATUS_OK；错误，返回STATUS_ERROR 
 ************************************************************************/
Status ConvertRGBToHalcon(Halcon::Hobject *Hobj, int nHeight, int nWidth,
                          unsigned char *pData, unsigned char *pDataSeparate);

/************************************************************************
 *  @fn     IsBayer8PixelFormat()
 *  @brief  判断是否是Bayer8格式
 *  @param  enType                [IN]            像素格式
 *  @return 是，返回true；否，返回false 
 ************************************************************************/
bool IsBayer8PixelFormat(MvGvspPixelType enType);

/************************************************************************
 *  @fn     IsColorPixelFormat()
 *  @brief  判断是否是彩色格式
 *  @param  enType                [IN]            像素格式
 *  @return 是，返回true；否，返回false 
 ************************************************************************/
bool IsColorPixelFormat(MvGvspPixelType enType);

/************************************************************************
 *  @fn     IsMonoPixelFormat()
 *  @brief  判断是否是黑白格式
 *  @param  enType                [IN]            像素格式
 *  @return 是，返回true；否，返回false 
 ************************************************************************/
bool IsMonoPixelFormat(MvGvspPixelType enType);

/************************************************************************
 *  @fn     HalconDisplay(HTuple *hWindow, const Halcon::Hobject &Hobj, HTuple hImageWidth, HTuple hImageHeight)
 *  @brief  Halcon图像显示
 *  @param  hWindow               [IN]        - 输入数据缓存
 *  @param  Hobj                  [IN]        - Halcon格式数据
 *  @param  hImageWidth           [IN]        - 图像宽
 *  @param  hImageHeight          [IN]        - 图像高
 *  @return 成功，返回STATUS_OK；
 ************************************************************************/
Status HalconDisplay(HTuple *hWindow, const Halcon::Hobject &Hobj, const Halcon::HTuple &hImageWidth, const Halcon::HTuple &hImageHeight);


// HalconGrabImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHalconGrabImageApp:
// �йش����ʵ�֣������ HalconGrabImage.cpp
//

class CHalconGrabImageApp : public CWinAppEx
{
public:
	CHalconGrabImageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHalconGrabImageApp theApp;
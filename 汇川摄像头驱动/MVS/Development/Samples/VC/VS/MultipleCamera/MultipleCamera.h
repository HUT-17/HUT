
// MultipleCamera.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
// 

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMultipleCameraApp:
// �йش����ʵ�֣������ MultipleCamera.cpp
// 

class CMultipleCameraApp : public CWinAppEx
{
public:
	CMultipleCameraApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMultipleCameraApp theApp;
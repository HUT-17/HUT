
// BasicDemoByGenTL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
// 

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBasicDemoByGenTLApp:
// �йش����ʵ�֣������ BasicDemoByGenTL.cpp
// 

class CBasicDemoByGenTLApp : public CWinAppEx
{
public:
	CBasicDemoByGenTLApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBasicDemoByGenTLApp theApp;
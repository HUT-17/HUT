
// ForceIpDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CForceIpDemoApp:
// �йش����ʵ�֣������ ForceIpDemo.cpp
//

class CForceIpDemoApp : public CWinAppEx
{
public:
	CForceIpDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CForceIpDemoApp theApp;
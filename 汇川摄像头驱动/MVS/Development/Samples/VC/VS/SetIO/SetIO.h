
// SetIO.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSetIOApp:
// �йش����ʵ�֣������ SetIO.cpp
//

class CSetIOApp : public CWinAppEx
{
public:
	CSetIOApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSetIOApp theApp;
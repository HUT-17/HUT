
// Raw2Himage_C.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRaw2Himage_CApp:
// �йش����ʵ�֣������ Raw2Himage_C.cpp
//

class CRaw2Himage_CApp : public CWinAppEx
{
public:
	CRaw2Himage_CApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRaw2Himage_CApp theApp;
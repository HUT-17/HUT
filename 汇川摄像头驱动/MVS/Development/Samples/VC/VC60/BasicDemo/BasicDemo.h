// BasicDemo.h : main header file for the BASICDEMO application
//

#if !defined(AFX_BASICDEMO_H__5791CCE0_AAD3_4CAC_9F39_2C3E53B51246__INCLUDED_)
#define AFX_BASICDEMO_H__5791CCE0_AAD3_4CAC_9F39_2C3E53B51246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBasicDemoApp:
// See BasicDemo.cpp for the implementation of this class
//

class CBasicDemoApp : public CWinApp
{
public:
	CBasicDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasicDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBasicDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICDEMO_H__5791CCE0_AAD3_4CAC_9F39_2C3E53B51246__INCLUDED_)

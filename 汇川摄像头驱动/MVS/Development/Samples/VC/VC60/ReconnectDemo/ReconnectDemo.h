// ReconnectDemo.h : main header file for the RECONNECTDEMO application
//

#if !defined(AFX_RECONNECTDEMO_H__DF59FBE9_83F7_4E35_84DA_BA6D41659130__INCLUDED_)
#define AFX_RECONNECTDEMO_H__DF59FBE9_83F7_4E35_84DA_BA6D41659130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CReconnectDemoApp:
// See ReconnectDemo.cpp for the implementation of this class
//

class CReconnectDemoApp : public CWinApp
{
public:
	CReconnectDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReconnectDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CReconnectDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECONNECTDEMO_H__DF59FBE9_83F7_4E35_84DA_BA6D41659130__INCLUDED_)

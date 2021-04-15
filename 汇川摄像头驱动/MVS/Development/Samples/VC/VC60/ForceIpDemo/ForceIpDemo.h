// ForceIpDemo.h : main header file for the FORCEIPDEMO application
//

#if !defined(AFX_FORCEIPDEMO_H__CA6A71E3_9C29_40A0_A676_D1C668B6D78C__INCLUDED_)
#define AFX_FORCEIPDEMO_H__CA6A71E3_9C29_40A0_A676_D1C668B6D78C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CForceIpDemoApp:
// See ForceIpDemo.cpp for the implementation of this class
//

class CForceIpDemoApp : public CWinApp
{
public:
	CForceIpDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForceIpDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CForceIpDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORCEIPDEMO_H__CA6A71E3_9C29_40A0_A676_D1C668B6D78C__INCLUDED_)

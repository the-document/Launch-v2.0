
// Launchv2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLaunchv2App:
// See Launchv2.cpp for the implementation of this class
//

class CLaunchv2App : public CWinApp
{
public:
	CLaunchv2App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLaunchv2App theApp;

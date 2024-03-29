
// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Launchv2.h"
#include "DlgProxy.h"
#include "Launchv2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLaunchv2DlgAutoProxy

IMPLEMENT_DYNCREATE(CLaunchv2DlgAutoProxy, CCmdTarget)

CLaunchv2DlgAutoProxy::CLaunchv2DlgAutoProxy()
{
	EnableAutomation();

	// To keep the application running as long as an automation
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CLaunchv2Dlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CLaunchv2Dlg)))
		{
			m_pDialog = reinterpret_cast<CLaunchv2Dlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CLaunchv2DlgAutoProxy::~CLaunchv2DlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CLaunchv2DlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CLaunchv2DlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLaunchv2DlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ILaunchv2 to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {3726fdb8-acc8-4026-a117-6cbb131a39da}
static const IID IID_ILaunchv2 =
{0x3726fdb8,0xacc8,0x4026,{0xa1,0x17,0x6c,0xbb,0x13,0x1a,0x39,0xda}};

BEGIN_INTERFACE_MAP(CLaunchv2DlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CLaunchv2DlgAutoProxy, IID_ILaunchv2, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {38b7922c-a23a-4b85-8ecf-29ef0cbd4369}
IMPLEMENT_OLECREATE2(CLaunchv2DlgAutoProxy, "Launchv2.Application", 0x38b7922c,0xa23a,0x4b85,0x8e,0xcf,0x29,0xef,0x0c,0xbd,0x43,0x69)


// CLaunchv2DlgAutoProxy message handlers

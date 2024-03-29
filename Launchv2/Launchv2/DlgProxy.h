
// DlgProxy.h: header file
//

#pragma once

class CLaunchv2Dlg;


// CLaunchv2DlgAutoProxy command target

class CLaunchv2DlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CLaunchv2DlgAutoProxy)

	CLaunchv2DlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CLaunchv2Dlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CLaunchv2DlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CLaunchv2DlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


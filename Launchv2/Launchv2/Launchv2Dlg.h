
// Launchv2Dlg.h : header file
//

#pragma once
#include"CExplorer.h"
#include"MyButton.h"
#include"MyText.h"
#include"MySettings.h"
#include"SingleThreadFileDownload.h"


class CLaunchv2DlgAutoProxy;

static int volatile app_closing = 0;

// CLaunchv2Dlg dialog
class CLaunchv2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLaunchv2Dlg);
	friend class CLaunchv2DlgAutoProxy;

// Construction
public:
	CLaunchv2Dlg(CWnd* pParent = nullptr);	// standard constructor
	virtual ~CLaunchv2Dlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAUNCHV2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CLaunchv2DlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	char* game_version;

	MyButton btnStart;
	MyButton btnHome;
	MyButton btnRegister;
	MyButton btnToken;
	MyButton btnGroup;
	MyButton btnExit;
	MyButton btnMinisize;
	MyButton btnClose;

	MyText *percentDownload;
	MyText *percentUnpack;

	CProgressCtrl downloadProgess;
	CProgressCtrl unpackProgess;

	CExplorer BannerWindows;
	CExplorer EventsWindows;
	CExplorer InforWindows;

public:
	void InitButon(MyButton& btn, int IdHover, int IdNomal, int idDisable);
	void SetUpProgessbar(CProgressCtrl &progress, int location_y);
	void SetPercentText(MyText *text, LPCWCHAR display, int y);

	void LoadPathInfo();
	void DowloadNewLaunch();
	void DowloadGameUpdate();

	afx_msg void StartGame();
	afx_msg void OpenHome();
	afx_msg void OpenRegister();
	afx_msg void OpenToken();
	afx_msg void OpenGroup();
	afx_msg void ExitApp();
	afx_msg void HideApp();
	afx_msg void CloseApp();
	afx_msg void Home();
	afx_msg void Facebook();
	afx_msg void YouTube();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	MyButton btnYouTube;
	MyButton btnFacebook;
	MyButton btnHome2;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	BOOL PreTranslateMessage(MSG* pMsg);
};

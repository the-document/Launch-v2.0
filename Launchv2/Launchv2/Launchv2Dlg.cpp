
// Launchv2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launchv2.h"
#include "Launchv2Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLaunchv2Dlg dialog


IMPLEMENT_DYNAMIC(CLaunchv2Dlg, CDialogEx);

CLaunchv2Dlg::CLaunchv2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAUNCHV2_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

CLaunchv2Dlg::~CLaunchv2Dlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to null, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

UINT DownloadPathInfo(LPVOID pParam) {

	CLaunchv2Dlg* dlg = (CLaunchv2Dlg*)pParam;

	SingleThreadFileDownload *download_path_info;

	const size_t cSize = strlen(MySettings::Instance()->FPatchInfo) + 1;
	wchar_t save[MAX_PATH];
	mbstowcs(save, MySettings::Instance()->FPatchInfo, cSize);

	download_path_info = new SingleThreadFileDownload(
		NULL,
		MySettings::Instance()->PatchInfo,
		save);

	boolean result = download_path_info->SetupDownload();
	if (!result) {
		delete download_path_info;
		AfxBeginThread(DownloadPathInfo, dlg);
		return TRUE;
	}

	dlg->downloadProgess.SetPos(0);

	CString text1 = L"Downloading path.infor...";
	OutputDebugString(text1);

	InternetDownloadStatus status = download_path_info->CheckStatusAndReadData();
	if (status.statusCode == StatusCode::TimedOutReadingData)
	{
		AfxBeginThread(DownloadPathInfo, (LPVOID)dlg);
		delete download_path_info;
		return TRUE;
	}

	dlg->downloadProgess.SetPos(status.downloadedPercent);

	CString per;
	per.Format(_T("%d%%\n"), (int)status.downloadedPercent);
	OutputDebugString(per);

	dlg->percentDownload->SetWindowTextW(per);

	int count = 0;
	while (status.downloadedPercent < 100)
	{
		
		status = download_path_info->CheckStatusAndReadData();
		if (status.statusCode == StatusCode::TimedOutReadingData)
		{
			AfxBeginThread(DownloadPathInfo, (LPVOID)dlg);
			delete download_path_info;
			return TRUE;
		}

		int i;
		for (i = dlg->downloadProgess.GetPos(); i < status.downloadedPercent; i++)
			dlg->downloadProgess.SetPos(i);

		per.Format(_T("%d%%\n"), i);
		OutputDebugString(per);

		dlg->percentDownload->SetWindowTextW(per);

		//if click pause
		//download_path_info->TerminateDownload();
	}

	OutputDebugString(L"Downloading Path.info...ok");

	int i;
	for (i = dlg->downloadProgess.GetPos(); i < 11; i++)
		dlg->unpackProgess.SetPos(i);
	dlg->percentUnpack->SetWindowTextW(L"10%");

	delete download_path_info;

	dlg->DowloadNewLaunch();
	return TRUE;
}

UINT DowloadLaunch(LPVOID pParam) {
	CLaunchv2Dlg* dlg = (CLaunchv2Dlg*)pParam;

	SingleThreadFileDownload *download_launch;
	download_launch = new SingleThreadFileDownload(
		NULL,
		MySettings::Instance()->NewLauncher,
		MySettings::Instance()->FPatchLaucher);


	boolean result = download_launch->SetupDownload();
	if (!result) {
		delete download_launch;
		AfxBeginThread(DowloadLaunch, dlg);
		return TRUE;
	}
	dlg->downloadProgess.SetPos(0);

	CString text1 = L"Dowloading new launch...";
	OutputDebugString(text1);

	InternetDownloadStatus status = download_launch->CheckStatusAndReadData();
	if (status.statusCode == StatusCode::TimedOutReadingData)
	{
		AfxBeginThread(DowloadLaunch, (LPVOID)dlg);
		delete download_launch;
		return TRUE;
	}

	dlg->downloadProgess.SetPos(status.downloadedPercent);

	CString per;
	per.Format(_T("%d%%\n"), (int)status.downloadedPercent);
	//OutputDebugString(per);

	dlg->percentDownload->SetWindowTextW(per);

	int pre_per = 0;
	while (status.downloadedPercent < 100)
	{
		
		status = download_launch->CheckStatusAndReadData();
		if (status.statusCode == StatusCode::TimedOutReadingData)
		{
			AfxBeginThread(DowloadLaunch, (LPVOID)dlg);
			delete download_launch;
			return TRUE;
		}

		int i;
		for (i = dlg->downloadProgess.GetPos(); i < status.downloadedPercent; i++)
			dlg->downloadProgess.SetPos(i);

		//if click pause
		//download_path_info->TerminateDownload();
		if (pre_per != i) {
			per.Format(_T("%d%%\n"), i);
			OutputDebugString(per);
			pre_per = i;
			dlg->percentDownload->SetWindowTextW(per);
		}


		if (app_closing) {
			download_launch->TerminateDownload();
			delete download_launch;
			app_closing = 0;
			dlg->SetTimer(500, 100, NULL);
			return TRUE;
		}
	}
	int i;
	for (i = dlg->downloadProgess.GetPos(); i < 51; i++)
		dlg->unpackProgess.SetPos(i);
	dlg->percentUnpack->SetWindowTextW(L"50%");

	//unpack here
	//===========================//
	download_launch->UnpackDownloadedFile(MySettings::Instance()->FPatchLaucher, L"../");

	delete download_launch;

	//update infor version launch
	char v[MAX_PATH];
	char inBuf[MAX_PATH];
	FILE *f;
	f = fopen(MySettings::Instance()->FLaunchVersion, "w");
	GetPrivateProfileStringA("Version", "NewLaunch", "", inBuf, MAX_PATH, MySettings::Instance()->FPatchInfo);
	fputs(inBuf, f);
	fclose(f);


	dlg->DowloadGameUpdate();

	return TRUE;
}

UINT GameUpdate(LPVOID pParam) {

	CLaunchv2Dlg* dlg = (CLaunchv2Dlg*)pParam;

	char inBuf[MAX_PATH];
	GetPrivateProfileSectionNamesA(inBuf, MAX_PATH + MAX_PATH, MySettings::Instance()->FPatchInfo);
	char* bufCHAR;
	bufCHAR = inBuf;
	CString str;
	CString cstring;
	bool canUpdate = false;

	while (*bufCHAR != 0x00)
	{
		bufCHAR = bufCHAR + strlen(bufCHAR) + 1;
		if (*bufCHAR != 0x00)
		{
			str = bufCHAR;
			CT2A ascii(str);
			GetPrivateProfileStringA(ascii.m_psz, "VersionFrom", NULL, inBuf, MAX_PATH, MySettings::Instance()->FPatchInfo);
			if (strcmp(dlg->game_version, inBuf) == 0) {
				canUpdate = true;
				GetPrivateProfileStringA(ascii.m_psz, "URL_1", NULL, inBuf, MAX_PATH, MySettings::Instance()->FPatchInfo);


				const size_t cSize = strlen(inBuf) + 1;
				mbstowcs(MySettings::Instance()->URL1, inBuf, cSize);


				SingleThreadFileDownload *download_game;

				_bstr_t b(MySettings::Instance()->URL1);
				char* url = b;
				char *file_name = strrchr(url, '/') + 1;
				char save_path[MAX_PATH] = "../Patch/";
				strcat(save_path, file_name);
				const size_t cSize2 = strlen(save_path) + 1;

				wchar_t* save = new wchar_t[260];
				mbstowcs(save, save_path, cSize2);

				download_game = new SingleThreadFileDownload(
					NULL,
					MySettings::Instance()->URL1,
					save);


				boolean result = download_game->SetupDownload();
				if (!result) {
					delete download_game;
					AfxBeginThread(GameUpdate, dlg);
					return TRUE;
				}

				dlg->downloadProgess.SetPos(0);
				dlg->unpackProgess.SetPos(10);

				CString text1 = L"Downloading game update...";

				const size_t cSize3 = strlen(save_path) + 1;
				wchar_t* char_file_name = new wchar_t[260];
				mbstowcs(char_file_name, file_name, cSize3);
				text1.Append(char_file_name);
				OutputDebugString(text1);

				InternetDownloadStatus status = download_game->CheckStatusAndReadData();
				if (status.statusCode == StatusCode::TimedOutReadingData)
				{
					delete download_game;
					AfxBeginThread(GameUpdate, (LPVOID)dlg);
					return TRUE;
				}

				dlg->downloadProgess.SetPos(status.downloadedPercent);

				CString per;
				per.Format(_T("%d%%\n"), (int)status.downloadedPercent);
				OutputDebugString(per);
				dlg->percentDownload->SetWindowTextW(per);

				int pre_per = 0;
				while (status.downloadedPercent < 100)
				{
					
					status = download_game->CheckStatusAndReadData();
					if (status.statusCode == StatusCode::TimedOutReadingData)
					{
						delete download_game;
						AfxBeginThread(GameUpdate, (LPVOID)dlg);
						return TRUE;
					}

					int i;
					for (i = dlg->downloadProgess.GetPos(); i < status.downloadedPercent; i++)
						dlg->downloadProgess.SetPos(i);

					if (pre_per != i) {
						per.Format(_T("%d%%\n"), i);
						OutputDebugString(per);
						pre_per = i;
						dlg->percentDownload->SetWindowTextW(per);
					}


					//if click pause
					//download_path_info->TerminateDownload();


					if (app_closing) {
						download_game->TerminateDownload();
						delete download_game;
						app_closing = 0;
						dlg->SetTimer(500, 100, NULL);
						return TRUE;
					}
				}

				text1.Append(L"  ok.");
				OutputDebugString(text1);

				int current_percent = dlg->downloadProgess.GetPos();
				if (current_percent + 20 < 100)
				{
					int i;
					for (i = current_percent; i < current_percent + 20; i++)
						dlg->unpackProgess.SetPos(i);
					CString per_str;
					per_str.Format(_T("%i%%"), current_percent + 20);
					dlg->percentUnpack->SetWindowTextW(per_str);
				}


				//unpack here
				//===========================//
				download_game->UnpackDownloadedFile(save, MySettings::Instance()->PathFolder);

				delete download_game;

				break;
			}
		}
	}
	if (!canUpdate)
	{
		MessageBox(0, L"Can't download from this version", L"Error version", MB_OK);
		//dlg->SetStatusText(dlg->textLoadGame, L"Error, Can't download from this version", RGB(250, 2, 2));
		return TRUE;
	}

	int i;
	for (i = dlg->unpackProgess.GetPos(); i < 101; i++)
		dlg->unpackProgess.SetPos(i);
	dlg->percentUnpack->SetWindowTextW(L"100%");

	//save infor version game
	char v[MAX_PATH];
	char inBuf2[MAX_PATH];
	FILE *f;
	f = fopen(MySettings::Instance()->FVersion, "w");
	GetPrivateProfileStringA("Version", "Latest", "", inBuf2, MAX_PATH, MySettings::Instance()->FPatchInfo);
	fputs(inBuf2, f);
	fclose(f);

	return TRUE;
}

void CLaunchv2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, BannerWindows);
	DDX_Control(pDX, IDC_BUTTON1, btnStart);
	DDX_Control(pDX, IDC_BUTTON2, btnHome);
	DDX_Control(pDX, IDC_BUTTON3, btnRegister);
	DDX_Control(pDX, IDC_BUTTON4, btnToken);
	DDX_Control(pDX, IDC_BUTTON5, btnGroup);
	DDX_Control(pDX, IDC_BUTTON6, btnExit);
	DDX_Control(pDX, IDC_BUTTON7, btnMinisize);
	DDX_Control(pDX, IDC_BUTTON8, btnClose);
	DDX_Control(pDX, IDC_PROGRESS1, downloadProgess);
	DDX_Control(pDX, IDC_PROGRESS2, unpackProgess);
	DDX_Control(pDX, IDC_EXPLORER2, EventsWindows);
	DDX_Control(pDX, IDC_EXPLORER3, InforWindows);
	DDX_Control(pDX, IDC_BUTTON11, btnYouTube);
	DDX_Control(pDX, IDC_BUTTON10, btnFacebook);
	DDX_Control(pDX, IDC_BUTTON9, btnHome2);
}

BEGIN_MESSAGE_MAP(CLaunchv2Dlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CLaunchv2Dlg::StartGame)
	ON_BN_CLICKED(IDC_BUTTON2, &CLaunchv2Dlg::OpenHome)
	ON_BN_CLICKED(IDC_BUTTON3, &CLaunchv2Dlg::OpenRegister)
	ON_BN_CLICKED(IDC_BUTTON4, &CLaunchv2Dlg::OpenToken)
	ON_BN_CLICKED(IDC_BUTTON5, &CLaunchv2Dlg::OpenGroup)
	ON_BN_CLICKED(IDC_BUTTON6, &CLaunchv2Dlg::ExitApp)
	ON_BN_CLICKED(IDC_BUTTON7, &CLaunchv2Dlg::HideApp)
	ON_BN_CLICKED(IDC_BUTTON8, &CLaunchv2Dlg::CloseApp)
	ON_BN_CLICKED(IDC_BUTTON9, &CLaunchv2Dlg::Home)
	ON_BN_CLICKED(IDC_BUTTON10, &CLaunchv2Dlg::Facebook)
	ON_BN_CLICKED(IDC_BUTTON11, &CLaunchv2Dlg::YouTube)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CLaunchv2Dlg message handlers

BOOL CLaunchv2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rc;
	GetWindowRect(&rc); // getting dialog coordinates
	MoveWindow(rc.left, rc.top,1000, 674);

	// TODO: Add extra initialization here
	this->game_version = new char[260];
	this->SetBackgroundImage(IDB_BITMAP1);
	

	this->BannerWindows.SetWindowPos(NULL, 401, 247, 316, 140, SWP_NOZORDER | SWP_SHOWWINDOW);
	this->BannerWindows.Navigate(MySettings::Instance()->Banner, NULL, NULL, NULL, NULL);
	this->EventsWindows.SetWindowPos(NULL, 396, 396, 322, 85, SWP_NOZORDER | SWP_SHOWWINDOW);
	this->EventsWindows.Navigate(MySettings::Instance()->News, NULL, NULL, NULL, NULL);
	this->InforWindows.SetWindowPos(NULL, 728, 249, 194, 267, SWP_NOZORDER | SWP_SHOWWINDOW);
	this->InforWindows.Navigate(MySettings::Instance()->Info, NULL, NULL, NULL, NULL);

	//make transparent
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	SetLayeredWindowAttributes(RGB(255, 0, 255), 0, LWA_COLORKEY);
	

	//set progessbar
	this->SetUpProgessbar(this->downloadProgess, 497);
	this->SetUpProgessbar(this->unpackProgess,516);
	
	//setting button
	InitButon(this->btnMinisize, IDB_BITMAP17, IDB_BITMAP18, IDB_BITMAP18);
	this->btnMinisize.SetWindowPos(NULL, 848, 230, 22, 21, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnClose, IDB_BITMAP15, IDB_BITMAP16, IDB_BITMAP15);
	this->btnClose.SetWindowPos(NULL, 870, 230, 22, 21, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnStart, IDB_BITMAP2, IDB_BITMAP4, IDB_BITMAP3);
	this->btnStart.SetWindowPos(NULL, 726, 512, 170, 95, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnHome, IDB_BITMAP5, IDB_BITMAP6, IDB_BITMAP5);
	this->btnHome.SetWindowPos(NULL, 530, 192, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnRegister, IDB_BITMAP8, IDB_BITMAP7, IDB_BITMAP8);
	this->btnRegister.SetWindowPos(NULL, 611, 192, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnToken, IDB_BITMAP10, IDB_BITMAP9, IDB_BITMAP10);
	this->btnToken.SetWindowPos(NULL, 698, 192, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnGroup, IDB_BITMAP11, IDB_BITMAP12, IDB_BITMAP11);
	this->btnGroup.SetWindowPos(NULL, 783, 192, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnExit, IDB_BITMAP14, IDB_BITMAP13, IDB_BITMAP14);
	this->btnExit.SetWindowPos(NULL, 867, 192, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);

	InitButon(this->btnHome2, IDB_BITMAP19, IDB_BITMAP19, IDB_BITMAP19);
	this->btnHome2.SetWindowPos(NULL, 286, 619, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);
	InitButon(this->btnFacebook, IDB_BITMAP19, IDB_BITMAP19, IDB_BITMAP19);
	this->btnFacebook.SetWindowPos(NULL, 309, 619, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);
	InitButon(this->btnYouTube, IDB_BITMAP19, IDB_BITMAP19, IDB_BITMAP19);
	this->btnYouTube.SetWindowPos(NULL, 332, 619, 70, 29, SWP_NOZORDER | SWP_SHOWWINDOW);

	//percent dowload text
	this->percentDownload = new MyText();
	this->percentUnpack = new MyText();
	SetPercentText(this->percentDownload, L"5%", 498);
	SetPercentText(this->percentUnpack, L"5%", 518);

	//check and update
	LoadPathInfo();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLaunchv2Dlg::OnPaint()
{
	CPaintDC dc(this);

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLaunchv2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CLaunchv2Dlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CLaunchv2Dlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CLaunchv2Dlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

void CLaunchv2Dlg::InitButon(MyButton & btn, int IdHover, int IdNomal, int idDisable)
{
	btn.SetHoverBitmap(IdHover);
	btn.SetNomalBitmap(IdNomal);
	btn.LoadBitmaps(IdNomal, IdNomal, 0, IDB_BITMAP3);

	BITMAP bmap;
	CBitmap cbitmap;
	cbitmap.LoadBitmap(IdNomal);
	cbitmap.GetBitmap(&bmap);
}

void CLaunchv2Dlg::SetUpProgessbar(CProgressCtrl &progress, int location_y)
{
	SetWindowTheme(progress.m_hWnd, L"", L"");
	progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(255, 210, 14));
	progress.SetPos(5);
	progress.SetWindowPos(NULL, 417, location_y, 272, 15, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void CLaunchv2Dlg::SetPercentText(MyText * text, LPCWCHAR display, int y)
{
	text->SetTextColor(RGB(255, 3, 36));
	text->Create(display, WS_VISIBLE,
		CRect(691, y, 35, text->height_line), this);
	text->SetWindowPos(&wndTopMost, 0, 0, 35, text->height_line, SWP_NOMOVE | SWP_NOZORDER);
}

void CLaunchv2Dlg::LoadPathInfo()
{
	AfxBeginThread(DownloadPathInfo, (LPVOID)this);
}

void CLaunchv2Dlg::DowloadNewLaunch()
{
	//Get version from txt file in LoadPathInfo and check to update launch
	char v[MAX_PATH];
	char inBuf[MAX_PATH];
	FILE *f;
	f = fopen(MySettings::Instance()->FLaunchVersion, "r");
	fgets(v, sizeof(v), f);
	fclose(f);
	GetPrivateProfileStringA("Version", "NewLaunch", "", inBuf, MAX_PATH, MySettings::Instance()->FPatchInfo);
	CString ver = CString(inBuf);
	CString textShow = CString("[Launcher] Version:");
	textShow.Append(ver);
	//SetInfoText(this->textVersionLaunch, textShow, RGB(255, 242, 0));
	OutputDebugString(textShow);

	if ((strlen(v) <= 2) || (strcmp(v, inBuf) < 0)) {

		AfxBeginThread(DowloadLaunch, (LPVOID)this);
	}
	else
	{
		//this->SetStatusText(this->textLoadLauncher, L"Downloading new launch...ok", RGB(123, 252, 126));;
		OutputDebugString(L"Downloading new launch...ok");
		DowloadGameUpdate();
	}
}

void CLaunchv2Dlg::DowloadGameUpdate()
{
	//get lastest version from pathInfo
	MySettings::Instance()->FPatchInfo;
	CString cstring;
	char v[MAX_PATH];
	char inBuf[MAX_PATH];
	FILE* f;
	f = fopen(MySettings::Instance()->FVersion, "r");
	//fgets(v, sizeof(v), f);
	fgets(this->game_version, sizeof(v), f);

	fclose(f);
	GetPrivateProfileStringA("Version", "Latest", "", inBuf, MAX_PATH, MySettings::Instance()->FPatchInfo);

	CString ver = CString(inBuf);
	CString textShow = CString("[Game] Version:");
	textShow.Append(ver);
	OutputDebugString(textShow);
	//SetInfoText(this->textVersionGame, textShow, RGB(255, 242, 0));

	if (strcmp(this->game_version, inBuf) == 0) {
		OutputDebugString(L"don't need update game");
		//this->SetStatusText(this->textLoadGame, L"Game is newest version", RGB(123, 252, 126));
		this->btnStart.EnableWindow(true);
		this->unpackProgess.SetPos(100);
		this->percentUnpack->SetWindowTextW(L"100%");
		
	}
	else if (strcmp(this->game_version, inBuf) > 0) {
		//this->SetStatusText(this->textLoadGame, L"Error with curren version", RGB(250, 2, 2));
		OutputDebugString(L"Error with curren version");
		this->btnStart.EnableWindow(true);
		this->unpackProgess.SetPos(100);
		
		this->percentUnpack->SetWindowTextW(L"100%");
	}
	else {
		//system("taskkill /f /im Game.exe");
		ShellExecute(NULL, L"open", L"C:\\WINDOWS\\system32\\cmd.exe", L"taskkill /f /im Game.exe", NULL, SW_HIDE);
		this->btnStart.EnableWindow(false);

		//download all version needed
		AfxBeginThread(GameUpdate, (LPVOID)this);
		//WaitForSingleObject(thread_download_game->m_hThread, INFINITE);
	}
}

BOOL CLaunchv2Dlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CLaunchv2Dlg::StartGame()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, TEXT("Open"), TEXT("Game.exe"), TEXT(" -fl"), NULL, SW_SHOW);
}

void CLaunchv2Dlg::OpenHome()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->homeUrl, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::OpenRegister()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->registerUrl, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::OpenToken()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->tokenUrl, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::OpenGroup()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->groupUrl, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::ExitApp()
{
	// TODO: Add your control notification handler code here
	app_closing = 1;
	SetTimer(500, 600, NULL);
}

void CLaunchv2Dlg::HideApp()
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_MINIMIZE);
}

void CLaunchv2Dlg::CloseApp()
{
	// TODO: Add your control notification handler code here
	app_closing = 1;
	SetTimer(500, 600, NULL);
}

void CLaunchv2Dlg::Home()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->Home, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::Facebook()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->Facebook, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::YouTube()
{
	// TODO: Add your control notification handler code here
	ShellExecute(0, 0, MySettings::Instance()->YouTube, 0, 0, SW_SHOW);
}

void CLaunchv2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 500) {
		OutputDebugString(L"\nClosing...\n\n");
		CLaunchv2Dlg::OnCancel();
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CLaunchv2Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect clientRect;

	GetClientRect(&clientRect);
	pDC->FillSolidRect(clientRect, RGB(255, 0, 255));  // paint background in magenta
	return CDialogEx::OnEraseBkgnd(pDC);
}

BOOL CLaunchv2Dlg::PreTranslateMessage(MSG * pMsg)
{
	//The code starts here
	static bool mouse_down = false;
	static CRect MainRect;
	static HCURSOR cursor; // use this if you want the 
	  // “dragging” cursor different from the normal one
	static CPoint point;
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
		//save current dialog’s rectangle
		GetWindowRect(&MainRect);
		//save current cursor coordinate
		point = pMsg->pt;
		ScreenToClient(&point);

		//change the sign
		mouse_down = true;
		cursor = ::AfxGetApp()->LoadCursor(IDC_HAND);
		break;
	case WM_LBUTTONUP:
		//stop the sign
		mouse_down = false;
		//gimme a standard cursor now!!
		cursor = ::AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		break;
	case WM_MOUSEMOVE:
		cursor = ::AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		if (mouse_down)
		{
			//if here, then the mouse is dragging
			cursor = ::AfxGetApp()->LoadCursor(IDC_HAND);
			//finally, move the window
			MoveWindow(pMsg->pt.x - point.x,
				//count the relative position
				pMsg->pt.y - point.y,
				MainRect.Width(),
				//if the width doesn’t change 
				MainRect.Height(),
				//if the height doesn’t change
				TRUE);
		}
	}
	SetCursor(cursor);
	//The code ends here
	return CDialog::PreTranslateMessage(pMsg);
}
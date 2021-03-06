
// vrmfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "vrmfc.h"
#include "vrmfcDlg.h"
#include "config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CvrmfcApp

BEGIN_MESSAGE_MAP(CvrmfcApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CvrmfcApp construction

CvrmfcApp::CvrmfcApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CvrmfcApp object

CvrmfcApp theApp;


// CvrmfcApp initialization

BOOL CvrmfcApp::InitInstance()
{
	// init logger
	{
		auto path = get_exe_path_a() + "/log";
		CreateDirectoryA(path.c_str(), nullptr);
		path += "/vrmfc";
		jlib::init_logger(path);
		JLOG_INFO("vrmfc start running");
		JLOG_INFO("before }");
	}
	JLOG_INFO("before AUTO_LOG_FUNCTION");
	AUTO_LOG_FUNCTION;

	SetThreadPriority(THREAD_PRIORITY_HIGHEST);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	auto cfg = config::get_instance();

	if (cfg->get_lang() == "en") {
		SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
		SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	} else {
		SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT));
		SetThreadUILanguage(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
	}

	CvrmfcDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

CString tr(UINT uid)
{
	CString s;
	if (s.LoadStringW(uid)) {
		return s;
	}

	s.Format(L"String resource %d not found", uid);
	return s;
}

std::wstring trw(UINT uid)
{
	return std::wstring(tr(uid).GetBuffer());
}

std::string tra(UINT uid)
{
	return utf8::w2a(trw(uid));
}

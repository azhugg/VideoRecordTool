#include "stdafx.h"
#include "DuiVideoPlayer.h"
#include "vrmfcDlg.h"

using namespace cv;

namespace {
#define WM_USER_PLAYING         (WM_USER + 1)     // ��ʼ�����ļ�
#define WM_USER_POS_CHANGED     (WM_USER + 2)     // �ļ�����λ�øı�
#define WM_USER_END_REACHED     (WM_USER + 3)     // �������

void CallbackPlayer(void *data, UINT uMsg)
{
	CAVPlayer *pAVPlayer = (CAVPlayer *)data;
	if (pAVPlayer) {
		HWND hWnd = pAVPlayer->GetHWND();
		if (::IsWindow(hWnd) && ::IsWindow(::GetParent(hWnd))) {
			::PostMessage(::GetParent(hWnd)/*hWnd*/, uMsg, (WPARAM)data, 0);
		}
	}
}

void CallbackPlaying(void *data)
{
	CallbackPlayer(data, WM_USER_PLAYING);
}

void CallbackPosChanged(void *data)
{
	CallbackPlayer(data, WM_USER_POS_CHANGED);
}

void CallbackEndReached(void *data)
{
	AUTO_LOG_FUNCTION;
	CallbackPlayer(data, WM_USER_END_REACHED);
}
};


CDuiVideoPlayer::CDuiVideoPlayer(const wchar_t* xml)
	: CXMLWnd(xml)
{
}


CDuiVideoPlayer::~CDuiVideoPlayer()
{
}

void CDuiVideoPlayer::InitWindow()
{
	CenterWindow();

	/*cap_.open(video_path_);
	if (cap_.isOpened()) {
		double fps = cap_.get(CAP_PROP_FPS);
		int gap = static_cast<int>(1000.0 / fps);
		SetTimer(m_hWnd, 1, gap, nullptr);
	}*/

	CWndUI *pWnd = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("wndMedia")));
	if (pWnd) {
		player_.SetHWND(pWnd->GetHWND());
		player_.SetCallbackPlaying(CallbackPlaying);
		player_.SetCallbackPosChanged(CallbackPosChanged);
		player_.SetCallbackEndReached(CallbackEndReached);
	} else {
		JLOG_ERRO("CDuiVideoPlayer::InitWindow() cannot find wndMedia!");
	}
}

void CDuiVideoPlayer::Notify(DuiLib::TNotifyUI & msg)
{
	__super::Notify(msg);
}

LRESULT CDuiVideoPlayer::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_TIMER && wParam == 1) {
		
	} else if (uMsg == WM_USER_END_REACHED) {
		OnEndReached(reinterpret_cast<void*>(wParam));

	} else if (uMsg == WM_LBUTTONUP ) {
		
	} else if (uMsg == WM_PARENTNOTIFY) {
		if (wParam == WM_LBUTTONDOWN) {
			auto maindlg = static_cast<CvrmfcDlg*>(AfxGetApp()->GetMainWnd()); assert(maindlg);
			if (maindlg) {
				if (maindlg->do_video_view_mode_show_or_hide_tools(show_tip_)) {
					show_tip_ = !show_tip_;
				}
			}
		}
	} else if (uMsg == WM_USER_PLAYING) {
		JLOG_INFO("CDuiVideoPlayer::HandleMessage WM_USER_PLAYING {:d} {:d}", wParam, lParam);

	} else if (uMsg == WM_USER_POS_CHANGED) {
		//JLOG_INFO("CDuiVideoPlayer::HandleMessage WM_USER_POS_CHANGED {:d} {:d}", wParam, lParam);
		struct tm   tmTotal, tmCurrent;
		time_t      timeTotal = player_.GetTotalTime() / 1000;
		time_t      timeCurrent = player_.GetTime() / 1000;
		TCHAR       szTotal[MAX_PATH], szCurrent[MAX_PATH];
		gmtime_s(&tmTotal, &timeTotal);
		gmtime_s(&tmCurrent, &timeCurrent);
		_tcsftime(szTotal, MAX_PATH, _T("%X"), &tmTotal);
		_tcsftime(szCurrent, MAX_PATH, _T("%X"), &tmCurrent);
		std::wstring cur(szCurrent);
		std::wstring total(szTotal);

		auto maindlg = static_cast<CvrmfcDlg*>(AfxGetApp()->GetMainWnd()); assert(maindlg);
		if (maindlg) {
			maindlg->do_video_view_mode_pos_changed(cur, total, player_.GetPos());
		}
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CDuiVideoPlayer::OnClick(TNotifyUI & msg)
{
	__super::OnClick(msg);
}

CControlUI * CDuiVideoPlayer::CreateControl(LPCTSTR pstrClassName)
{
	CDuiString     strXML;
	CDialogBuilder builder;

	if (_tcsicmp(pstrClassName, _T("WndMediaDisplay")) == 0) {
		CWndUI *pUI = new CWndUI();
		HWND hWnd = CreateWindowW(_T("#32770"), _T("WndMediaDisplay"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), (HMENU)0, NULL, NULL);
		pUI->Attach(hWnd);
		return pUI;
	}

	if (!strXML.IsEmpty()) {
		CControlUI* pUI = builder.Create(strXML.GetData(), NULL, NULL, &m_PaintManager, NULL); // ������봫��m_PaintManager����Ȼ��XML����ʹ��Ĭ�Ϲ���������Ϣ��
		return pUI;
	}

	return NULL;
}

void CDuiVideoPlayer::OnEndReached(void *)
{
	player_.Stop();
	CWndUI *pWnd = static_cast<CWndUI*>(m_PaintManager.FindControl(_T("wndMedia")));
	if (pWnd) {
		pWnd->SetBkColor(0xFFABCDDC);
	}


}

bool CDuiVideoPlayer::play()
{
	return player_.Play(video_path_);
}

bool CDuiVideoPlayer::pause()
{
	player_.Pause();
	return true;
}

bool CDuiVideoPlayer::stop()
{
	player_.Stop();
	return true;
}

















DUI_BEGIN_MESSAGE_MAP(CDuiVideoPlayer, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

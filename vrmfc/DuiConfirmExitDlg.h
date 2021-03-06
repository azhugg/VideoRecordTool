#pragma once
#include "duilib.h"

class CDuiConfirmExitDlg : public CXMLWnd
{
public:
	explicit CDuiConfirmExitDlg(const wchar_t* xmlpath);
	virtual ~CDuiConfirmExitDlg();

	DUI_DECLARE_MESSAGE_MAP();
	virtual void InitWindow() override;
	virtual LPCTSTR GetWindowClassName() const override { return L"CDuiConfirmExitDlg"; }
	virtual void Notify(DuiLib::TNotifyUI& msg) override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnClick(TNotifyUI& msg) override;

public:
	bool confirmed_ = false;
};

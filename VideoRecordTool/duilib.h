#pragma once
#include "../DuiLib/UIlib.h"
using namespace DuiLib;


#ifdef _DEBUG
#   pragma comment(lib, "../Debug/DuiLib_Debug.lib")
#else
#   pragma comment(lib, "../Release/DuiLib_Release.lib")
#endif


// ��XML���ɽ���Ĵ��ڻ���
class CXMLWnd : public WindowImplBase
{
public:
    explicit CXMLWnd(LPCTSTR pszXMLPath) 
        : m_strXMLPath(pszXMLPath){}

public:
    virtual LPCTSTR GetWindowClassName() const
    {
        return _T("XMLWnd");
    }

    virtual CDuiString GetSkinFile()
    {
        return m_strXMLPath;
    }

    virtual CDuiString GetSkinFolder()
    {
        return _T("");
    }

protected:
    CDuiString m_strXMLPath;
};


// ��HWND��ʾ��CControlUI����
class CWndUI: public CControlUI
{
public:
    CWndUI(): m_hWnd(nullptr){}

    virtual void SetInternVisible(bool bVisible = true)
    {
        __super::SetInternVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
    }

    virtual void SetPos(RECT rc)
    {
        __super::SetPos(rc);
        ::SetWindowPos(m_hWnd, nullptr, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    BOOL Attach(HWND hWndNew)
    {
        if (! ::IsWindow(hWndNew))
        {
            return FALSE;
        }

        m_hWnd = hWndNew;
        return TRUE;
    }

    HWND Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = nullptr;
        return hWnd;
    }

	HWND GetHWND()
	{
		return m_hWnd;
	}

protected:
    HWND m_hWnd;
};

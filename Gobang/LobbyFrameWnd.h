#pragma once
#include "StdAfx.h"

class CLobbyFrameWnd :
	public CWindowWnd,
	public INotifyUI
{
public:
	CLobbyFrameWnd();

	LPCTSTR GetWindowClassName() const;
	void Notify(TNotifyUI &);

	void OnFinalMessage(HWND /*hWnd*/);


	LRESULT OnNcActivate(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnNcCalcSize(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnNcPaint(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnNcHitTest(UINT, WPARAM, LPARAM, BOOL &);

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL &);

	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnGetMinMaxInfo(UINT, WPARAM, LPARAM, BOOL &);
	LRESULT OnSysCommand(UINT, WPARAM, LPARAM, BOOL &);

	LRESULT HandleMessage(UINT, WPARAM, LPARAM);

private:
	CPaintManagerUI m_PaintManager;
};


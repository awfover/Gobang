#pragma once
#include "StdAfx.h"
#include "GobangManager.h"
#include "GameUserUI.h"

#define BOARD_PT_START 27
#define BOARD_PT_INTERVAL 39
#define BOARD_PT_OFFSET 16

class CGobangManager;

class CGameFrameWnd :
	public CWindowWnd
{
public:
	CGameFrameWnd(CGobangManager *lpGobangManager);

	LPCTSTR GetWindowClassName() const;
	void Notify(TNotifyUI &);

	void InitWindow();
	CControlUI* CreateControl(LPCTSTR pstrClassName);

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

	void Startup(const UserProfile &, const UserProfile &);

	BOOL InBoardPointArea(TNotifyUI &);
	BOOL GetBoardCoordinate(TNotifyUI &, POINT &);
	BOOL IsCurrentPointAvailable(TNotifyUI &);

	void ShowWaitingPiece(TNotifyUI &);
	void HideWaitingPiece();

	BOOL IsPtInRect(POINT &, RECT &);

private:
	CPaintManagerUI m_PaintManager;
	CGobangManager *m_pGobangManager;

	CGameUserUI *m_pUser;
	CGameUserUI *m_pRival;
};


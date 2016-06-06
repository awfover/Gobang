#pragma once
#include "StdAfx.h"
#include "GobangManager.h"
#include <vector>

using namespace std;

class CGobangManager;

class CLobbyFrameWnd :
	public CWindowWnd,
	public INotifyUI
{
public:
	CLobbyFrameWnd(CGobangManager *lpGobangManager);

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

	void Update(const UserProfile &, const vector<UserProfile> &);
	void UpdateUserProfile(const UserProfile &);
	void UpdateOnlineUserList(const vector<UserProfile> &);

private:
	CPaintManagerUI m_PaintManager;
	CGobangManager *m_pGobangManager;
	CDialogBuilder m_DialogBuilder;

	UserProfile m_UserProfile;
	vector<UserProfile> m_OnlineUserList;
};


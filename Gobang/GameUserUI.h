#pragma once
#include "StdAfx.h"
#include "Base.h"

#define ONE_ROUND_TIME 600
#define GameUserInterface L"GameUser"

enum PIECE_TYPE
{
	PIECE_WHITE = 0,
	PIECE_BLACK = 1,
};

class CGameUserUI :
	public CVerticalLayoutUI,
	public IDialogBuilderCallback
{
public:
	CGameUserUI(CPaintManagerUI &);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR);
	CControlUI * CreateControl(LPCTSTR);

	void DoEvent(TEventUI& event);

	UserProfile & GetProfile() const;
	PIECE_TYPE GetPieceType();
	UINT GetRemainSecs();
	UINT GetWinTimes();
	BOOL IsReady();
	BOOL IsInRound();

	void SetUserProfile(const UserProfile &);
	void SetPieceType(PIECE_TYPE);
	void SetReady();
	void StartRound();
	void EndRound();

	void Update();
	void UpdateProfile();
	void UpdateRemainSecs();
	void UpdateWinTimes();
	void UpdateReady();

private:
	CPaintManagerUI m_PaintManager;
	UserProfile m_UserProfile;
	PIECE_TYPE m_PieceType;
	UINT m_RemainSecs;
	UINT m_WinTimes;
	BOOL m_IsReady;
	BOOL m_IsInRound;
};
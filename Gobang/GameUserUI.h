#pragma once
#include "StdAfx.h"
#include "Base.h"

#define ONE_ROUND_TIME 600
#define GameUserInterface L"GameUser"

class CGameUserUI :
	public CVerticalLayoutUI
{
public:
	CGameUserUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR);

	void DoEvent(TEventUI& event);

	UserProfile & GetProfile() const;
	UINT GetRemainSecs();
	UINT GetWinTimes();
	BOOL IsReady();
	BOOL IsInRound();

	void SetUserProfile(const UserProfile &);
	void SetReady();
	void StartRound();
	void EndRound();

	void Update();
	void UpdateProfile();
	void UpdateRemainSecs();
	void UpdateWinTimes();
	void UpdateReady();

private:
	UserProfile m_UserProfile;
	UINT m_RemainSecs;
	UINT m_WinTimes;
	BOOL m_IsReady;
	BOOL m_IsInRound;
};
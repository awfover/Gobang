#pragma once
#include "StdAfx.h"

class UserProfile
{
public:

	UserProfile(LPCTSTR = L"", LPCTSTR = L"", UINT = 0, UINT = 0, DOUBLE = 0);
	UserProfile & operator = (const UserProfile &profile);

	LPCTSTR GetName();
	LPCTSTR GetIp();
	LPCTSTR GetAvatar(LPCTSTR);
	LPCTSTR GetTotalGames();
	LPCTSTR GetWinProbility();

private:

	LPCTSTR szName;
	LPCTSTR szIp;
	UINT uAvatar;
	UINT uTotalGames;
	DOUBLE fWinProbility;
};

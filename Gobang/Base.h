#pragma once
#include "StdAfx.h"
#include "Util.h"

typedef struct tagUSERPROFILE
{
	char szName[30];
	UINT uAvatar;
	UINT uTotalGames;
	DOUBLE fWinProbility;
} USERPROFILE, *PUSERPROFILE;

class UserProfile
{
public:

	UserProfile(LPCTSTR = L"", LPCTSTR = L"", UINT = 0, UINT = 0, DOUBLE = 0);
	UserProfile(const USERPROFILE &);
	UserProfile & operator = (const UserProfile &);

	void GetStruct(USERPROFILE &);
	LPCTSTR GetName();
	LPCTSTR GetIp();
	LPCTSTR GetAvatar(LPCTSTR);
	LPCTSTR GetTotalGames();
	LPCTSTR GetWinProbility();

	LPCTSTR GetAvatarString();
	LPCTSTR GetTotalGamesString();
	LPCTSTR GetWinProbilityString();

	void SetIp(LPCTSTR);

private:

	LPCTSTR szName;
	LPCTSTR szIp;
	UINT uAvatar;
	UINT uTotalGames;
	DOUBLE fWinProbility;
};

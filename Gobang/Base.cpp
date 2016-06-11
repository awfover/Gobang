#include "Base.h"


UserProfile::UserProfile(LPCTSTR _szName, LPCTSTR _szIp, UINT _uAvatar, UINT _uTotalGames, DOUBLE _fWinProbility)
{
	szName = _szName;
	szIp = _szIp;
	uAvatar = _uAvatar;
	uTotalGames = _uTotalGames;
	fWinProbility = _fWinProbility;
}

UserProfile::UserProfile(const USERPROFILE &profile)
{
	szName = M2W((char *)profile.szName);
	szIp = L"";
	uAvatar = profile.uAvatar;
	uTotalGames = profile.uTotalGames;
	fWinProbility = profile.fWinProbility;
}

UserProfile & UserProfile::operator = (const UserProfile &profile)
{
	this->szName = profile.szName;
	this->szIp = profile.szIp;
	this->uAvatar = profile.uAvatar;
	this->uTotalGames = profile.uTotalGames;
	this->fWinProbility = profile.fWinProbility;
	return *this;
}

void UserProfile::GetStruct(USERPROFILE &profile)
{
	strcpy(profile.szName, W2M((LPTSTR)szName));
	profile.uAvatar = uAvatar;
	profile.uTotalGames = uTotalGames;
	profile.fWinProbility = fWinProbility;
}

LPCTSTR UserProfile::GetName()
{
	return szName;
}

LPCTSTR UserProfile::GetIp()
{
	return szIp;
}

void UserProfile::SetIp(LPCTSTR _szIp)
{
	szIp = _szIp;
}

LPCTSTR UserProfile::GetAvatar(const LPCTSTR szSize)
{
	LPTSTR szPath = new WCHAR[30];
	wsprintf(szPath, L"res\\one-piece\\%s\\%u.png", szSize, uAvatar);
	return (LPCTSTR)szPath;
}

LPCTSTR UserProfile::GetAvatarString()
{
	LPTSTR szPath = new WCHAR[30];
	wsprintf(szPath, L"%u", uAvatar);
	return (LPCTSTR)szPath;
}

LPCTSTR UserProfile::GetTotalGames()
{
	LPTSTR szTotalGames = new WCHAR[30];
	wsprintf(szTotalGames, L"共进行了%u局游戏", uTotalGames);
	return (LPCTSTR)szTotalGames;
}

LPCTSTR UserProfile::GetTotalGamesString()
{
	LPTSTR szTotalGames = new WCHAR[30];
	wsprintf(szTotalGames, L"%u", uTotalGames);
	return (LPCTSTR)szTotalGames;
}

LPCTSTR UserProfile::GetWinProbility()
{
	UINT integer = (UINT)(fWinProbility * 100);
	UINT decimal = (UINT)((fWinProbility * 100 - integer) * 100 + 0.5);

	LPTSTR szProbility = new WCHAR[10];
	wsprintf(szProbility, L"%02u.%02u%%", integer, decimal);
	return (LPCTSTR)szProbility;
}

LPCTSTR UserProfile::GetWinProbilityString()
{
	UINT integer = (UINT)(fWinProbility);
	UINT decimal = (UINT)((fWinProbility - integer ) * 10000 + 0.5);

	LPTSTR szProbility = new WCHAR[10];
	wsprintf(szProbility, L"%u.%u", integer, decimal);
	return (LPCTSTR)szProbility;
}
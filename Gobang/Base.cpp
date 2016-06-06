#include "Base.h"


UserProfile::UserProfile(LPCTSTR _szName, LPCTSTR _szIp, UINT _uAvatar, UINT _uTotalGames, DOUBLE _fWinProbility)
{
	szName = _szName;
	szIp = _szIp;
	uAvatar = _uAvatar;
	uTotalGames = _uTotalGames;
	fWinProbility = _fWinProbility;
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

LPCTSTR UserProfile::GetName()
{
	return szName;
}

LPCTSTR UserProfile::GetIp()
{
	return szIp;
}

LPCTSTR UserProfile::GetAvatar(const LPCTSTR szSize)
{
	LPTSTR szPath = new WCHAR[30];
	wsprintf(szPath, L"res\\one-piece\\%s\\%u.png", szSize, uAvatar);
	return (LPCTSTR)szPath;
}

LPCTSTR UserProfile::GetTotalGames()
{
	LPTSTR szTotalGames = new WCHAR[30];
	wsprintf(szTotalGames, L"共进行了%u局游戏", uTotalGames);
	return (LPCTSTR)szTotalGames;
}

LPCTSTR UserProfile::GetWinProbility()
{
	UINT integer = (UINT)(fWinProbility * 100);
	UINT decimal = (UINT)(fWinProbility / 100 * 100 + 0.5);

	LPTSTR szProbility = new WCHAR[10];
	wsprintf(szProbility, L"%02u.%02u%%", integer, decimal);
	return (LPCTSTR)szProbility;
}
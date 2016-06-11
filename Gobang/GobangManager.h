#pragma once

#include "StdAfx.h"
#include "Base.h"
#include "Util.h"
#include "GameBoard.h"
#include "WelcomeFrameWnd.h"
#include "LobbyFrameWnd.h"
#include "GameFrameWnd.h"
#include <vector>
#include <sstream>

using namespace std;

#define USER_DATA_FILE L"UserData.ini"
#define USER_AVATAR_LENGTH 15

#define INFO_USER_LOGIN     1
#define INFO_USER_LOGOUT    2
#define INFO_USER_INVITE    3
#define INFO_USER_ACCEPT    4
#define INFO_USER_READY     5
#define INFO_USER_PLACE     6

#define PORT 2333

const UINT MAX_BUFFER = 600;
const char INFO_TOKEN[] = "This is the token for gobang comunicating.";

typedef struct tagINFO
{
	DWORD dwType;
	DWORD dwSize;
	char szToken[50];
	char szMsg[500];

	tagINFO()
	{
		memset(this->szToken, 0, sizeof(this->szToken));
		memset(this->szMsg, 0, sizeof(this->szMsg));
		this->dwType = this->dwSize = 0;
	}

	tagINFO(DWORD dwType, DWORD dwSize, const char *szMsg)
	{
		this->dwType = dwType;
		this->dwSize = dwSize;
		strcpy(this->szToken, INFO_TOKEN);
		memcpy(this->szMsg, szMsg, dwSize);
	}
} INFO, *PINFO;

class CWelcomeFrameWnd;
class CLobbyFrameWnd;
class CGameFrameWnd;

class CGobangManager
{
public:
	CGobangManager();
	~CGobangManager();

	void ShowWelcome();
	void HideWelcome();
	void ShowLobby();
	void HideLobby();
	void ShowGame();
	void HideGame();

	void UserLogin(UserProfile &);
	void UserLogout(UserProfile &);

	bool IsPointAvailable(POINT &);
	bool NewUserInvitation(SOCKET, LPCTSTR);
	void TryInviteUser(UserProfile &rival);
	void NewGame(bool = true);

	void UserReady(LPVOID, bool = false);
	void UserPlace(LPVOID, bool = false);

private:
	void Init();
	void InitSocket();
	bool Startup();
	bool InitUserProfile();
	void SaveUserProfile();
	void SendUserProfile(LPCTSTR);
	bool GetOnlineUserProfile(LPCTSTR, UserProfile &);
	void RefreshOnlineUserList(bool = false);

	static unsigned __stdcall TcpListening(LPVOID);
	static unsigned __stdcall UdpListening(LPVOID);
	static unsigned __stdcall SendInvitation(LPVOID);
	static unsigned __stdcall GameListening(LPVOID);
	static unsigned __stdcall SendUserAccept(LPVOID);
	static unsigned __stdcall SendUserReady(LPVOID);
	static unsigned __stdcall SendUserPlace(LPVOID);

	LPCTSTR GetUserDataPath();

	SOCKET m_UdpSocket;
	SOCKET m_TcpSocket;
	SOCKET m_GameSocket;

	UserProfile m_RivalProfile;
	UserProfile m_UserProfile;

	POINT m_UserLastPlace;

	vector<LPCTSTR> m_UserIPList;
	vector<UserProfile> m_OnlineUserList;

	bool m_IsUserInviteRival;
	UINT m_GameRounds;
	int m_UserRoll;
	int m_RivalRoll;

	LPCTSTR m_InvitationIP;
	bool m_IsInWaitingInvitation;
	bool m_IsInGame;

	GameBoard m_GameBoard;

	CWelcomeFrameWnd *m_pWelcomeFrame;
	CLobbyFrameWnd *m_pLobbyFrame;
	CGameFrameWnd *m_pGameFrame;
};
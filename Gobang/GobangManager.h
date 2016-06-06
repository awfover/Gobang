#pragma once

#include "StdAfx.h"
#include "Base.h"
#include "Util.h"
#include "WelcomeFrameWnd.h"
#include "LobbyFrameWnd.h"
#include "GameFrameWnd.h"
#include <vector>

using namespace std;

enum GAME_STATE {
	STATE_INIT,
	STATE_WELCOME,
	STATE_LOBBY,
	STATE_GAME,
};


class CWelcomeFrameWnd;
class CLobbyFrameWnd;
class CGameFrameWnd;


class CGobangManager {
public:
	CGobangManager();
	~CGobangManager();

	void ShowWelcome();
	void HideWelcome();
	void ShowLobby();
	void HideLobby();
	void ShowGame();
	void HideGame();

	BOOL IsPointAvailable(POINT &);
	void TryInviteUser(const UserProfile &rival);

private:
	BOOL Startup();
	BOOL InitUserProfile();
	void RefreshOnlineUserList();

	UserProfile m_UserProfile;
	vector<UserProfile> m_OnlineUserList;

	GAME_STATE m_GameState;

	CWelcomeFrameWnd *m_pWelcomeFrame;
	CLobbyFrameWnd *m_pLobbyFrame;
	CGameFrameWnd *m_pGameFrame;
};
#pragma once

#include "StdAfx.h"
#include "WelcomeFrameWnd.h"
#include "LobbyFrameWnd.h"
#include "GameFrameWnd.h"

class CWelcomeFrameWnd;
class CLobbyFrameWnd;
class CGameFrameWnd;

enum GAME_STATE {
	STATE_INIT,
	STATE_WELCOME,
	STATE_LOBBY,
	STATE_GAME,
};

class CGobangManager {
public:
	CGobangManager();
	~CGobangManager();

	void ShowWelcome();
	void HideWelcome();
	void ShowLobby();
	void ShowGame();

private:
	GAME_STATE m_GameState;

	CWelcomeFrameWnd *m_pWelcomeFrame;
	CLobbyFrameWnd *m_pLobbyFrame;
	CGameFrameWnd *m_pGameFrame;
};
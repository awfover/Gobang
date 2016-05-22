#include "GobangManager.h"

CGobangManager::CGobangManager() {
	this->m_GameState = STATE_INIT;
}

CGobangManager::~CGobangManager() {
	if (this->m_pWelcomeFrame != nullptr) {
		delete this->m_pWelcomeFrame;
	}
	if (this->m_pLobbyFrame != nullptr) {
		delete this->m_pLobbyFrame;
	}
	if (this->m_pGameFrame != nullptr) {
		delete this->m_pGameFrame;
	}
}

void CGobangManager::ShowWelcome() {
	this->m_pWelcomeFrame = new CWelcomeFrameWnd(this);
	if (this->m_pWelcomeFrame == nullptr) {
		return;
	}

	this->m_pWelcomeFrame->Create(NULL, _T("五子棋"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
	this->m_pWelcomeFrame->CenterWindow();
	this->m_pWelcomeFrame->ShowWindow(SW_SHOW);
}

void CGobangManager::HideWelcome() {
	if (this->m_pWelcomeFrame == nullptr) {
		return;
	}
	this->m_pWelcomeFrame->ShowWindow(SW_HIDE);
}

void CGobangManager::ShowLobby() {
	this->m_pLobbyFrame = new CLobbyFrameWnd();
	if (this->m_pLobbyFrame == nullptr) {
		return;
	}

	this->m_pLobbyFrame->Create(NULL, _T("大厅"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
	this->m_pLobbyFrame->CenterWindow();
	this->m_pLobbyFrame->ShowWindow(SW_SHOW);
}

void CGobangManager::ShowGame() {
	this->m_pGameFrame = new CGameFrameWnd();
	if (this->m_pGameFrame == nullptr) {
		return;
	}

	this->m_pGameFrame->Create(NULL, _T("游戏"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
	this->m_pGameFrame->CenterWindow();
	this->m_pGameFrame->ShowWindow(SW_SHOW);
}
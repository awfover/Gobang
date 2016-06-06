#include "GobangManager.h"


CGobangManager::CGobangManager()
{
	m_GameState = STATE_INIT;

	Startup();
	InitUserProfile();
	RefreshOnlineUserList();
}

CGobangManager::~CGobangManager()
{
	if (m_pWelcomeFrame != nullptr)
	{
		delete m_pWelcomeFrame;
	}
	if (m_pLobbyFrame != nullptr)
	{
		delete m_pLobbyFrame;
	}
	if (m_pGameFrame != nullptr)
	{
		delete m_pGameFrame;
	}
}

BOOL CGobangManager::Startup()
{
	// ��ʼ��
	WSADATA wsadata;
	return (WSAStartup(MAKEWORD(2, 2), &wsadata) == 0);
}

BOOL CGobangManager::InitUserProfile()
{
	char hostname[255];
	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		return false;
	}

	m_UserProfile = UserProfile((LPCTSTR)C2W(hostname), GetComputerIPs());
	return true;
}

void CGobangManager::RefreshOnlineUserList()
{
	// ...
	m_OnlineUserList.push_back(m_UserProfile);
}

void CGobangManager::ShowWelcome()
{
	m_pWelcomeFrame = new CWelcomeFrameWnd(this);
	if (m_pWelcomeFrame == nullptr)
	{
		return;
	}

	m_pWelcomeFrame->Create(NULL, _T("������"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
	m_pWelcomeFrame->CenterWindow();
	m_pWelcomeFrame->ShowWindow(SW_SHOW);
}

void CGobangManager::HideWelcome()
{
	if (m_pWelcomeFrame == nullptr)
	{
		return;
	}
	m_pWelcomeFrame->ShowWindow(SW_HIDE);
}

void CGobangManager::ShowLobby()
{
	if (m_pLobbyFrame == nullptr)
	{
		m_pLobbyFrame = new CLobbyFrameWnd(this);
	}

	m_pLobbyFrame->Create(NULL, _T("����"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
	m_pLobbyFrame->CenterWindow();
	m_pLobbyFrame->ShowWindow(SW_SHOW);
	m_pLobbyFrame->Update(m_UserProfile,  m_OnlineUserList);
}

void CGobangManager::HideLobby()
{
	if (m_pLobbyFrame == nullptr)
	{
		return;
	}
	m_pLobbyFrame->ShowWindow(SW_HIDE);
}

void CGobangManager::ShowGame()
{
	m_pGameFrame = new CGameFrameWnd(this);
	if (m_pGameFrame == nullptr)
	{
		return;
	}

	m_pGameFrame->Create(NULL, _T("��Ϸ"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
	m_pGameFrame->CenterWindow();
	m_pGameFrame->ShowWindow(SW_SHOW);
	m_pGameFrame->Startup(m_UserProfile, m_UserProfile);
}

void CGobangManager::HideGame()
{
	if (m_pGameFrame == nullptr)
	{
		return;
	}
	m_pGameFrame->ShowWindow(SW_HIDE);
}

/**
 * �жϸ��������������Ƿ��Ѿ��¹�����
 */
BOOL CGobangManager::IsPointAvailable(POINT &pt)
{
	return true;
}

/**
* ����������Ҷ�ս
*/
void CGobangManager::TryInviteUser(const UserProfile &rival)
{
	// ��ʱ������...
	HideLobby();
	ShowGame();
}
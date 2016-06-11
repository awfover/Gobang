#include "GobangManager.h"
#include <random>
#include <ctime>
#include <iostream>

CGobangManager::CGobangManager() :
	m_IsInGame(false),
	m_pWelcomeFrame(NULL),
	m_pLobbyFrame(NULL),
	m_pGameFrame(NULL)
{
	Startup();
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

LPCTSTR CGobangManager::GetUserDataPath()
{
	LPTSTR szPath = new TCHAR[255];
	wsprintf(szPath, L"%s%s", GetInstancePath(),USER_DATA_FILE);
	return szPath;
}

void CGobangManager::Init()
{
	InitUserProfile();
	InitSocket();
	RefreshOnlineUserList(true);

	_beginthreadex(NULL, 0, CGobangManager::TcpListening, (LPVOID)this, 0, NULL);
	_beginthreadex(NULL, 0, CGobangManager::UdpListening, (LPVOID)this, 0, NULL);
}

void CGobangManager::InitSocket()
{
	m_UdpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	int optval = 1;
	setsockopt(m_UdpSocket, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (char *)&optval, sizeof(int));
}

bool CGobangManager::Startup()
{
	// 初始化
	WSADATA wsadata;
	return (WSAStartup(MAKEWORD(2, 2), &wsadata) == 0);
}

unsigned __stdcall CGobangManager::TcpListening(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int));

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (sockaddr *)&addr, sizeof(sockaddr)))
	{
		return 0;
	}

	listen(sock, 1);

	sockaddr_in clientSock;
	int nSockLen = sizeof(clientSock);
	char buffer[MAX_BUFFER];
	UINT nRecv;
	SOCKET connector;
	LPCTSTR szIP;
	INFO info;
	while (true)
	{
		connector = accept(sock, (sockaddr *)&clientSock, &nSockLen);
		szIP = M2W(inet_ntoa(clientSock.sin_addr));

		nRecv = recv(connector, buffer, MAX_BUFFER, 0);
		if (nRecv > 0)
		{
			info = *(PINFO)buffer;
			if ((strcmp(info.szToken, INFO_TOKEN) == 0) && (info.dwType == INFO_USER_INVITE))
			{
				if (!pManager->NewUserInvitation(connector, szIP))
				{
					closesocket(connector);
				}
			}
			else
			{
				closesocket(connector);
			}
		}
		else
		{
			closesocket(connector);
		}
	}

	return 0;
}

unsigned __stdcall CGobangManager::UdpListening(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (sockaddr *)&addr, sizeof(sockaddr)) == -1)
	{
		return 0;
	}

	char buffer[MAX_BUFFER];
	sockaddr_in clientSock;
	int nSockLen = sizeof(clientSock);
	UINT nRecv;
	INFO info;
	while (true)
	{
		nRecv = recvfrom(sock, buffer, MAX_BUFFER, 0, (sockaddr *)&clientSock, &nSockLen);
		if (nRecv == 0)
		{
			continue;
		}
		
		info = *(PINFO)buffer;
		if (!info.szToken)
		{
			continue;
		}

		LPCTSTR szToken = M2W(info.szToken);
		if (_tcsicmp(szToken, M2W((char *)INFO_TOKEN)) != 0)
		{
			continue;
		}
		delete szToken;

		if ((info.dwType == INFO_USER_LOGIN) || (info.dwType == INFO_USER_LOGOUT))
		{
			LPCTSTR szIP = M2W(inet_ntoa(clientSock.sin_addr));
			USERPROFILE profile = *(USERPROFILE *)info.szMsg;
			UserProfile user(profile);
			user.SetIp(szIP);

			if (info.dwType == INFO_USER_LOGIN)
			{
				pManager->UserLogin(user);
			}
			else
			{
				pManager->UserLogout(user);
			}
		}
	}

	return 0;
}

unsigned __stdcall CGobangManager::SendInvitation(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(W2M((LPTSTR)pManager->m_InvitationIP));

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sock, (sockaddr *)&addr, sizeof(addr)) != 0)
	{
		return 0;
	}

	pManager->m_GameSocket = sock;
	_beginthreadex(NULL, 0, CGobangManager::GameListening, lpParam, 0, NULL);

	INFO info(INFO_USER_INVITE, 0, "");
	if (send(sock, (char *)&info, sizeof(INFO), 0) == SOCKET_ERROR)
	{
		return 0;
	}

	return 0;
}

unsigned __stdcall CGobangManager::GameListening(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;
	SOCKET sock = pManager->m_GameSocket;

	char buffer[MAX_BUFFER];
	UINT nRecv;
	INFO info;
	while (true)
	{
		nRecv = recv(sock, buffer, MAX_BUFFER, 0);
		if ((nRecv > 0) && (nRecv <= MAX_BUFFER))
		{
			info = *(PINFO)buffer;
			if (!info.szToken || strcmp(info.szToken, INFO_TOKEN) != 0)
			{
				continue;
			}

			switch (info.dwType)
			{
			case INFO_USER_ACCEPT:
				pManager->NewGame();
				break;

			case INFO_USER_READY:
				pManager->UserReady(info.szMsg, true);
				break;
				
			case INFO_USER_PLACE:
				pManager->UserPlace(info.szMsg, true);
				break;
			}
		}
		else
		{
			// 连接关闭
			MessageBox(NULL, L"连接中断", L"错误", NULL);
			closesocket(sock);
			return 0;
		}
	}

	return 0;
}

unsigned __stdcall CGobangManager::SendUserAccept(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;
	SOCKET sock = pManager->m_GameSocket;

	INFO info(INFO_USER_ACCEPT, 0, "");
	if (send(sock, (char *)&info, sizeof(INFO), 0) == SOCKET_ERROR)
	{
		// 连接关闭
		MessageBox(NULL, L"连接中断", L"错误", NULL);
		closesocket(sock);
		return 0;
	}

	return 0;
}

unsigned __stdcall CGobangManager::SendUserReady(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;
	SOCKET sock = pManager->m_GameSocket;

	INFO info(INFO_USER_READY, sizeof(pManager->m_UserRoll), (char *)&pManager->m_UserRoll);
	if (send(sock, (char *)&info, sizeof(INFO), 0) == SOCKET_ERROR)
	{
		// 连接关闭
		MessageBox(NULL, L"连接中断", L"错误", NULL);
		closesocket(sock);
		return 0;
	}

	return 0;
}

unsigned __stdcall CGobangManager::SendUserPlace(LPVOID lpParam)
{
	CGobangManager *pManager = (CGobangManager *)lpParam;
	SOCKET sock = pManager->m_GameSocket;

	INFO info(INFO_USER_PLACE, sizeof(pManager->m_UserLastPlace), (char *)&pManager->m_UserLastPlace);
	if (send(sock, (char *)&info, sizeof(INFO), 0) == SOCKET_ERROR)
	{
		// 连接关闭
		MessageBox(NULL, L"连接中断", L"错误", NULL);
		closesocket(sock);
		return 0;
	}

	return 0;
}

bool CGobangManager::InitUserProfile()
{
	GetComputerIPList(m_UserIPList);

	LPCTSTR szPath = GetUserDataPath();
	LPTSTR szName = new TCHAR[255];
	GetPrivateProfileString(L"User", L"Name", M2W(GetHostName()), szName, 255, szPath);

	LPTSTR szWinProbility = new TCHAR[255];
	GetPrivateProfileString(L"User", L"WinProbility", L"0", szWinProbility, 255, szPath);
	
	srand((UINT)time(0));
	UINT uAvatar = GetPrivateProfileInt(L"User", L"Avatar", rand() % USER_AVATAR_LENGTH, szPath);
	if (uAvatar >= USER_AVATAR_LENGTH)
	{
		uAvatar = rand() % USER_AVATAR_LENGTH;
	}

	UINT uTotalTimes = GetPrivateProfileInt(L"User", L"TotalGames", 0, szPath);

	m_UserProfile = UserProfile(szName, L"", uAvatar, uTotalTimes, atof(W2M(szWinProbility)));
	SaveUserProfile();

	return true;
}

void CGobangManager::SaveUserProfile()
{
	LPCTSTR szPath = GetUserDataPath();
	WritePrivateProfileString(L"User", L"Name", m_UserProfile.GetName(), szPath);
	WritePrivateProfileString(L"User", L"Avatar", m_UserProfile.GetAvatarString(), szPath);
	WritePrivateProfileString(L"User", L"TotalGames", m_UserProfile.GetTotalGamesString(), szPath);
	WritePrivateProfileString(L"User", L"WinProbility", m_UserProfile.GetWinProbilityString(), szPath);
	delete szPath;
}

void CGobangManager::SendUserProfile(LPCTSTR szIP)
{
	USERPROFILE profile;
	m_UserProfile.GetStruct(profile);

	INFO info(INFO_USER_LOGIN, sizeof(profile), (char *)&profile);

	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(W2M((LPTSTR)szIP));

	sendto(m_UdpSocket, (char *)&info, sizeof(info), 0, (sockaddr *)&addr, sizeof(addr));
}

bool CGobangManager::GetOnlineUserProfile(LPCTSTR szIP, UserProfile &user)
{
	for (auto i = m_OnlineUserList.begin(); i != m_OnlineUserList.end(); i++)
	{
		if (_tcsicmp(szIP, i->GetIp()) == 0)
		{
			user = *i;
			return true;
		}
	}
	return false;
}

void CGobangManager::RefreshOnlineUserList(bool isBroadcast)
{
	if (isBroadcast)
	{
		vector<LPCTSTR> broadcastIPList;
		GetBroadcastIPList(broadcastIPList);
		for (auto i = broadcastIPList.begin(); i != broadcastIPList.end(); i++)
		{
			SendUserProfile(*i);
		}
	}
	else if (m_pLobbyFrame)
	{
		m_pLobbyFrame->UpdateOnlineUserList(m_OnlineUserList);
	}
}

void CGobangManager::UserLogin(UserProfile &user)
{
	LPCTSTR szIP = user.GetIp();
	for (auto i = m_UserIPList.begin(); i != m_UserIPList.end(); i++)
	{
		if (_tcsicmp(*i, szIP) == 0)
		{
			return;
		}
	}

	for (auto i = m_OnlineUserList.begin(); i != m_OnlineUserList.end(); i++)
	{
		if (_tcsicmp(i->GetIp(), szIP) == 0)
		{
			return;
		}
	}

	SendUserProfile(szIP);
	m_OnlineUserList.push_back(user);
	RefreshOnlineUserList();
}

void CGobangManager::UserLogout(UserProfile &user)
{
	LPCTSTR szIP = user.GetIp();
	for (auto i = m_OnlineUserList.begin(); i != m_OnlineUserList.end(); i++)
	{
		if (_tcsicmp(i->GetIp(), szIP) == 0)
		{
			m_OnlineUserList.erase(i);
			RefreshOnlineUserList();
			return;
		}
	}
}

void CGobangManager::ShowWelcome()
{
	if (m_pWelcomeFrame == nullptr)
	{
		m_pWelcomeFrame = new CWelcomeFrameWnd(this);
		m_pWelcomeFrame->Create(NULL, _T("五子棋"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
		m_pWelcomeFrame->CenterWindow();
	}

	m_pWelcomeFrame->ShowWindow(true);
}

void CGobangManager::HideWelcome()
{
	if (m_pWelcomeFrame == nullptr)
	{
		return;
	}
	m_pWelcomeFrame->ShowWindow(false);
}

void CGobangManager::ShowLobby()
{
	ShowGame();
	HideGame();

	if (m_pLobbyFrame == nullptr)
	{
		m_pLobbyFrame = new CLobbyFrameWnd(this);
		m_pLobbyFrame->Create(NULL, _T("大厅"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
		m_pLobbyFrame->CenterWindow();
		Init();
	}

	m_pLobbyFrame->ShowWindow(true);
	m_pLobbyFrame->Update(m_UserProfile,  m_OnlineUserList);
}

void CGobangManager::HideLobby()
{
	if (m_pLobbyFrame == nullptr)
	{
		return;
	}
	m_pLobbyFrame->ShowWindow(false);
}

void CGobangManager::ShowGame()
{
	if (m_pGameFrame == nullptr)
	{
		m_pGameFrame = new CGameFrameWnd(this);
		m_pGameFrame->Create(NULL, _T("游戏"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 1010, 690);
		m_pGameFrame->CenterWindow();
	}

	m_pGameFrame->ShowWindow(true);
	m_pGameFrame->Startup(m_UserProfile, m_RivalProfile);
}

void CGobangManager::HideGame()
{
	if (m_pGameFrame == nullptr)
	{
		return;
	}
	m_pGameFrame->ShowWindow(false);
}

/**
 * 判断给定参数坐标上是否已经下过棋了
 */
bool CGobangManager::IsPointAvailable(POINT &pt)
{
	return m_GameBoard.IsAvailable(pt.x, pt.y);
}

/**
* 尝试邀请玩家对战
*/
void CGobangManager::TryInviteUser(UserProfile &rival)
{
	if (m_IsInWaitingInvitation)
	{
		return;
	}

	m_InvitationIP = rival.GetIp();
	m_IsInWaitingInvitation = true;
	_beginthreadex(NULL, 0, CGobangManager::SendInvitation, (LPVOID)this, 0, NULL);
}

bool CGobangManager::NewUserInvitation(SOCKET sock, LPCTSTR szIP)
{
	if (m_IsInGame)
	{
		return false;
	}

	if (!GetOnlineUserProfile(szIP, m_RivalProfile))
	{
		return false;
	}

	LPTSTR szMsg = new TCHAR[100];
	wsprintf(szMsg, L"IP为%s的%s邀请你加入游戏哦=。=", szIP, m_RivalProfile.GetName());
	if (MessageBox(NULL, szMsg, L"游戏邀请", MB_YESNO) != 6)
	{
		delete szMsg;
		return false;
	}
	delete szMsg;

	NewGame(false);

	m_GameSocket = sock;
	_beginthreadex(NULL, 0, CGobangManager::SendUserAccept, (LPVOID)this, 0, NULL);
	_beginthreadex(NULL, 0, CGobangManager::GameListening, (LPVOID)this, 0, NULL);
	
	return true;
}

void CGobangManager::NewGame(bool IsUserInviteRival)
{
	m_IsInGame = true;
	m_UserRoll = -1;
	m_RivalRoll = -1;
	m_IsUserInviteRival = IsUserInviteRival;
	m_GameRounds = 0;
	m_GameBoard.Clear();

	HideLobby();
	ShowGame();
}

void CGobangManager::UserReady(LPVOID lpParam, bool isRival)
{
	int uRoll = *(UINT *)lpParam;
	if (isRival)
	{
		m_RivalRoll = uRoll;
		m_pGameFrame->SetReady(true);
	}
	else
	{
		m_UserRoll = uRoll;
		_beginthreadex(NULL, 0, CGobangManager::SendUserReady, (LPVOID)this, 0, NULL);
	}

	if ((m_UserRoll != -1) && (m_RivalRoll != -1))
	{
		bool bOrder = ((m_UserRoll > m_RivalRoll) || ((m_UserRoll == m_RivalRoll) && m_IsUserInviteRival && (m_GameRounds % 2 == 0)));
		m_pGameFrame->SetPieceOrder(bOrder);
	}
}

void CGobangManager::UserPlace(LPVOID lpParam, bool isRival)
{
	POINT pt = *(PPOINT)lpParam;
	if (!isRival)
	{
		m_UserLastPlace = pt;
		_beginthreadex(NULL, 0, CGobangManager::SendUserPlace, (LPVOID)this, 0, NULL);
	}
	else
	{
		m_pGameFrame->PlacePiece(pt);
	}

	int nResult = m_GameBoard.Place(pt.x, pt.y, isRival);
	if (nResult == -1)
	{
		// 出错
	}
	else if (nResult == 1)
	{
		// 游戏结束
		MessageBox(NULL, L"游戏结束", NULL, NULL);
	}
}
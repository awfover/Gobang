#include "GameFrameWnd.h"


CGameFrameWnd::CGameFrameWnd(CGobangManager *pGobangManager) :
	m_pUser(NULL),
	m_pRival(NULL)
{
	m_pGobangManager = pGobangManager;
}


LPCTSTR CGameFrameWnd::GetWindowClassName() const
{
	return _T("UIMainFrame");
}

void CGameFrameWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

CDuiString CGameFrameWnd::GetSkinFile()
{
	return L"game.xml";
}

CDuiString CGameFrameWnd::GetSkinFolder()
{
	return L"";
}

CControlUI* CGameFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, L"GameUser") == 0)
	{
		return new CGameUserUI(m_PaintManager);
	}
	return NULL;
}

CPaintManagerUI* CGameFrameWnd::GetMainWndPaintManager()
{
	return &m_PaintManager;
}

void CGameFrameWnd::Notify(TNotifyUI &msg)
{
	if (msg.sType == L"click")
	{
		CDuiString dsControlName = msg.pSender->GetName();
		if (dsControlName == L"BtnMinimize")
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if (dsControlName == L"BtnMaximize")
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if (dsControlName == L"BtnRestore")
		{
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else if (dsControlName == L"BtnClose")
		{
			PostQuitMessage(0);
			return;
		}
		else if (dsControlName == L"BtnStart")
		{
			if (!m_IsReady)
			{
				SetReady();
			}
		}
		else if (dsControlName == L"BtnCancel")
		{
			// 退出结束游戏，回到大厅
			m_pGobangManager->HideGame();
			m_pGobangManager->ShowLobby();
			return;
		}
		else if (dsControlName == L"BtnConcede")

		{
			// 认输
		}
		else if (m_IsInRound)
		{
			if (IsCurrentPointAvailable(msg))
			{
				PlacePiece(msg);
			}
		}
	}
	if (msg.sType == L"mousemove")
	{
		if (m_IsInRound && IsCurrentPointAvailable(msg))
		{
			ShowWaitingPiece(msg);
		}
		else
		{
			HideWaitingPiece();
		}
	}
}

void CGameFrameWnd::InitWindow()
{
	CenterWindow();

	m_pUser = static_cast<CGameUserUI*>(m_PaintManager.FindControl(L"User"));
	m_pRival = static_cast<CGameUserUI*>(m_PaintManager.FindControl(L"Rival"));
}

LPCTSTR CGameFrameWnd::GetPieceImage(PIECE_TYPE type)
{
	return (type == PIECE_WHITE) ? PIECE_WHITE_IMAGE : PIECE_BLACK_IMAGE;
}

LPCTSTR CGameFrameWnd::GetWaitPieceName()
{
	return (m_pUser && (m_pUser->GetPieceType() == PIECE_WHITE)) ? PIECE_WHITE_WAIT : PIECE_BLACK_WAIT;
}

bool CGameFrameWnd::IsPtInRect(POINT &pt, RECT &rc)
{
	return ((pt.x >= rc.left) && (pt.x <= rc.right) && (pt.y >= rc.top) && (pt.y <= rc.bottom));
}

void CGameFrameWnd::Startup(const UserProfile &user, const UserProfile &rival)
{
	m_IsReady = m_IsInRound = m_IsInGame = false;
	m_pUser->SetUserProfile(user);
	m_pRival->SetUserProfile(rival);
	ClearBoard();
	UpdateButton();
}

bool CGameFrameWnd::InBoardPointArea(TNotifyUI &msg)
{
	RECT rcBoard = static_cast<CControlUI*>(m_PaintManager.FindControl(L"Board"))->GetPos();
	if (!IsPtInRect(msg.ptMouse, rcBoard))
	{
		return false;
	}

	RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
	LONG rtLeft = msg.ptMouse.x - rcWrapper.left;
	LONG rtTop = msg.ptMouse.y - rcWrapper.top;
	UINT dx = (UINT)((rtLeft - BOARD_PT_START) * 1.0 / BOARD_PT_INTERVAL + 0.5);
	UINT dy = (UINT)((rtTop - BOARD_PT_START) * 1.0 / BOARD_PT_INTERVAL + 0.5);
	LONG dLeft = dx * BOARD_PT_INTERVAL + BOARD_PT_START;
	LONG dTop = dy * BOARD_PT_INTERVAL + BOARD_PT_START;

	return ((abs(dLeft - rtLeft) <= BOARD_PT_OFFSET) && (abs(dTop - rtTop) <= BOARD_PT_OFFSET));
}

void CGameFrameWnd::ClearBoard()
{
	static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"PieceWrapper"))->RemoveAll();
	static_cast<CControlUI*>(m_PaintManager.FindControl(L"WhitePieceWait"))->SetVisible(false);
	static_cast<CControlUI*>(m_PaintManager.FindControl(L"BlackPieceWait"))->SetVisible(false);
}

bool CGameFrameWnd::GetBoardCoordinate(TNotifyUI &msg, POINT &pt)
{
	if (!InBoardPointArea(msg))
	{
		return false;
	}

	RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
	LONG rtLeft = msg.ptMouse.x - rcWrapper.left;
	LONG rtTop = msg.ptMouse.y - rcWrapper.top;
	pt.x = (UINT)((rtLeft - BOARD_PT_START) * 1.0 / BOARD_PT_INTERVAL + 0.5);
	pt.y = (UINT)((rtTop - BOARD_PT_START) * 1.0 / BOARD_PT_INTERVAL + 0.5);

	return true;
}

bool CGameFrameWnd::IsCurrentPointAvailable(TNotifyUI &msg)
{
	POINT pt;
	return (GetBoardCoordinate(msg, pt) && (m_pGobangManager->IsPointAvailable(pt)));
}

void CGameFrameWnd::PlacePiece(TNotifyUI &msg)
{
	POINT pt;
	if (!GetBoardCoordinate(msg, pt))
	{
		return;
	}

	m_pGobangManager->UserPlace(&pt);
	PlacePiece(pt, m_pUser->GetPieceType());
}

void CGameFrameWnd::PlacePiece(POINT &pt, PIECE_TYPE type)
{
	RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
	LONG ptLeft = pt.x * BOARD_PT_INTERVAL + BOARD_PT_START;
	LONG ptTop = pt.y * BOARD_PT_INTERVAL + BOARD_PT_START;

	RECT rcPiece;
	rcPiece.left = rcWrapper.left + ptLeft - PIECE_SIZE / 2;
	rcPiece.top = rcWrapper.top + ptTop - PIECE_SIZE / 2;
	rcPiece.right = rcPiece.left + PIECE_SIZE;
	rcPiece.bottom = rcPiece.top + PIECE_SIZE;

	CControlUI *pPiece = new CControlUI();

	LPTSTR szSize = new TCHAR[5];
	wsprintf(szSize, L"%u", PIECE_SIZE);
	pPiece->SetAttribute(L"width", szSize);
	pPiece->SetAttribute(L"height", szSize);
	pPiece->SetAttribute(L"float", L"true");
	pPiece->SetAttribute(L"bkimage", GetPieceImage(type));

	LPTSTR szPos = new TCHAR[20];
	wsprintf(szPos, L"%u,%u,0,0", ptLeft - PIECE_SIZE / 2, ptTop - PIECE_SIZE / 2);
	pPiece->SetAttribute(L"pos", szPos);

	static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"PieceWrapper"))->Add(pPiece);

	delete szSize;
	delete szPos;

	m_IsInRound = !m_IsInRound;
	if (m_IsInRound)
	{
		m_pUser->StartRound();
		m_pRival->EndRound();
	}
	else
	{
		m_pUser->EndRound();
		m_pRival->StartRound();
	}
}

void CGameFrameWnd::PlacePiece(POINT &pt)
{
	PlacePiece(pt, m_pRival->GetPieceType());
}

void CGameFrameWnd::ShowWaitingPiece(TNotifyUI &msg)
{
	RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
	CControlUI* pWaitPiece = static_cast<CControlUI*>(m_PaintManager.FindControl(GetWaitPieceName()));
	
	POINT pt;
	if (!GetBoardCoordinate(msg, pt))
	{
		return;
	}

	LONG ptLeft = pt.x * BOARD_PT_INTERVAL + BOARD_PT_START;
	LONG ptTop = pt.y * BOARD_PT_INTERVAL + BOARD_PT_START;

	RECT rcPiece;
	rcPiece.left = rcWrapper.left + ptLeft - PIECE_WAIT_SIZE / 2;
	rcPiece.top = rcWrapper.top + ptTop - PIECE_WAIT_SIZE / 2;
	rcPiece.right = rcPiece.left + PIECE_WAIT_SIZE;
	rcPiece.bottom = rcPiece.top + PIECE_WAIT_SIZE;
	pWaitPiece->SetPos(rcPiece);
	pWaitPiece->SetVisible(true);
}

void CGameFrameWnd::HideWaitingPiece()
{
	CControlUI* pWaitPiece = static_cast<CControlUI*>(m_PaintManager.FindControl(GetWaitPieceName()));
	pWaitPiece->SetVisible(false);
}

void CGameFrameWnd::UpdateButton()
{
	static_cast<CControlUI*>(m_PaintManager.FindControl(L"BtnStart"))->SetEnabled(!(m_IsInGame || m_IsInRound || m_IsReady));
	static_cast<CControlUI*>(m_PaintManager.FindControl(L"BtnConcede"))->SetEnabled(m_IsInGame);
}

void CGameFrameWnd::SetGameStart()
{
	m_IsInGame = true;
	if (m_pUser->GetPieceType() == PIECE_BLACK)
	{
		m_pUser->StartRound();
	}
	else
	{
		m_pRival->StartRound();
	}
	UpdateButton();
}

void CGameFrameWnd::SetReady(bool isRival)
{
	if (isRival)
	{
		m_pRival->SetReady();
	}
	else
	{
		m_IsReady = true;
		m_pUser->SetReady();

		UINT nRoll = rand();
		m_pGobangManager->UserReady(&nRoll);
	}
	UpdateButton();
}

void CGameFrameWnd::SetPieceOrder(bool IsUserFirst)
{
	m_IsInGame = true;
	if (IsUserFirst)
	{
		m_pUser->SetPieceType(PIECE_BLACK);
		m_pRival->SetPieceType(PIECE_WHITE);

		m_IsInRound = true;
		m_pUser->StartRound();
	}
	else
	{
		m_pUser->SetPieceType(PIECE_WHITE);
		m_pRival->SetPieceType(PIECE_BLACK);
		m_pRival->StartRound();
	}
}

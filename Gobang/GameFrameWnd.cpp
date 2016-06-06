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

CControlUI* CGameFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	MessageBox(NULL, pstrClass, NULL, NULL);
	if (_tcsicmp(pstrClass, L"GameUser") == 0)
	{
		return new CGameUserUI();
	}
	return NULL;
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
	}
	if (msg.sType == L"mousemove")
	{
		if (IsCurrentPointAvailable(msg)) {
			ShowWaitingPiece(msg);
		}
		else
		{
			HideWaitingPiece();
		}
	}
	if (msg.sType == L"click")
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(msg.ptMouse));
		RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
		RECT rcBoard = static_cast<CControlUI*>(m_PaintManager.FindControl(L"Board"))->GetPos();

		const LONG dwStart = 27;
		const LONG dwInterval = 39;
		const LONG dwOffset = 30;

		if ((msg.ptMouse.x >= rcBoard.left) && (msg.ptMouse.x <= rcBoard.right) && (msg.ptMouse.y >= rcBoard.top) && (msg.ptMouse.y <= rcBoard.bottom))
		{
			LONG rtLeft = msg.ptMouse.x - rcWrapper.left;
			LONG rtTop = msg.ptMouse.y - rcWrapper.top;

			UINT dx = (UINT)((rtLeft - dwStart) * 1.0 / dwInterval + 0.5);
			UINT dy = (UINT)((rtTop - dwStart) * 1.0 / dwInterval + 0.5);

			LONG ptLeft = dx * dwInterval + dwStart;
			LONG ptTop = dy * dwInterval + dwStart;

			if ((abs(rtLeft - ptLeft) <= dwOffset) && (abs(rtTop - ptTop) <= dwOffset)) {
				CControlUI* pWhitePiece = static_cast<CControlUI*>(m_PaintManager.FindControl(L"WhitePiece"));
				LONG dwPieceWidth = pWhitePiece->GetWidth();
				LONG dwPieceHeight = pWhitePiece->GetHeight();

				RECT rcPiece;
				rcPiece.left = rcWrapper.left + ptLeft - dwPieceWidth / 2;
				rcPiece.top = rcWrapper.top + ptTop - dwPieceHeight / 2;
				rcPiece.right = rcPiece.left + dwPieceWidth;
				rcPiece.bottom = rcPiece.top + dwPieceHeight;
				pWhitePiece->SetPos(rcPiece);
			}
		}
	}
}

void CGameFrameWnd::InitWindow()
{
	CenterWindow();

	m_pUser = static_cast<CGameUserUI*>(m_PaintManager.FindControl(L"User"));
	m_pRival = static_cast<CGameUserUI*>(m_PaintManager.FindControl(L"Rival"));
}

LRESULT CGameFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:
		lRes = OnCreate(uMsg, wParam, lParam, bHandled);
		break;
	case WM_CLOSE:
		lRes = OnClose(uMsg, wParam, lParam, bHandled);
		break;
	case WM_DESTROY:
		lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCACTIVATE:
		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCCALCSIZE:
		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCPAINT:
		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCHITTEST:
		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SIZE:
		lRes = OnSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_GETMINMAXINFO:
		lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SYSCOMMAND:
		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
		break;
	default:
		bHandled = FALSE;
	}
	if (bHandled)
	{
		return lRes;
	}
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CGameFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	// CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("game.xml"), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	return 0;
}

LRESULT CGameFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	bHandled = FALSE;

	return 0;
}

LRESULT CGameFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;

	return 0;
}

LRESULT CGameFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CGameFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}

LRESULT CGameFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}

LRESULT CGameFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption;
	rcCaption.left = 0;
	rcCaption.right = 0;
	rcCaption.top = 0;
	rcCaption.bottom = 58;

	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("ContainerUI")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("EditUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CGameFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CGameFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CGameFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("BtnMaximize")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("BtnRestore")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("BtnMaximize")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("BtnRestore")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

BOOL CGameFrameWnd::IsPtInRect(POINT &pt, RECT &rc)
{
	return ((pt.x >= rc.left) && (pt.x <= rc.right) && (pt.y >= rc.top) && (pt.y <= rc.bottom));
}

void CGameFrameWnd::Startup(const UserProfile &user, const UserProfile &rival)
{
	if (!m_pUser)
	{
		m_pUser = static_cast<CGameUserUI*>(m_PaintManager.FindControl(L"User"));
	}
	if (!m_pRival)
	{
		m_pRival = static_cast<CGameUserUI*>(m_PaintManager.FindControl(L"Rival"));
	}
	
	m_pUser->SetUserProfile(user);
	m_pUser->SetReady();
	m_pUser->StartRound();
	m_pRival->SetUserProfile(rival);
}

BOOL CGameFrameWnd::InBoardPointArea(TNotifyUI &msg)
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

BOOL CGameFrameWnd::GetBoardCoordinate(TNotifyUI &msg, POINT &pt)
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

BOOL CGameFrameWnd::IsCurrentPointAvailable(TNotifyUI &msg)
{

	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(msg.ptMouse));
	RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
	RECT rcBoard = static_cast<CControlUI*>(m_PaintManager.FindControl(L"Board"))->GetPos();

	POINT pt;
	return (GetBoardCoordinate(msg, pt) && (m_pGobangManager->IsPointAvailable(pt)));
}

void CGameFrameWnd::ShowWaitingPiece(TNotifyUI &msg)
{
	RECT rcWrapper = static_cast<CControlUI*>(m_PaintManager.FindControl(L"BoardWrapper"))->GetPos();
	CControlUI* pWhitePiece = static_cast<CControlUI*>(m_PaintManager.FindControl(L"WhitePieceWait"));
	
	POINT pt;
	if (!GetBoardCoordinate(msg, pt))
	{
		return;
	}

	LONG dwPieceWidth = pWhitePiece->GetWidth();
	LONG dwPieceHeight = pWhitePiece->GetHeight();
	LONG ptLeft = pt.x * BOARD_PT_INTERVAL + BOARD_PT_START;
	LONG ptTop = pt.y * BOARD_PT_INTERVAL + BOARD_PT_START;

	RECT rcPiece;
	rcPiece.left = rcWrapper.left + ptLeft - dwPieceWidth / 2;
	rcPiece.top = rcWrapper.top + ptTop - dwPieceHeight / 2;
	rcPiece.right = rcPiece.left + dwPieceWidth;
	rcPiece.bottom = rcPiece.top + dwPieceHeight;
	pWhitePiece->SetPos(rcPiece);
	pWhitePiece->SetVisible(true);
}

void CGameFrameWnd::HideWaitingPiece()
{
	CControlUI* pWhitePiece = static_cast<CControlUI*>(m_PaintManager.FindControl(L"WhitePieceWait"));
	pWhitePiece->SetVisible(false);
}

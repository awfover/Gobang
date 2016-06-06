#include "LobbyFrameWnd.h"


CLobbyFrameWnd::CLobbyFrameWnd(CGobangManager *pGobangManager)
{
	m_pGobangManager = pGobangManager;
	m_DialogBuilder.Create(L"UserListItem.xml", (UINT)0, NULL, &m_PaintManager);
}


LPCTSTR CLobbyFrameWnd::GetWindowClassName() const
{
	return _T("UIMainFrame");
}

void CLobbyFrameWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

void CLobbyFrameWnd::Notify(TNotifyUI &msg)
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
		else if (dsControlName == L"ItemInvite")
		{
			LPCTSTR szIndex = msg.pSender->GetUserData().GetData();
			UINT uIndex = _ttoi(szIndex);
			m_pGobangManager->TryInviteUser(m_OnlineUserList.at(uIndex));
			return;
		}
	}
}

LRESULT CLobbyFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CLobbyFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_PaintManager.Init(m_hWnd);
	CDialogBuilder builder;
	// CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("lobby.xml"), (UINT)0, NULL, &m_PaintManager);
	ASSERT(pRoot && "Failed to parse XML");
	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	return 0;
}

LRESULT CLobbyFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	bHandled = FALSE;

	return 0;
}

LRESULT CLobbyFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	::PostQuitMessage(0L);
	bHandled = FALSE;

	return 0;
}

LRESULT CLobbyFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CLobbyFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}

LRESULT CLobbyFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	return 0;
}

LRESULT CLobbyFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLobbyFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLobbyFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLobbyFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CLobbyFrameWnd::Update(const UserProfile &user, const vector<UserProfile> &list)
{
	UpdateUserProfile(user);
	UpdateOnlineUserList(list);
}

void CLobbyFrameWnd::UpdateUserProfile(const UserProfile &user)
{
	m_UserProfile = user;
	static_cast<CControlUI*>(m_PaintManager.FindControl(_T("UserAvatar")))->SetBkImage(m_UserProfile.GetAvatar(L"lg"));
	static_cast<CControlUI*>(m_PaintManager.FindControl(_T("UserName")))->SetText(m_UserProfile.GetName());
	static_cast<CControlUI*>(m_PaintManager.FindControl(_T("UserIP")))->SetText(m_UserProfile.GetIp());
	static_cast<CControlUI*>(m_PaintManager.FindControl(_T("UserTotalGames")))->SetText(m_UserProfile.GetTotalGames());
	static_cast<CControlUI*>(m_PaintManager.FindControl(_T("UserWinProbility")))->SetText(m_UserProfile.GetWinProbility());
}

void CLobbyFrameWnd::UpdateOnlineUserList(const vector<UserProfile> &list)
{
	static_cast<CListUI*>(m_PaintManager.FindControl(_T("UserList")))->RemoveAll();

	m_OnlineUserList = list;
	UINT index = 0;
	for (auto i = m_OnlineUserList.begin(); i != m_OnlineUserList.end(); i++)
	{
		CControlUI *pItem = static_cast<CControlUI*>(m_DialogBuilder.Create(NULL, &m_PaintManager));
		static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pItem, _T("ItemAvatar")))->SetBkImage(i->GetAvatar(L"sm"));
		static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pItem, _T("ItemName")))->SetText(i->GetName());
		static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pItem, _T("ItemIP")))->SetText(i->GetIp());
		static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pItem, _T("ItemWinProbility")))->SetText(i->GetWinProbility());

		LPTSTR szIndex = new WCHAR[5];
		wsprintf(szIndex, L"%u", index++);
		static_cast<CControlUI*>(m_PaintManager.FindSubControlByName(pItem, _T("ItemInvite")))->SetUserData(szIndex);
		
		static_cast<CListUI*>(m_PaintManager.FindControl(_T("UserList")))->Add(pItem);
	}
}
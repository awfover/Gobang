#include "GameUserUI.h"


CGameUserUI::CGameUserUI(CPaintManagerUI &PaintManager) :
	m_PaintManager(PaintManager),
	m_UserProfile(),
	m_RemainSecs(0),
	m_WinTimes(0),
	m_IsReady(false),
	m_IsInRound(false)
{
}

LPCTSTR CGameUserUI::GetClass() const
{
	return L"VerticalLayoutUI";
}

LPVOID CGameUserUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, GameUserInterface) == 0)
	{
		return static_cast<CGameUserUI*>(this);
	}
	else
	{
		return CVerticalLayoutUI::GetInterface(pstrName);
	}
}

CControlUI *CGameUserUI::CreateControl(LPCTSTR pstrName)
{
	return NULL;
}

void CGameUserUI::DoEvent(TEventUI &event)
{
	if (event.Type == UIEVENT_TIMER)
	{
		m_RemainSecs -= 1;
		UpdateRemainSecs();

		if (m_RemainSecs == 0)
		{
			m_pManager->KillTimer(this);
		}
	}

	CVerticalLayoutUI::DoEvent(event);
}

void CGameUserUI::Update()
{
	UpdateProfile();
	UpdateRemainSecs();
	UpdateWinTimes();
	UpdateReady();
}

void CGameUserUI::UpdateProfile()
{
	static_cast<CControlUI*>(m_pManager->FindSubControlByName(this, L"ItemAvatar"))->SetBkImage(m_UserProfile.GetAvatar(L"md"));
	static_cast<CControlUI*>(m_pManager->FindSubControlByName(this, L"ItemName"))->SetText(m_UserProfile.GetName());
}

void CGameUserUI::UpdateRemainSecs()
{
	LPTSTR szRemainSecs = new WCHAR[20];
	// wsprintf(szRemainSecs, L"倒计时  %02u:%02u", m_RemainSecs / 60, m_RemainSecs % 60);
	wsprintf(szRemainSecs, L"%02u:%02u", m_RemainSecs / 60, m_RemainSecs % 60);
	static_cast<CControlUI*>(m_pManager->FindSubControlByName(this, L"ItemRemainSecs"))->SetText(szRemainSecs);
	delete szRemainSecs;
}

void CGameUserUI::UpdateWinTimes()
{
	LPTSTR szWinTimes = new WCHAR[10];
	// wsprintf(szWinTimes, L"比分  %u", m_WinTimes);
	wsprintf(szWinTimes, L"%u", m_WinTimes);
	static_cast<CControlUI*>(m_pManager->FindSubControlByName(this, L"ItemWinTimes"))->SetText(szWinTimes);
	delete szWinTimes;
}

void CGameUserUI::UpdateReady()
{
	LPCTSTR szReady = m_IsReady ? L"已准备" : L"未准备";
	static_cast<CControlUI*>(m_pManager->FindSubControlByName(this, L"ItemReadyText"))->SetText(szReady);
}

void CGameUserUI::SetUserProfile(const UserProfile &user)
{
	m_UserProfile = user;
	m_RemainSecs = ONE_ROUND_TIME;
	m_WinTimes = 0;
	m_IsInRound = false;
	m_IsReady = false;
	Update();
}

void CGameUserUI::SetPieceType(PIECE_TYPE type)
{
	m_PieceType = type;
}

void CGameUserUI::SetReady()
{
	m_IsReady = true;
	UpdateReady();
}

void CGameUserUI::StartRound()
{
	m_pManager->SetTimer(this, 0, 1000);
}

void CGameUserUI::EndRound()
{
	m_pManager->KillTimer(this);
}

PIECE_TYPE CGameUserUI::GetPieceType()
{
	return m_PieceType;
}
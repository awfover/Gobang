#pragma once
#include "StdAfx.h"
#include "GobangManager.h"
#include "GameUserUI.h"

#define BOARD_PT_START 27
#define BOARD_PT_INTERVAL 39
#define BOARD_PT_OFFSET 16

#define PIECE_SIZE 45
#define PIECE_WAIT_SIZE 33
#define PIECE_WHITE_IMAGE L"res\\white-piece.png"
#define PIECE_BLACK_IMAGE L"res\\black-piece.png"

#define PIECE_WHITE_WAIT L"WhitePieceWait"
#define PIECE_BLACK_WAIT L"BlackPieceWait"

class CGobangManager;

class CGameFrameWnd :
	public WindowImplBase
{
public:
	CGameFrameWnd(CGobangManager *lpGobangManager);

	LPCTSTR GetWindowClassName() const;
	void Notify(TNotifyUI &);

	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();

	void InitWindow();
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	CPaintManagerUI* GetMainWndPaintManager();

	void OnFinalMessage(HWND /*hWnd*/);

	void Startup(const UserProfile &, const UserProfile &);

	bool IsPtInRect(POINT &, RECT &);

	void SetReady(bool = false);
	void SetGameStart();
	void PlacePiece(POINT &);
	void SetPieceOrder(bool);

private:
	void ClearBoard();
	void UpdateButton();

	bool InBoardPointArea(TNotifyUI &);
	bool GetBoardCoordinate(TNotifyUI &, POINT &);
	bool IsCurrentPointAvailable(TNotifyUI &);

	void PlacePiece(TNotifyUI &);
	void PlacePiece(POINT &, PIECE_TYPE);

	LPCTSTR GetPieceImage(PIECE_TYPE);
	LPCTSTR GetWaitPieceName();

	void ShowWaitingPiece(TNotifyUI &);
	void HideWaitingPiece();

	CGobangManager *m_pGobangManager;

	CGameUserUI *m_pUser;
	CGameUserUI *m_pRival;

	bool m_IsReady;
	bool m_IsInGame;
	bool m_IsInRound;
};


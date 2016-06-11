#pragma once
#include "StdAfx.h"

#define BOARD_SIZE 15
#define MAX_SUM 5

const int DX[] = { 0, -1, -1, -1 };
const int DY[] = { -1, -1, 0, 1 };
const int DZ[] = { -1, 1 };

class GameBoard
{
public:
	GameBoard();

	void Clear();
	int Place(UINT, UINT, UINT = 0);
	bool IsValid(int, int);
	bool IsAvailable(UINT, UINT);

private:
	int m_Pieces[BOARD_SIZE][BOARD_SIZE];
	UINT m_PieceSums[BOARD_SIZE][BOARD_SIZE][4];

	UINT Maintain(UINT, UINT);
};
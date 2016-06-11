#include "GameBoard.h"

GameBoard::GameBoard()
{
	Clear();
}

void GameBoard::Clear()
{
	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			m_Pieces[x][y] = -1;
		}
	}

	memset(&m_PieceSums, 0, sizeof(m_PieceSums));
}

bool GameBoard::IsValid(int x, int y)
{
	return ((x > 0) && (x < BOARD_SIZE) && (y > 0) && (y < BOARD_SIZE));
}

bool GameBoard::IsAvailable(UINT x, UINT y)
{
	return (IsValid(x, y) && (m_Pieces[x][y] == -1));
}

int GameBoard::Place(UINT x, UINT y, UINT type)
{
	if ((type > 1) || !IsAvailable(x, y))
	{
		return -1;
	}

	m_Pieces[x][y] = type;
	return Maintain(x, y);
}

UINT GameBoard::Maintain(UINT x, UINT y)
{
	UINT i, j, k;
	for (i = 0; i < 4; i++)
	{
		m_PieceSums[x][y][i] = 1;

		for (j = 0; j < 2; j++)
		{
			int dx = DX[i] * DZ[j];
			int dy = DY[i] * DZ[j];

			if (IsValid(x + dx, y + dy) && (m_Pieces[x][y] == m_Pieces[x + dx][y + dy]))
			{
				m_PieceSums[x][y][i] += m_PieceSums[x + dx][y + dy][i];
			}
		}

		if (m_PieceSums[x][y][i] >= MAX_SUM)
		{
			return 1;
		}

		for (j = 0; j < 2; j++)
		{
			int dx = DX[i] * DZ[j];
			int dy = DY[i] * DZ[j];

			if (IsValid(x + dx, y + dy) && (m_Pieces[x][y] == m_Pieces[x + dx][y + dy]))
			{
				for (k = m_PieceSums[x + dx][y + dy][i]; k; k--)
				{
					if (IsValid(x + dx * k, y + dy * k))
					{
						m_PieceSums[x + dx * k][y + dy * k][i] = m_PieceSums[x][y][i];
					}
				}
			}
		}
	}

	return 0;
}
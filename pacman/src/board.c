#include <stdio.h>
#include <stdlib.h>

#include "board.h"

bool is_valid_square(Board *board, int x, int y)
{
	if(Multi_flags() == 1) // # 9 Dong : 2p 맵 연동을 위한 추가
	{
		if (x < 0 || y < 0) return false;
		if (x >= 56 || y >= 31) return false;
		if(x==-1&&y!=14) return false;
		if(x==28&&y!=14) return false;
		if(x==56&&y!=14) return false;
		return board->boardSquares[x][y].walkable;
	}
	else
	{
		if (x < 0 || y < 0) return false;
		if (x >= 28 || y >= 31) return false;
		if(x==0&&y!=14) return false;
		if(x==27&&y!=14) return false;
		return board->boardSquares[x][y].walkable;
	}
}

bool is_tele_square(int x, int y)
{
	if(Multi_flags() == 1) // # 9 Dong : 2p 맵 연동을 위한 추가
	{
		if (x == -1 && y == 14) return true;
		else if (x == 56 && y == 14) return true;
		else return false;
	}
	else
	{
		if (x == -1 && y == 14) return true;
		else if (x == 28 && y == 14) return true;
		else return false;
	}
}

bool is_ghost_noup_square(int x, int y)
{
	//squares are:
	// (12, 22) (14, 22)
	// (12, 10) (14, 10)
	return (x == 12 && y == 22) || (x == 15 && y == 22) ||
	       (x == 12 && y == 10) || (x == 15 && y == 10);
}

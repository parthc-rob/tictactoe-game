#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define OCC_PLAYER_1    1
#define OCC_PLAYER_2   -1
#define UNOCCUPIED      0
#define BOARDSIZE       3

#include<array>

//TODO: can use enum as well

//initialize n*n gameboard upon input // generality

class GameBoard {

	std::array <int, BOARDSIZE*BOARDSIZE> current_board;
public:
	GameBoard();
	void resetBoard();
	int isGameOver();
	int playMove(string userInput, int playerNumber);
};



#endif GAMEBOARD_H
#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define OCC_PLAYER_1    1
#define OCC_PLAYER_2   -1
#define OCC_PLAYER_MIX  0
#define UNOCCUPIED      0
#define GAME_OVER_1		1
#define GAME_OVER_2	   -1
#define GAME_NOT_OVER	0
#define BOARDSIZE       3

#include<array>
#include<string>

//TODO: can use enum as well

//initialize n*n gameboard upon input // generality
typedef std::array <int, BOARDSIZE*BOARDSIZE> board_type;

class GameBoard {

	// std::array <int, BOARDSIZE*BOARDSIZE> current_board;
	board_type 					current_board;
	board_type					player_1_board;
	board_type					player_2_board;
	int 						who_won;
	int 						is_game_over;
	std::array<int, BOARDSIZE> 	is_row_filled;
	std::array<int, BOARDSIZE> 	is_column_filled;
	std::array<int, 2>			is_diagonal_filled; //assuming 2-D board
	char 						colStart = 'A'; //zero-th column name

public:
	GameBoard();
	void 	resetBoard();
	int 	checkFilledRow( int );
	int 	checkFilledColumn( int );
	int 	checkLeftRightDiagonal();
	int 	checkRightLeftDiagonal();
	bool	isBoardFilled();
	int 	isGameOver();
	int 	playMove(std::string userInput, int playerNumber);
	int 	spin();
	void	showBoard();
	int     whoWon();
};



#endif
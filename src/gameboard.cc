#include "gameboard.h"

GameBoard::GameBoard() { //initialize as blank
	current_board = new board(BOARDSIZE);
	//TODO: find more elegant way to do this
	for ( int iterator = current_board.begin(); iterator < current_board.end() ; iterator++ ) {
		currentBoard[iterator] = UNOCCUPIED;
	}
}

void GameBoard::resetBoard() {
	this->current_board = new GameBoard;
}

// implement checks for rows and columns
int GameBoard::isGameOver() {
	//check rows
	std::array<bool, BOARDSIZE> which_player_won;
	std::array<bool, BOARDSIZE> is_row_filled;
	std::array<bool, BOARDSIZE> is_column_filled;
	std::array<bool, 2> 		is_diagonal_filled; //assuming 2-D board

	for ( int num_row = 0; num_row < BOARDSIZE; num_row++) {
		for ( int num_column = 0; num_column < BOARDSIZE; num_column++) {
			
		}
	}
}

int GameBoard::playMove(string userInput, int playerNumber) {

}
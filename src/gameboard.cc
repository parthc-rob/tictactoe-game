#include "gameboard.h"

////////// Utilities
namespace ticTacUtils {
	board_type _reset_board(board_type& current_board) {
		for ( auto iterator = current_board.begin(); iterator < current_board.end() ; iterator++ ) {
			current_board.at(*iterator) = OCC_PLAYER_1;
		}
		return current_board;
	}

	int _random(int min, int max) {
		std::srand(std::time(nullptr));
		try {
			return min + (std::rand() % static_cast<int>(max - min));
		}
		catch (std::exception &e) {
			return -1;
		}
	}

	template<typename T>
	typename T::iterator _get_random_from_list(T & list) {
		typename T::iterator it = list.begin();
		try {
			std::advance(it,_random(0,list.size()-1));
			return it;
		}
		catch (std::exception& e) {
			return list.begin();
		}
	}

	cell convertIndexToCell(int index, int board_size) {
		cell result{ (static_cast<int>(index / board_size)) , (static_cast<int>(index % board_size)) };
		return result;
	}
	int convertCellToIndex(cell rowCol, int board_size) {
		return static_cast<int>(rowCol[0]*board_size + rowCol[1]);
	}
}

////////// Class Definitions
GameBoard::GameBoard(bool is_random_bot_active) {
	//// TODO: find more elegant way to do this
	this->current_board = ticTacUtils::_reset_board(this->current_board);
	this->current_board = { UNOCCUPIED };
	this->resetEmptyCells();

	//// defaults
	this->who_won 				= OCC_PLAYER_MIX;
	this->is_game_over 			= GAME_NOT_OVER;
	this->is_line_filled		= { true };
	this->is_random_bot_active	= is_random_bot_active;
}
void GameBoard::resetBoard() {
	this->current_board = ticTacUtils::_reset_board(this->current_board);
	this->resetEmptyCells();
}
void GameBoard::resetEmptyCells() {
	this->emptyCells.clear();
	cell current_cell;
	for (int i = 0; i<BOARDSIZE; i++) {
		current_cell = ticTacUtils::convertIndexToCell(i, BOARDSIZE);
		this->emptyCells.push_back(current_cell);
	}
}

void GameBoard::showBoard() {
	std::cout<<"Row/Col\t: ";
	for ( int col=0; col<BOARDSIZE; col++) {
		std::cout<<char(this->colStart + col)<<"\t";
	}
	std::cout<<std::endl;

	for ( int row=0; row<BOARDSIZE; row++) {
		std::cout<<row+1<<"\t: ";
		for (int col=0; col<BOARDSIZE; col++) {
			std::cout<<this->playerSymbol(this->current_board.at(row*BOARDSIZE + col))<<"\t";
		}
		std::cout<<std::endl;
	}
}

int GameBoard::checkFilledRow( int num_row ) {

	int row_ownership = OCC_PLAYER_MIX;
	for ( auto num_column = 0; num_column < BOARDSIZE; num_column++ ) {
		if ( this->player_1_board.at(num_column + num_row*BOARDSIZE) != UNOCCUPIED ) {
			row_ownership += OCC_PLAYER_1;
		}
		if ( this->player_2_board.at(num_column + num_row*BOARDSIZE) != UNOCCUPIED ) {
			row_ownership += OCC_PLAYER_2;
		}
	}
	return int(row_ownership/BOARDSIZE); // return 0 if no full ownership
}

int GameBoard::checkFilledColumn( int num_column ) {
	int column_ownership = OCC_PLAYER_MIX;
	for ( auto num_row = 0; num_row < BOARDSIZE; num_row++ ) {
		if ( this->player_1_board.at(num_column + num_row*BOARDSIZE) != UNOCCUPIED ) {
			column_ownership += OCC_PLAYER_1;
		}
		if ( this->player_2_board.at(num_column + num_row*BOARDSIZE) != UNOCCUPIED ) {
			column_ownership += OCC_PLAYER_2;
		}
	}
	return int(column_ownership/BOARDSIZE); // return 0 if no full ownership
}

int GameBoard::checkLeftRightDiagonal() {
	int diagonal_ownership = OCC_PLAYER_MIX;
	for ( int num_cell = 0; num_cell < BOARDSIZE; num_cell++ ) {
		if ( this->player_1_board.at(num_cell + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			diagonal_ownership += OCC_PLAYER_1;
		}
		if ( this->player_2_board.at(num_cell + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			diagonal_ownership += OCC_PLAYER_2;
		}
	}
	return int(diagonal_ownership/BOARDSIZE); // return 0 if no full ownership
}

int GameBoard::checkRightLeftDiagonal() {
	int off_diagonal_ownership = OCC_PLAYER_MIX;
	for ( int num_cell = 0; num_cell < BOARDSIZE; num_cell++ ) {
		if ( this->player_1_board.at(BOARDSIZE - 1 - num_cell + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			off_diagonal_ownership += OCC_PLAYER_1;
		}
		if ( this->player_2_board.at(BOARDSIZE - 1 - num_cell + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			off_diagonal_ownership += OCC_PLAYER_2;
		}
	}
	return int(off_diagonal_ownership/BOARDSIZE); // return 0 if no full ownership
}

bool GameBoard::isBoardFilled() {
	for ( int num_row = 0; num_row < BOARDSIZE; num_row++) {
		for ( int num_column = 0; num_column < BOARDSIZE; num_column++) {
			if(current_board.at(num_row*BOARDSIZE + num_column) == UNOCCUPIED) {
				return false;
			}
		}
	}
	return true;
}

int GameBoard::isGameOver() {
	for ( int num_row = 0; num_row < BOARDSIZE; num_row++) {
		for ( int num_column = 0; num_column < BOARDSIZE; num_column++) {
			if (
				this->current_board.at(num_column + num_row*BOARDSIZE) != UNOCCUPIED
				) {
				std::cout<<"\n cell check : "<<num_row<<num_column<<std::endl;
				this->is_line_filled[num_row] =
					this->checkFilledRow(num_row);

				this->is_line_filled[BOARDSIZE + num_column] =
					this->checkFilledColumn(num_column);
			}
		}
	}
	this->is_line_filled[2*BOARDSIZE] =
		this->checkLeftRightDiagonal();

	this->is_line_filled[2*BOARDSIZE + 1] =
		this->checkRightLeftDiagonal();

	int value = 0;
	for ( int iterator = 0; iterator < this->is_line_filled.size(); iterator++ ) {
		value = this->is_line_filled[iterator];
		if( value != OCC_PLAYER_MIX ) {
			this->who_won = value;
			return value;
		}
	}

	if( this->isBoardFilled() ) {
		this->who_won = OCC_PLAYER_MIX; //no-one
		return 1;
	}
	return GAME_NOT_OVER;
}


cell GameBoard::processKeyboardInput(std::string userInput) {
	cell rowCol;

	rowCol[0] = int(userInput.at(0)) - int('1');
	if ( (int)userInput.at(1) >= (int)'A' && (int)userInput.at(1) <= (int)'A'+BOARDSIZE-1) {
		rowCol[1] = int(userInput.at(1)) - int('A');
	}
	else if ( (int)userInput.at(1) >= 'a' && (int)userInput.at(1) <= (int)'a'+BOARDSIZE-1) {
		rowCol[1] = int(userInput.at(1)) - int('a');
	}
	else {
		rowCol[1] = -1;
	}
	return rowCol;
}

int GameBoard::playMove(cell inputCell, int playerNumber) {
	int row = inputCell[0], col = inputCell[1];

	if(row >=0 && row < BOARDSIZE &&
		col >=0 && col < BOARDSIZE &&
		this->current_board.at(row*BOARDSIZE + col) == UNOCCUPIED) {
		if( playerNumber == 1) {
			this->player_1_board.at(row*BOARDSIZE + col) = OCC_PLAYER_1;
			this->current_board.at(row*BOARDSIZE + col)  = OCC_PLAYER_1;
		}
		else if( playerNumber == 2) {
			this->player_2_board.at(row*BOARDSIZE + col) = OCC_PLAYER_2;
			this->current_board.at(row*BOARDSIZE + col)  = OCC_PLAYER_2;
		}
		else {
			std::cout<<"\n Invalid player Number! \n";
			return 0;
		}
	}
	else {
		if (this->current_board.at(row*BOARDSIZE + col) != UNOCCUPIED) {
			std::cout<<"\n Invalid cell number! Must be unoccupied!\n";
		}
		else {
			std::cout<<"\n Invalid cell number! Must be in format RowColumn [e.g. 1A, 2b, 3C ]\n";
		}
		return 0;
	}
	int size = this->emptyCells.size();
	this->emptyCells.remove(inputCell);
	return 1;
}


int GameBoard::playMove(std::string userInput, int playerNumber) {
	cell rowCol = this->processKeyboardInput(userInput);
	return this->playMove(rowCol, playerNumber);
}

int GameBoard::playRandomMove(int playerNumber) {
	// int randomIndex = ticTacUtils::_random(0,this->emptyCells.size()-1c);

	// cell rowCol;
	// rowCol[1] = cell_number % BOARDSIZE;
	// rowCol[0] = static_cast<int>(cell_number / BOARDSIZE);
	return this->playMove(*ticTacUtils::_get_random_from_list(this->emptyCells), playerNumber);
}

int GameBoard::spin() {
	return 0;
}

char GameBoard::whoWon() {
	return playerSymbol(this->who_won);
}

char GameBoard::noOne() {
	return this->no_one;
}

char GameBoard::playerSymbol(int playerNumber) {
	if (playerNumber == OCC_PLAYER_1) {
		return '0';
	}
	if (playerNumber == OCC_PLAYER_2) {
		return 'X';
	}
	else {
		return ' ';
	}
}
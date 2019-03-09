#include "gameboard.h"

////////// Utilities
ticTacUtils::board_type_t ticTacUtils::reset_board(ticTacUtils::board_type_t& current_board) {
	for ( auto iterator = current_board.begin(); iterator < current_board.end() ; iterator++ ) {
		current_board.at(*iterator) = ticTacUtils::NONE;
	}
	return current_board;
}

int ticTacUtils::random(int min, int max) {
	std::srand(std::time(nullptr));
	try {
		return min + (std::rand() % static_cast<int>(max - min));
	}
	catch (std::exception &e) {
		return -1;
	}
}

template<typename T> typename T::iterator ticTacUtils::get_random_from_list(T & list) {
		typename T::iterator it = list.begin();
		try {
			std::advance(it,random(0,list.size()-1));
			return it;
		}
		catch (std::exception& e) {
			return list.begin();
		}
	}

ticTacUtils::cell_t ticTacUtils::convertIndexToCell(int index, int board_size) {
	ticTacUtils::cell_t result{ (static_cast<int>(index / board_size)) , (static_cast<int>(index % board_size)) };
	return result;
}
int ticTacUtils::convertCellToIndex(ticTacUtils::cell_t rowCol, int board_size) {
	return static_cast<int>(rowCol[0]*board_size + rowCol[1]);
}
int ticTacUtils::convertRowColToIndex(int row, int column, int board_size) {
	return row*board_size + column;
}

////////// Class Definitions
GameBoard::GameBoard() {
	//// TODO: find more elegant way to do this
	this->current_board = ticTacUtils::reset_board(this->current_board);
	this->current_board = { ticTacUtils::NONE };
	this->resetEmptyCells();

	//// defaults
	this->who_won 				= ticTacUtils::NONE;
	this->is_game_over 			= GAME_NOT_OVER;
	this->line_occupancy		= { ticTacUtils::NONE };
}
void GameBoard::resetBoard() {
	this->current_board = ticTacUtils::reset_board(this->current_board);
	this->resetEmptyCells();
}
void GameBoard::resetEmptyCells() {
	this->emptyCells.clear();
	ticTacUtils::cell_t current_cell;
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
			auto current_cell =
				this->current_board.at(ticTacUtils::convertRowColToIndex(row,col,BOARDSIZE));
			std::cout<<ticTacUtils::player_symbol[current_cell]<<"\t";
		}
		std::cout<<std::endl;
	}
}

ticTacUtils::player_enum GameBoard::checkFilledRow( int num_row ) {

	int row_ownership = (int)ticTacUtils::NONE;
	for ( auto num_column = 0; num_column < BOARDSIZE; num_column++ ) {
		row_ownership += (int)this->current_board.at(
			ticTacUtils::convertRowColToIndex(num_row,num_column,BOARDSIZE)
			);
	}
	return (ticTacUtils::player_enum)static_cast<int>(row_ownership/BOARDSIZE);
}

ticTacUtils::player_enum GameBoard::checkFilledColumn( int num_column ) {
	int column_ownership = (int)ticTacUtils::NONE;
	for ( auto num_row = 0; num_row < BOARDSIZE; num_row++ ) {
		column_ownership += this->current_board.at(
			ticTacUtils::convertRowColToIndex(num_row,num_column,BOARDSIZE)
			);
	}
	return (ticTacUtils::player_enum)static_cast<int>(column_ownership/BOARDSIZE);
}

ticTacUtils::player_enum GameBoard::checkLeftRightDiagonal() {
	int diagonal_ownership = ticTacUtils::NONE;
	for ( int num_cell = 0; num_cell < BOARDSIZE; num_cell++ ) {
		diagonal_ownership += this->current_board.at(
			ticTacUtils::convertRowColToIndex(num_cell,num_cell,BOARDSIZE)
			);
	}
	return (ticTacUtils::player_enum)static_cast<int>(diagonal_ownership/BOARDSIZE);
}

ticTacUtils::player_enum GameBoard::checkRightLeftDiagonal() {
	int off_diagonal_ownership = ticTacUtils::NONE;
	for ( int num_cell = 0; num_cell < BOARDSIZE; num_cell++ ) {
		off_diagonal_ownership += this->current_board.at(
			ticTacUtils::convertRowColToIndex(BOARDSIZE - 1 - num_cell,num_cell,BOARDSIZE)
			);
	}
	return (ticTacUtils::player_enum)static_cast<int>(off_diagonal_ownership/BOARDSIZE);
}

bool GameBoard::isBoardFilled() {
	for ( int num_row = 0; num_row < BOARDSIZE; num_row++) {
		for ( int num_column = 0; num_column < BOARDSIZE; num_column++) {
			if(current_board.at(
				ticTacUtils::convertRowColToIndex(num_row,num_column,BOARDSIZE)
				) == ticTacUtils::NONE) {
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
				this->current_board.at(
					ticTacUtils::convertRowColToIndex(num_row,num_column,BOARDSIZE)
					) != ticTacUtils::NONE
				) {
				this->line_occupancy[num_row] =
					this->checkFilledRow(num_row);

				this->line_occupancy[BOARDSIZE + num_column] =
					this->checkFilledColumn(num_column);
			}
		}
	}
	this->line_occupancy[2*BOARDSIZE] =
		this->checkLeftRightDiagonal();

	this->line_occupancy[2*BOARDSIZE + 1] =
		this->checkRightLeftDiagonal();

	int value = 0;
	for ( int iterator = 0; iterator < this->line_occupancy.size(); iterator++ ) {
		if( line_occupancy[iterator] !=  ticTacUtils::NONE) {
			this->who_won = line_occupancy[iterator];
			return this->who_won;
		}
	}

	if( this->isBoardFilled() ) {
		this->who_won = ticTacUtils::NONE;
		return this->who_won;
	}
	return GAME_NOT_OVER;
}


ticTacUtils::cell_t GameBoard::processKeyboardInput(std::string userInput) {
	ticTacUtils::cell_t rowCol;

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

int GameBoard::playMove(ticTacUtils::cell_t inputCell, bool is_player_0) {
	int row = inputCell[0], col = inputCell[1];

	if(row >=0 && row < BOARDSIZE &&
		col >=0 && col < BOARDSIZE &&
		this->current_board.at(row*BOARDSIZE + col) == ticTacUtils::NONE) {
		if( is_player_0) {
			this->player_0_board.at(row*BOARDSIZE + col) = ticTacUtils::PLAYER_0;
			this->current_board.at(row*BOARDSIZE + col)  = ticTacUtils::PLAYER_0;
		}
		else {
			this->player_X_board.at(row*BOARDSIZE + col) = ticTacUtils::PLAYER_X;
			this->current_board.at(row*BOARDSIZE + col)  = ticTacUtils::PLAYER_X;
		}
	}
	else {
		if (this->current_board.at(row*BOARDSIZE + col) != ticTacUtils::NONE) {
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


int GameBoard::playMove(std::string userInput, bool is_player_0) {
	auto rowCol = this->processKeyboardInput(userInput);
	return this->playMove(rowCol, is_player_0);
}

int GameBoard::playRandomMove(bool is_player_0) {
	// int randomIndex = ticTacUtils::_random(0,this->emptyCells.size()-1c);

	// cell rowCol;
	// rowCol[1] = cell_number % BOARDSIZE;
	// rowCol[0] = static_cast<int>(cell_number / BOARDSIZE);
	return this->playMove(*ticTacUtils::get_random_from_list(this->emptyCells), is_player_0);
}

std::string GameBoard::whoWon() {
	return ticTacUtils::player_name[this->who_won];
}

char GameBoard::noOne() {
	return this->no_one;
}

char GameBoard::playerSymbol(bool is_player_0) {
	if (is_player_0) {
		return '0';
	}
	return 'X';
	// }
	// else {
	// 	return ' ';
	// }
}
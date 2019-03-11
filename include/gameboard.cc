#include "gameboard.h"

////////// Utilities
ticTacUtils::board_type_t ticTacUtils::reset_board(
	ticTacUtils::board_type_t& current_board
	) {
	for ( auto iterator = current_board.begin();
		iterator < current_board.end();
		iterator++ ) {
		current_board.at(*iterator) = ticTacUtils::NONE;
	}
	return current_board;
}

int ticTacUtils::random(int min, int max) {
	std::srand(std::time(nullptr));
	if (max-min > 0) {
		return min + (std::rand() % static_cast<int>(max - min));
	}
	return min;
}

template<typename T> typename T::iterator ticTacUtils::get_random_from_list(
	T & list
	) {
		typename T::iterator it = list.begin();
		try {
			std::advance(it,random(0,list.size()-1));
			return it;
		}
		catch (std::exception& e) {
			return list.begin();
		}
	}

ticTacUtils::cell_t ticTacUtils::convertIndexToCell(
	int index, int board_size
	) {
	ticTacUtils::cell_t result{
		(static_cast<int>(index / board_size)) ,
		(static_cast<int>(index % board_size))
	};
	return result;
}
int ticTacUtils::convertCellToIndex(
	ticTacUtils::cell_t rowCol, int board_size
	) {
	return static_cast<int>(rowCol[0]*board_size + rowCol[1]);
}
int ticTacUtils::convertRowColToIndex(
	int row, int column, int board_size
	) {
	return row*board_size + column;
}
void ticTacUtils::printRowSeparator(int board_size) {
	std::cout<<"\n--------+";
	for ( int col=0; col<board_size; col++) {
		std::cout<<"---+";
	}
	std::cout<<std::endl;
}
bool ticTacUtils::promptReplay() {
	std::string keyIn;
	std::cout<<"\n========\nPlay Again? [y/n]: ";
	std::cin>>keyIn;
	if(keyIn[0] == 'y' || keyIn[0] == 'Y') {
		return true;
	}
	return false;
}

////////// Class Definitions
GameBoard::GameBoard(bool visualize){
	this->current_board = { ticTacUtils::NONE };
	this->resetEmptyCells();
	this->who_won 				= ticTacUtils::NONE;
	this->is_game_over 			= GAME_NOT_OVER;
	this->line_occupancy		= { ticTacUtils::NONE };
	this->visualize				= visualize;
}
void GameBoard::resetBoard() {
	this->current_board = ticTacUtils::reset_board(this->current_board);
	this->resetEmptyCells();
}
void GameBoard::resetEmptyCells() {
	this->emptyCells.clear();
	ticTacUtils::cell_t current_cell;
	for (int i = 0; i<BOARDSIZE*BOARDSIZE; i++) {
		current_cell = ticTacUtils::convertIndexToCell(i, BOARDSIZE);
		this->emptyCells.push_back(current_cell);
	}
}

void GameBoard::showBoard(bool visualize) {
	if (!this->visualize && !visualize) return;
	std::cout<<"Row/Col\t| ";
	for ( int col=0; col<BOARDSIZE; col++) {
		std::cout<<char(this->colStart + col)<<" | ";
	}
	ticTacUtils::printRowSeparator(BOARDSIZE);
	for ( int row=0; row<BOARDSIZE; row++) {
		std::cout<<row+1<<"\t| ";
		for (int col=0; col<BOARDSIZE; col++) {
			auto current_cell =
				this->current_board.at(
					ticTacUtils::convertRowColToIndex(row,col,BOARDSIZE)
					);
			std::cout<<ticTacUtils::player_symbol[current_cell]<<" | ";
		}
		ticTacUtils::printRowSeparator(BOARDSIZE);
	}
}

ticTacUtils::player_enum GameBoard::checkFilledLine(
	ticTacUtils::line_type line
	) {
	int line_ownership = (int)ticTacUtils::NONE; ticTacUtils::cell_t rowCol;
	for ( auto iterator = 0; iterator < BOARDSIZE; iterator++ ) {
		switch (line) {
			case ticTacUtils::LR_DIAGONAL: {
				rowCol = {iterator, iterator}; break;
			}
			case ticTacUtils::RL_DIAGONAL: {
				rowCol = {BOARDSIZE - 1 - iterator, iterator}; break;
			}
			default:
				return ticTacUtils::NONE;
		}
		line_ownership += (int)this->current_board.at(
			ticTacUtils::convertCellToIndex(rowCol,BOARDSIZE)
			);
	}
	return (ticTacUtils::player_enum)static_cast<int>(line_ownership/BOARDSIZE);
}

ticTacUtils::player_enum GameBoard::checkFilledLine(
	ticTacUtils::line_type line, ticTacUtils::cell_t rowCol
	) {
	int line_ownership = (int)ticTacUtils::NONE; int iterator = 0;
	for ( auto iterator = 0; iterator < BOARDSIZE; iterator++ ) {
		switch (line) {
			case ticTacUtils::ROW: {
				line_ownership += (int)this->current_board.at(
					ticTacUtils::convertRowColToIndex(rowCol[0],iterator,BOARDSIZE)
					); break;
			}
			case ticTacUtils::COLUMN: {
				line_ownership += (int)this->current_board.at(
					ticTacUtils::convertRowColToIndex(iterator,rowCol[1],BOARDSIZE)
					); break;
			}
			default:
				return ticTacUtils::NONE;
		}
	}
	return (ticTacUtils::player_enum)static_cast<int>(line_ownership/BOARDSIZE);
}

bool GameBoard::isBoardFilled() {
	for ( int num_row = 0; num_row < BOARDSIZE; num_row++) {
		for ( int num_column = 0; num_column < BOARDSIZE; num_column++) {
			if(current_board.at(
				ticTacUtils::convertRowColToIndex(num_row, num_column, BOARDSIZE)
				) == ticTacUtils::NONE) {
				return false;
			}
		}
	}
	return true;
}

int GameBoard::isGameOver() {
	ticTacUtils::cell_t rowCol = {0,0};
	for (; rowCol[0] < BOARDSIZE; rowCol[0]++) {
		for (; rowCol[1] < BOARDSIZE; rowCol[1]++) {
			if (
				this->current_board.at(
					ticTacUtils::convertCellToIndex(rowCol,BOARDSIZE)
					) != ticTacUtils::NONE
				) {
				this->line_occupancy[ticTacUtils::ROW + rowCol[0]] =
					this->checkFilledLine(ticTacUtils::ROW,rowCol);

				this->line_occupancy[ticTacUtils::COLUMN + rowCol[1]] =
					this->checkFilledLine(ticTacUtils::COLUMN,rowCol);
			}
		}
	}
	this->line_occupancy[ticTacUtils::LR_DIAGONAL] =
		this->checkFilledLine(ticTacUtils::LR_DIAGONAL);

	this->line_occupancy[ticTacUtils::RL_DIAGONAL] =
		this->checkFilledLine(ticTacUtils::RL_DIAGONAL);

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
	if (int(userInput.at(0)) < int('1') + BOARDSIZE) {
		rowCol[0] = int(userInput.at(0)) - int('1');
	}
	else {
		rowCol[0] = -1;
	}
	if (
		(int)userInput.at(1) >= (int)'A'
		&& (int)userInput.at(1) <= (int)'A'+BOARDSIZE-1
		) {
		rowCol[1] = int(userInput.at(1)) - int('A');
	}
	else if (
		(int)userInput.at(1) >= 'a'
		&& (int)userInput.at(1) <= (int)'a'+BOARDSIZE-1
		) {
		rowCol[1] = int(userInput.at(1)) - int('a');
	}
	else {
		rowCol[1] = -1;
	}
	return rowCol;
}

int GameBoard::playMove(ticTacUtils::cell_t inputCell, bool is_player_0) {
	if( inputCell[0] <0 || inputCell[0] >= BOARDSIZE ||
		inputCell[1] <0 || inputCell[1] >= BOARDSIZE) {
		std::cout<<"\n Invalid cell number!"
			<<" Must be in format RowColumn [e.g. 1A, 2b, 3C ]\n";
		return 0;
	}
	else {
		if (
			this->current_board.at(
				ticTacUtils::convertCellToIndex(inputCell,BOARDSIZE)
				) != ticTacUtils::NONE
			) {
			std::cout<<"\n Invalid cell number! Must be unoccupied!\n";
			return 0;
		}
		else {
			if( is_player_0) {
				this->player_0_board.at(
					ticTacUtils::convertCellToIndex(inputCell,BOARDSIZE)
					) = ticTacUtils::PLAYER_0;
				this->current_board.at(
					ticTacUtils::convertCellToIndex(inputCell,BOARDSIZE)
					)  = ticTacUtils::PLAYER_0;
			}
			else {
				this->player_X_board.at(
					ticTacUtils::convertCellToIndex(inputCell,BOARDSIZE)
					) = ticTacUtils::PLAYER_X;
				this->current_board.at(
					ticTacUtils::convertCellToIndex(inputCell,BOARDSIZE)
					)  = ticTacUtils::PLAYER_X;
			}
		}
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
	if (this->emptyCells.size() == 0) {
		return 0;
	}
	auto random_move = *ticTacUtils::get_random_from_list(this->emptyCells);
	this->emptyCells.remove(random_move);
	return this->playMove(random_move, is_player_0);
}

std::string GameBoard::whoWon() {
	return ticTacUtils::player_name[this->who_won];
}
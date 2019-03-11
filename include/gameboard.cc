#include "gameboard.h"

////////// Utilities
void ticTacUtils::reset_board(
	ticTacUtils::board_type_t &current_board,
	ticTacUtils::line_list_t &line_occupancy,
	int size
	) {
	current_board.clear(); current_board.resize(size*size);
	line_occupancy.clear(); line_occupancy.resize(2*size+2);
	for ( auto iterator = current_board.begin();
		iterator != current_board.end();
		iterator++ ) {
		*iterator = player_enum::none;
	}
	for ( auto iterator = line_occupancy.begin();
		iterator != line_occupancy.end();
		iterator++ ) {
		*iterator = player_enum::none;
	}
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
int	ticTacUtils::promptBoardSize() {
	std::string keyIn; int board_size;
	while(true) {
		std::cout<<"\n========\n Specify size of board : [2-9]: ";
		std::cin>>keyIn;
		if(keyIn[0] > '1' && keyIn[0] <= '9') {
			return (int)keyIn[0] - (int)'1' + 1;
		}
	}
}

////////// Class Definitions
GameBoard::GameBoard(bool vis, int size) : board_size(size),visualize(vis) {
	ticTacUtils::reset_board(this->current_board,this->line_occupancy,this->board_size);
	this->resetEmptyCells();
	this->who_won 				= player_enum::none;
	this->is_game_over 			= GAME_NOT_OVER;
}
void GameBoard::resetBoard() {
	ticTacUtils::reset_board(this->current_board,line_occupancy,this->board_size);
	this->resetEmptyCells();
}
void GameBoard::resetEmptyCells() {
	this->emptyCells.clear();
	ticTacUtils::cell_t current_cell;
	for (int i = 0; i<this->board_size*this->board_size; i++) {
		current_cell = ticTacUtils::convertIndexToCell(i, this->board_size);
		this->emptyCells.push_back(current_cell);
	}
}

void GameBoard::showBoard(bool visualize) {
	if (!this->visualize && !visualize) return;
	std::cout<<"Row/Col\t| ";
	for ( int col=0; col<this->board_size; col++) {
		std::cout<<char(this->colStart + col)<<" | ";
	}
	ticTacUtils::printRowSeparator(this->board_size);
	for ( int row=0; row<this->board_size; row++) {
		std::cout<<row+1<<"\t| ";
		for (int col=0; col<this->board_size; col++) {
			auto current_cell =
				this->current_board.at(
					ticTacUtils::convertRowColToIndex(row,col,this->board_size)
					);
			std::cout<<ticTacUtils::player_symbol[current_cell]<<" | ";
		}
		ticTacUtils::printRowSeparator(this->board_size);
	}
}

player_enum GameBoard::checkFilledLine(
	line_type line
	) {
	int line_ownership = (int)player_enum::none; ticTacUtils::cell_t rowCol;
	for ( auto iterator = 0; iterator < this->board_size; iterator++ ) {
		switch (line) {
			case line_type::lr_diagonal: {
				rowCol = {iterator, iterator}; break;
			}
			case line_type::rl_diagonal: {
				rowCol = {this->board_size - 1 - iterator, iterator}; break;
			}
			default:
				return player_enum::none;
		}
		line_ownership += (int)this->current_board.at(
			ticTacUtils::convertCellToIndex(rowCol,this->board_size)
			);
	}
	return (player_enum)static_cast<int>(line_ownership/this->board_size);
}

player_enum GameBoard::checkFilledLine(
	line_type line, ticTacUtils::cell_t rowCol
	) {
	int line_ownership = (int)player_enum::none; int iterator = 0;
	for ( auto iterator = 0; iterator < this->board_size; iterator++ ) {
		switch (line) {
			case line_type::row: {
				line_ownership += (int)this->current_board.at(
					ticTacUtils::convertRowColToIndex(rowCol[0],iterator,this->board_size)
					); break;
			}
			case line_type::column: {
				line_ownership += (int)this->current_board.at(
					ticTacUtils::convertRowColToIndex(iterator,rowCol[1],this->board_size)
					); break;
			}
			default:
				return player_enum::none;
		}
	}
	return (player_enum)static_cast<int>(line_ownership/this->board_size);
}

bool GameBoard::isBoardFilled() {
	for ( int num_row = 0; num_row < this->board_size; num_row++) {
		for ( int num_column = 0; num_column < this->board_size; num_column++) {
			if(current_board.at(
				ticTacUtils::convertRowColToIndex(num_row, num_column, this->board_size)
				) == player_enum::none) {
				return false;
			}
		}
	}
	return true;
}

int GameBoard::isGameOver() {
	ticTacUtils::cell_t rowCol = {0,0};
	for (; rowCol[0] < this->board_size; rowCol[0]++) {
		for (; rowCol[1] < this->board_size; rowCol[1]++) {
			if (
				this->current_board.at(
					ticTacUtils::convertCellToIndex(rowCol,this->board_size)
					) != player_enum::none
				) {
				this->line_occupancy.at(rowCol[0]) =
					this->checkFilledLine(line_type::row,rowCol);

				this->line_occupancy.at(this->board_size + rowCol[1]) =
					this->checkFilledLine(line_type::column,rowCol);
			}
		}
	}
	this->line_occupancy.at(2*this->board_size) =
		this->checkFilledLine(line_type::lr_diagonal);

	this->line_occupancy.at(2*this->board_size + 1) =
		this->checkFilledLine(line_type::rl_diagonal);

	int value = 0;
	for ( int iterator = 0; iterator < this->line_occupancy.size(); iterator++ ) {
		if( line_occupancy.at(iterator) !=  player_enum::none) {
			this->who_won = line_occupancy.at(iterator);
			return (int)this->who_won;
		}
	}

	if( this->isBoardFilled() ) {
		this->who_won = player_enum::none;
		return (int)this->who_won;
	}
	return GAME_NOT_OVER;
}


ticTacUtils::cell_t GameBoard::processKeyboardInput(std::string userInput) {
	ticTacUtils::cell_t rowCol;
	if (int(userInput.at(0)) < int('1') + this->board_size) {
		rowCol[0] = int(userInput.at(0)) - int('1');
	}
	else {
		rowCol[0] = -1;
	}
	if (
		(int)userInput.at(1) >= (int)'A'
		&& (int)userInput.at(1) <= (int)'A'+this->board_size-1
		) {
		rowCol[1] = int(userInput.at(1)) - int('A');
	}
	else if (
		(int)userInput.at(1) >= 'a'
		&& (int)userInput.at(1) <= (int)'a'+this->board_size-1
		) {
		rowCol[1] = int(userInput.at(1)) - int('a');
	}
	else {
		rowCol[1] = -1;
	}
	return rowCol;
}

bool GameBoard::playMove(ticTacUtils::cell_t inputCell, bool is_player_0) {
	if( inputCell[0] <0 || inputCell[0] >= this->board_size ||
		inputCell[1] <0 || inputCell[1] >= this->board_size) {
		std::cout<<"\n Invalid cell number!"
			<<" Must be in format RowColumn [e.g. 1A, 2b, 3C ]\n";
		return false;
	}
	else {
		if (
			this->current_board.at(
				ticTacUtils::convertCellToIndex(inputCell,this->board_size)
				) != player_enum::none
			) {
			std::cout<<"\n Invalid cell number! Must be unoccupied!\n";
			return false;
		}
		else {
			if( is_player_0) {
				this->current_board.at(
					ticTacUtils::convertCellToIndex(inputCell,this->board_size)
					)  = player_enum::player_0;
			}
			else {
				this->current_board.at(
					ticTacUtils::convertCellToIndex(inputCell,this->board_size)
					)  = player_enum::player_x;
			}
		}
	}
	int size = this->emptyCells.size();
	this->emptyCells.remove(inputCell);
	return true;
}


bool GameBoard::playMove(std::string userInput, bool is_player_0) {
	auto rowCol = this->processKeyboardInput(userInput);
	return this->playMove(rowCol, is_player_0);
}

bool GameBoard::playRandomMove(bool is_player_0) {
	if (this->emptyCells.size() == 0) {
		return false;
	}
	auto random_move = *ticTacUtils::get_random_from_list(this->emptyCells);
	this->emptyCells.remove(random_move);
	return this->playMove(random_move, is_player_0);
}

std::string GameBoard::whoWon() {
	return ticTacUtils::player_name[this->who_won];
}
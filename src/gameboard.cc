#include "gameboard.h"

board_type _reset_board(board_type& current_board) {
	for ( auto iterator = current_board.begin(); iterator < current_board.end() ; iterator++ ) {
		current_board.at(*iterator) = UNOCCUPIED;
	}
	return current_board;
}

GameBoard::GameBoard() { //initialize as blank
	this->current_board = { UNOCCUPIED };
	//TODO: find more elegant way to do this
	this->current_board = _reset_board(this->current_board);

	//defaults
	this->who_won 				= OCC_PLAYER_1;
	this->is_game_over 			= UNOCCUPIED;
	this->is_row_filled 		= { true };
	this->is_column_filled 		= { true };
	this->is_diagonal_filled	= { true };

}

void GameBoard::resetBoard() {
	this->current_board = _reset_board(this->current_board);
}

void GameBoard::showBoard() {
	std::cout<<"Row/Col\t : ";
	for ( int col=0; col<BOARDSIZE; col++) {
		std::cout<<char(this->colStart + col)<<"\t";
	}
	std::cout<<std::endl;

	for ( int row=0; row<BOARDSIZE; row++) {
		std::cout<<row+1<<"\t: ";
		for (int col=0; col<BOARDSIZE; col++) {
			std::cout<<this->current_board.at(row*BOARDSIZE + col)<<"\t";
		}
		std::cout<<std::endl;
	}
}
// implement checks for rows and columns

int GameBoard::checkFilledRow( int num_row ) {

	// for ( auto num_row = 0; num_row < BOARDSIZE; num_row++ ) {
	// 	if ( this->is_row_filled.at(num_row) == true ) {
	// 		if ( this->current_board.at(num_row*BOARDSIZE) == OCC_PLAYER_1)
	// 			return OCC_PLAYER_1;
	// 		else if ( current_board.at(num_row*BOARDSIZE) == OCC_PLAYER_2)
	// 			return OCC_PLAYER_2;
	// 	}
	// }
	// return UNOCCUPIED;

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

	// if ( off_diagonal_ownership == BOARDSIZE*OCC_PLAYER_1) {
	// 	return OCC_PLAYER_1;
	// }
	// else if ( off_diagonal_ownership == BOARDSIZE*OCC_PLAYER_2) {
	// 	return OCC_PLAYER_2;
	// }
	// else {
	// 	return off_diagonal_ownership;
	// }
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

int GameBoard::checkFilledDiagonal() {
	int diagonal_ownership = OCC_PLAYER_MIX;
	for ( auto num_cell = 0; num_cell < BOARDSIZE; num_cell++ ) {
		if ( this->player_1_board.at(num_cell + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			diagonal_ownership += OCC_PLAYER_1;
		}
		if ( this->player_2_board.at(num_cell + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			diagonal_ownership += OCC_PLAYER_2;
		}
	}
	return int(diagonal_ownership/BOARDSIZE); // return 0 if no full ownership
}

int GameBoard::checkFilledOffDiagonal() {
	int off_diagonal_ownership = OCC_PLAYER_MIX;
	for ( auto num_cell = 0; num_cell < BOARDSIZE; num_cell++ ) {
		if ( this->player_1_board.at( num_cell - BOARDSIZE - 1 + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			off_diagonal_ownership += OCC_PLAYER_1;
		}
		if ( this->player_2_board.at( num_cell - BOARDSIZE - 1 + num_cell*BOARDSIZE) != UNOCCUPIED ) {
			off_diagonal_ownership += OCC_PLAYER_2;
		}
	}
	return int(off_diagonal_ownership/BOARDSIZE); // return 0 if no full ownership
}

int GameBoard::isGameOver() {
	// O(n^2) check, gives us which columns are filled / unfilled
	for ( int num_row = 0; num_row < BOARDSIZE; num_row++) {
		for ( int num_column = 0; num_column < BOARDSIZE; num_column++) {
			if (
				this->player_1_board.at(num_column + num_row*BOARDSIZE) == UNOCCUPIED
				|| this->player_2_board.at(num_column + num_row*BOARDSIZE) == UNOCCUPIED
				) {
				this->is_row_filled[num_row] 		= false;
				this->is_column_filled[num_column] 	= false;

				//diagonal check
				if ( num_row == num_column ) {
					this->is_diagonal_filled[0] = false;
				}
				//off-diagonal check
				else if ( num_row == BOARDSIZE - 1 - num_column ) {
					this->is_diagonal_filled[1] = false;
				}
			}
		}
	}

	// for ( auto num_row = 0; num_row < BOARDSIZE; num_row++ ) {
	// 	if ( this->checkFilledRow(num_row) != UNOCCUPIED ) {
			// if(  )
	// 	}
	// }
	for ( auto num_column = 0; num_column < BOARDSIZE; num_column++ ) { 
		this->checkFilledColumn(num_column);
	}
	// for ( auto num_row = 0; num_row < BOARDSIZE; num_row++ ) { 
	// 	if( this->is_row_filled[num_row] == true) {
	// 		int who_filled_row = this->checkFilledRow(num_row);
	// 		if (who_filled_row == OCC_PLAYER_1) {
	// 			return OCC_PLAYER_1;
	// 		}
	// 		if (who_filled_row == OCC_PLAYER_2) {
	// 			return OCC_PLAYER_2;
	// 		}

	// 	}
	// }
	

	// //assume 2-D grid
	// for ( auto num_diagonal = 0; iterator < 2; iterator++ ) {

	// }

	// // if 
	return 1;
}

//input format : 1A , 2B, 3C row/column
int GameBoard::playMove(std::string userInput, int playerNumber) {
	int row = int(userInput.at(0))-49; //TODO introduce errorcheck
	int col = int(userInput.at(1))-int(this->colStart);
	if(row >=0 && row < BOARDSIZE &&
		col >=0 && col < BOARDSIZE) {
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
		std::cout<<"\n Invalid cell number! Must be in format RowColumn [e.g. 1A, 2B, 3C ]\n";
		return 0;
	}
	return 1;
}

int GameBoard::spin() {
	return 0;
}
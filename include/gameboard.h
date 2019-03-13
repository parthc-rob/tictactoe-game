//Console-based Tic-Tac-Toe Game. Can be played vs a random bot
//    Copyright (C) 2019, Parth Chopra [parthchopra93@gmail.com]
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define GAME_NOT_OVER	999

#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <list>
#include <iterator>
#include <map>
#include <vector>

enum class player_enum{
	 none = 0, player_0 = 1, player_x = -1
	};
enum class line_type{
	row, column, lr_diagonal, rl_diagonal
	};

namespace ticTacUtils {	
	std::map<player_enum, std::string>
			player_name
			= {
				{player_enum::player_0 	, "Player 0"},
				{player_enum::player_x 	, "Player X"},
				{player_enum::none 	, "No One"},
			};
	std::map<player_enum, char>
			player_symbol
			= {
				{player_enum::player_0 	, '0'},
				{player_enum::player_x 	, 'X'},
				{player_enum::none 	, ' '},
			};
	typedef std::vector <player_enum>
					board_type_t;
	typedef std::array <int,2>
					cell_t;
	typedef std::vector <player_enum>
					line_list_t;

	void		 	reset_board(
						board_type_t& current_board, line_list_t& line_occupancy,
						int size
						);
	int 			random(int min, int max);
	template<typename T> typename T::iterator get_random_from_list(T & list);
	cell_t 			convertIndexToCell(int index, int board_size);
	int 			convertCellToIndex(cell_t rowCol, int board_size);
	int 			convertRowColToIndex(int row, int column, int board_size);
	void			printRowSeparator(int board_size);
	bool			promptReplay();
	int			promptBoardSize();
}

class GameBoard {
	int 						board_size;
	ticTacUtils::board_type_t 			current_board;
	ticTacUtils::board_type_t			player_0_board;
	ticTacUtils::board_type_t			player_X_board;
	player_enum					who_won;
	int 						is_game_over;
	ticTacUtils::line_list_t			line_occupancy;
	char 						colStart = 'A';
	std::list<ticTacUtils::cell_t>			emptyCells;
	bool						visualize = true;
public:
	GameBoard(bool vis = true, int board_size = 3);
	void 						resetBoard();
	void						resetEmptyCells();
	player_enum 					checkFilledLine(line_type line);
	player_enum 					checkFilledLine(line_type line,
									ticTacUtils::cell_t rowCol);
	bool						isBoardFilled();
	int 						isGameOver();
	ticTacUtils::cell_t				processKeyboardInput(std::string userInput);
	bool 						playMove(ticTacUtils::cell_t inputCell,
								bool is_player_0);
	bool 						playMove(std::string userInput,
								bool is_player_0);
	bool						playRandomMove(bool is_player_0);
	void						showBoard(bool visualize = false);
	std::string					whoWon();
	void						activateVisualization();
};

#endif

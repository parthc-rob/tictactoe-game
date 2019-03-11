#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#define OCC_PLAYER_1    1
#define OCC_PLAYER_2   -1
#define OCC_PLAYER_MIX  0
#define UNOCCUPIED      0
#define GAME_OVER_1		1
#define GAME_OVER_2	   -1
#define GAME_NOT_OVER	999
#define BOARDSIZE       3

#include <array>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <list>
#include <iterator>
#include <map>
#include <vector>

namespace ticTacUtils {
	enum 	bot_enum{ HUMAN = 0, BOT = 1 };
	enum 	player_enum{ NONE = 0, PLAYER_0 = 1, PLAYER_X = -1};
	enum 	line_type{
		ROW = 0, COLUMN = BOARDSIZE,
		LR_DIAGONAL = 2*BOARDSIZE,
		RL_DIAGONAL = 2*BOARDSIZE + 1
		};
	std::map<ticTacUtils::player_enum, std::string>
			player_name
			= {
				{PLAYER_0 	, "Player 0"},
				{PLAYER_X 	, "Player X"},
				{NONE 		, "No One"},
			};
	std::map<ticTacUtils::player_enum, char>
			player_symbol
			= {
				{PLAYER_0 	, '0'},
				{PLAYER_X 	, 'X'},
				{NONE 		, ' '},
			};
	typedef std::array <player_enum, BOARDSIZE*BOARDSIZE>
					board_type_t;
	typedef std::array <int,2>
					cell_t;

	board_type_t 	reset_board(board_type_t& current_board);
	int 			random(int min, int max);
	template<typename T> typename T::iterator get_random_from_list(T & list);
	cell_t 			convertIndexToCell(int index, int board_size);
	int 			convertCellToIndex(cell_t rowCol, int board_size);
	int 			convertRowColToIndex(int row, int column, int board_size);
	void			printRowSeparator(int board_size);
	bool			promptReplay();
}

class GameBoard {

	ticTacUtils::board_type_t 			current_board;
	ticTacUtils::board_type_t			player_0_board;
	ticTacUtils::board_type_t			player_X_board;
	ticTacUtils::player_enum			who_won;
	int 								is_game_over;
	std::array<ticTacUtils::player_enum, 2*BOARDSIZE + 2>
										line_occupancy;
	char 								colStart = 'A';
	char 								no_one = ' ';
	std::list<ticTacUtils::cell_t>		emptyCells;
	bool								visualize = true;

public:
	GameBoard(bool visualize = true);
	void 						resetBoard();
	void						resetEmptyCells();
	ticTacUtils::player_enum 	checkFilledLine(ticTacUtils::line_type line);
	ticTacUtils::player_enum 	checkFilledLine(ticTacUtils::line_type line,
											ticTacUtils::cell_t rowCol);
	bool						isBoardFilled();
	int 						isGameOver();
	ticTacUtils::cell_t			processKeyboardInput(std::string userInput);
	int 						playMove(ticTacUtils::cell_t inputCell,
											bool is_player_0);
	int 						playMove(std::string userInput,
											bool is_player_0);
	int							playRandomMove(bool is_player_0);
	void						showBoard(bool visualize = false);
	std::string					whoWon();
	void						activateVisualization();
};

#endif
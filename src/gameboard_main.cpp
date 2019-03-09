#include <iostream>
#include "gameboard.cc"

int main(int argc, char** argv) {

	std::cout<< " \nTic - Tac - Toe Game Started \n Players : 0, X\n"
		<<"\nPlayer 0 starts\n\n";

	std::array<ticTacUtils::player_enum,2> game_players
			= { ticTacUtils::PLAYER_0, ticTacUtils::PLAYER_X };

	std::vector<std::pair<ticTacUtils::player_enum,bool>> who_is_bot;

	auto player_iterator = game_players.begin();

	while(player_iterator < game_players.end()) {
		int keyInput;
		std::cout<<"\nChoose Player "<<ticTacUtils::player_symbol[*player_iterator];
		std::cout<<" :\n0\t: Human\n1\t: Random bot\n";

		std::cin>>keyInput;
		if (keyInput == 0 || keyInput == 1) {
			who_is_bot.push_back(std::make_pair(*player_iterator,(bool)keyInput));
			player_iterator++;
		}
		else {
			std::cout<<"\nYou gave an invalid input, please try again.\n";
		}
	}

	bool is_game_active = true;
	std::string is_game_active_user_input;
	bool is_current_player_0 = true;

	bool show_game_steps
		= !(who_is_bot[is_current_player_0].second
			&& who_is_bot[!is_current_player_0].second);
	GameBoard game_1(show_game_steps);
	game_1.showBoard();

	while( is_game_active ) {
		std::string current_move;
		do {
			int isMoveValid;
			if (who_is_bot[!is_current_player_0].second) {
				do {
					isMoveValid = game_1.playRandomMove(is_current_player_0);
				} while (!isMoveValid);
			}
			else {
				do {
					std::cout<<"Enter a move, "
						<<ticTacUtils::player_name[game_players[(int)!is_current_player_0]]
						<<" [e.g. 1A , 2B, 3C] : ";
					std::cin>>current_move;
					isMoveValid = game_1.playMove(current_move,is_current_player_0);
				} while (!isMoveValid);
			}
			is_current_player_0 = !is_current_player_0;
			game_1.showBoard();
			if ( game_1.isGameOver() ) {

				std::cout<<"\nGame Over. Winning Player : "<<game_1.whoWon()<<std::endl;
				is_game_active = false;
			}
		}while(is_game_active);
	}
	game_1.activateVisualization();
	game_1.showBoard();
	return 0;
}
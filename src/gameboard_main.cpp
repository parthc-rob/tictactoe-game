#include <iostream>
#include "gameboard.cc"

int main(int argc, char** argv) {

	std::cout<< " \nTic - Tac - Toe Game Started \n Players : 0, X\n"
		<<"\nPlayer 0 starts\n\n";
	bool is_player_2_bot=false;
	int mode;
	do{
		std::cout<<"\nChoose mode [1,2]:\n1\t: Player-vs-Player\n2\t: Player-vs-random bot\n";
		std::cin>>mode;
	}while(mode>2 || mode <1);

	if(mode == 2) {
		is_player_2_bot = true;
	}

	GameBoard game_1;
	game_1.showBoard();

	bool is_game_active = true;
	std::string is_game_active_user_input;
	int current_player = 1;

	while( is_game_active ) {
		std::string current_move;
		do {
			int isMoveValid;
			if ( is_player_2_bot && current_player != 1) {
				isMoveValid = game_1.playRandomMove(current_player);
			}
			else {
				std::cout<<"Enter a move, player "<<game_1.playerSymbol(current_player)<<" [e.g. 1A , 2B, 3C] : ";
				std::cin>>current_move;
				isMoveValid = game_1.playMove(current_move,current_player);
			}
			if( isMoveValid ) {
				current_player = (current_player)%2 + 1;
				game_1.showBoard();
			}

			if ( game_1.isGameOver() ) {

				std::cout<<"\nGame Over. Winning Player : ";
				if( game_1.whoWon() == game_1.noOne() ) {
					std::cout<< " No One \n";
				}
				else {
					std::cout<<game_1.whoWon()<<"\n";
				}
				is_game_active = false;
			}
		}while(is_game_active);
	}
	return 0;
}
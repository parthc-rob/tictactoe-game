#include <iostream>
#include "gameboard.cc"

int main(int argc, char** argv) {
	std::cout<<"hello"<<std::endl;

	GameBoard game_1;
	game_1.showBoard();

	bool is_game_active = true;
	std::string is_game_active_user_input;
	int current_player = 1;

	while(is_game_active) {
		std::string current_move;
		do { //DEBUG
			std::cout<<"Enter a move, player "<<current_player<<": ";
			std::cin>>current_move;
			int isMoveValid = game_1.playMove(current_move,current_player);
			if(isMoveValid) {
				current_player = (current_player)%2 + 1;
				game_1.showBoard();
			}
			std::cout<<"Keep Playing? [y/n]";
			std::cin>>is_game_active_user_input;
			if(is_game_active_user_input == "y") {
				is_game_active = true;
			}
			else if(is_game_active_user_input == "n") {
				is_game_active = false;
			}
		}while(is_game_active);
	}
	return 0;
}
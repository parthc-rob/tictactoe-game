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

#include <iostream>
#include "../include/gameboard.cc"

int main(int argc, char** argv) {
	int board_size = ticTacUtils::promptBoardSize();
	bool playAgain = false;
	do {
		std::cout<< " \nTic - Tac - Toe Game Started \n Players : 0, X\n"
			<<"\nPlayer 0 starts\n\n";

		std::array<player_enum,2> game_players
				= { player_enum::player_0, player_enum::player_x };

		std::vector<std::pair<player_enum,bool>> who_is_bot;

		auto player_iterator = game_players.begin();

		while(player_iterator != game_players.end()) {
			int keyInput;
			std::cout<<"\nChoose Player "
				<<ticTacUtils::player_symbol[*player_iterator];
			std::cout<<" :\n0\t: Human\n1\t: Random bot\n";

			std::cin>>keyInput;
			if (keyInput == 0 || keyInput == 1) {
				who_is_bot.push_back(
					std::make_pair(*player_iterator,(bool)keyInput)
					);
				player_iterator++;
			}
			else {
				std::cout<<"\nYou gave an invalid input, please try again.\n";
			}
		}

		bool is_game_active = true;
		bool is_current_player_0 = true;

		bool show_game_steps // automatically false if bot vs bot game
			= !(who_is_bot[is_current_player_0].second
				&& who_is_bot[!is_current_player_0].second);
		GameBoard current_game(show_game_steps, board_size);
		current_game.showBoard();

		while( is_game_active ) {
			std::string current_move;
			do {
				bool is_move_valid;
				if (who_is_bot[!is_current_player_0].second) {
					do {
						is_move_valid =
							current_game.playRandomMove(is_current_player_0);
					} while (!is_move_valid);
				}
				else {
					do {
						std::cout<<"Enter a move, "
							<<ticTacUtils::player_name[
								game_players[(int)!is_current_player_0]
								]
							<<" [e.g. 1A , 2B, 3C] : ";
						std::cin>>current_move;
						is_move_valid =
							current_game.playMove(
								current_move,is_current_player_0);
					} while (!is_move_valid);
				}
				is_current_player_0 = !is_current_player_0;
				current_game.showBoard();
				if ( current_game.isGameOver() != GAME_NOT_OVER) {

					std::cout<<"\n===========\n GAME OVER.\n Winning Player : "
						<<current_game.whoWon()<<"\n===========\n";
					is_game_active = false;
				}
			}while(is_game_active);
		}
		current_game.showBoard(true);
		playAgain = ticTacUtils::promptReplay();
	}while(playAgain);
	return 0;
}

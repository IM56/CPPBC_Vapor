//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#include "Game.h"

std::ostream& operator<< (std::ostream& os, const Game& game)
{
	os << std::left << std::setw(6) << game.get_game_id() << std::setw(3) << "|"
		<< std::setw(MAX_GAME_TITLE) << game.get_title() << std::setw(3) << "|"
		<< std::setw(MAX_DESCRIPTION) << game.get_desc() << std::setw(3) << "|"
		<< "\x9C" << std::setw(7) << std::setprecision(2) << std::fixed << game.get_price();

	return os;
}
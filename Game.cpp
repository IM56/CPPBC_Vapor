//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#include "Game.h"
#include "DatabaseManager.h"

std::ostream& operator<< (std::ostream& os, const Game& game)
{
	std::string title = game.get_title();
	std::string desc = game.get_desc();

	if (title.front() == ' ')
		title = (title.substr(1, title.length() - 2));

	if (desc.front() == ' ')
		desc = (desc.substr(1, desc.length() - 2));

	os << std::left << std::setw(6) << game.get_game_id() << std::setw(3) << "|"
		<< std::setw(MAX_GAME_TITLE) << title << std::setw(3) << "|"
		<< std::setw(MAX_DESCRIPTION) << desc << std::setw(3) << "|"
		<< "\x9C" << std::setw(7) << std::setprecision(2) << std::fixed << game.get_price();

	return os;
}

//////////////////////////////////////
// GameFactory class implementation //
//////////////////////////////////////

void GameFactory::createNewGame(const Game::GameId game_id, const std::string & title, const std::string & desc, const double price)
{
	pGame = new Game(game_id, title, desc, price);
	DatabaseManager::instance().add_game(pGame);
}
;
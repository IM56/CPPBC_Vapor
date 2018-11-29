//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef PLAYER_USER_H
#define PLAYER_USER_H


#include "Users.h"
#include "Game.h"
#include "Wallet.h"
#include "Stopwatch.h"

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;
	using UserBase::UserBase;

	PlayerUser(const Username& username, const std::string& password, const std::string& email, double funds) :
		UserBase(username, password, email), m_wallet(funds) 
	{
		std::string file = username + "_game_bag.txt";
		m_game_bag = std::string("data\\GameBags\\") + file;
	}

	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kPlayerUser; }
	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }

	void add_to_game_list(const Game::GameId game_id);
	void remove_from_game_list(const Game::GameId game_id);
	void list_owned_games() const;
	virtual double get_available_funds() const override { return m_wallet.get_available_funds(); }

	void add_funds();
	void add_funds(double a);

	std::string get_game_file() { return m_game_bag; }

	void buy_game(const Game::GameId game_id);
	void play_game(const Game::GameId game_id);

	bool owns_game(const Game::GameId game_id);
	bool is_bag_empty();

private:
	GameList m_ownedGames; // List of owned games.
	Wallet m_wallet; // The players available funds.
	std::string m_game_bag; // File path to user's list of owned games
};

#endif
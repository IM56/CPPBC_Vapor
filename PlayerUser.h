//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef PLAYER_USER_H
#define PLAYER_USER_H


#include "Users.h"
#include "Game.h"
#include "Wallet.h"

//--
// PlayerUser represents a system user who owns games
//--
class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;

	using UserBase::UserBase;

	PlayerUser(const Username& username, const std::string& password, const std::string& email, double funds) :
		UserBase(username, password, email), m_wallet(funds) {}

	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kPlayerUser; }

	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }

	void list_owned_games() const;

	virtual double get_available_funds() const override { return m_wallet.get_available_funds(); }

	void add_funds();

	void buy_game(const Game::GameId game);

private:
	GameList m_ownedGames; // List of owned games.
	Wallet m_wallet; // The players available funds.
};

#endif
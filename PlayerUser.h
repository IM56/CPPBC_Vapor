//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

//////////////////////////////////////////////////////////////////
// PlayerUser: A user who can buy and play games from the store //
//////////////////////////////////////////////////////////////////

#pragma once
#ifndef PLAYER_USER_H
#define PLAYER_USER_H


#include "Users.h"
#include "Game.h"
#include "Wallet.h"
#include "Stopwatch.h"

class PlayerUser : public UserBase
{
public:
	using GameList = std::list<Game::GameId>;
	// Inherit the base constructor
	using UserBase::UserBase;

	PlayerUser(const Username& username, const std::string& password, const std::string& email, double funds) :
		UserBase(username, password, email), m_wallet(funds) 
	{
		std::string file = username + "_game_bag.txt";
		m_game_bag = std::string("data\\GameBags\\") + file;
	}

	// define the specific user type.
	virtual const UserTypeId  get_user_type() const override { return UserTypeId::kPlayerUser; }
	const PlayerUser::GameList& get_game_list() const { return m_ownedGames; }  // Returns a reference to the user's list of games

	void add_to_game_list(const Game::GameId game_id);           // Add a game to their list
	void remove_from_game_list(const Game::GameId game_id);      // Remove a game from their list
	void list_owned_games() const;                               // Output their list of games to the screen
	virtual double get_available_funds() const override { return m_wallet.get_available_funds(); }

	void add_funds();           // Ask the user how much money they would like to deposit
	void add_funds(double a);   // Used by admins for refunding affected players

	std::string get_game_file() { return m_game_bag; }  // Get the path to their bag file

	void buy_game(const Game::GameId game_id);      // Purchase a chosen game
	void play_game(const Game::GameId game_id);     // Play a chosen game

	bool owns_game(const Game::GameId game_id);   // Check if game is in their bag
	bool is_bag_empty();  // Check if they own no games

private:
	GameList m_ownedGames;  // List of owned games.
	Wallet m_wallet;        // Contains the player's available funds.
	std::string m_game_bag; // File path to user's list of owned games
};

#endif
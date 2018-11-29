//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

///////////////////////////////////////////////////////////////////////////////
// DatabaseManager: Keeps records of users, players, games, and transactions //
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "AdminUser.h"
#include "PlayerUser.h"
#include "Game.h"
#include "Transaction.h"

#include <cstdio>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>


class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	const char* adminFile = "data\\AdminUserList.txt";     // File that holds a list of admin users
	const char* playerFile = "data\\PlayerUserList.txt";   // File that holds a list of player users
	const char* gameFile = "data\\GameList.txt";           // File that holds a master list of games on the system

	void load_data(); // Initialize the database from storage.

	void add_user_to_file(UserBase* pUser);        // Write user data to file
	void remove_user_from_file(UserBase* pUser);   // Remove user data from file
	void update_user_in_file(UserBase* pUser);     // Update the storage records of the user
	void add_user(UserBase* pUser);                // Add a new user to the system
	void remove_user(const std::string& username); // Remove a user from the system
	
	void add_game_to_file(Game* pGame);                                         // Write game info to file
	void remove_game_from_file(Game* pGame);                                    // Remove game info from file
	void remove_game_from_bag(Game::GameId game_id, PlayerUser* pPlayer);       // Removes a game from a player's bag file
	void update_game_in_file(Game* pGame);                                      // Update the storage records of the game
	void add_game(Game* pGame);                                              // Add a new game to the system
	void remove_game(Game::GameId game_id);                                     // Remove a game from the system
	void add_game_to_bag(Game::GameId game_id, PlayerUser* pPlayer);            // Add a game to a user's bag file

	void log_playtime(const std::string& time, const std::string& game_title, const char* log_file) const;   // Logs a play session out to a file
	void log_transaction(const Transaction& trans, const char* transaction_file) const;                      // Logs a transaction out to a file
	void play_log_header() const;                              // Sets a table header for outputting play session info
	void transaction_header() const;                           // Sets a table header for outputting transaction info
	void display_play_log(const std::string& username);        // Outputs a user's play sessions to the console
	void display_transactions(const std::string& username);    // Outputs a user's transactions to the console
	

	UserBase* find_user(const std::string& username);   // Returns a pointer to the user if they exist, nullptr otherwise
	Game* find_game(const Game::GameId gameid);         // Returns a pointer to the game if it exits, nullptr otherwise
	std::vector<PlayerUser*> find_users_who_own_game(const Game::GameId game_id);  // Returns a vector of pointers to players who own this game
	
	bool is_game_in_file(Game* pgame, const char* filename); // Checks if a game is recorded in the file

	// Searches for games in the system, returns a vector of found titles.
	// (0) to query by title, (1) to query by description
	std::vector<std::string> search_games(const std::string& query, SearchDescriptor flag, const char* filename);

	// Iterate over all users (visitor pattern)
	template<typename Visitor> 
	void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Iterate over all players (visitor pattern)
	template<typename Visitor> 
	void visit_players(Visitor& func)
	{
		for (auto& it : m_users)
		{
			UserBase* pUser = it.second;
			if (pUser->get_user_type() == UserTypeId::kPlayerUser)
			{
				// Not all user methods are polymorphic, some are specific to players only
				PlayerUser* pPlayer = static_cast<PlayerUser*>(it.second);
				func(pPlayer);
			}
		}
	}
	

	// Iterate over all games (visitor pattern)
	template<typename Visitor> 
	void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(*it.second); }
	}

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager(); // Frees all users and games stored on the heap

	void load_users_from_file(UserTypeId, const char* filename);   // Load all users of that type from a file
	void load_games_from_file(const char * filename);              // Load all available games from file
	void load_game_bag_from_file(PlayerUser* pPlayer);             // Load a user's game bag from file
	void load_games_each_player();                                 // Load all player users' games from file
	bool is_user_in_file(UserBase* pUser, const char* filename);   // Checks for a user in the file
	


private:
	// Types
	using UserContainer = std::map<Username, UserBase*>;
	using GameContainer = std::map<Game::GameId, Game*>;

	UserContainer m_users;
	GameContainer m_games;
	UserFactory m_uFactory;
	GameFactory m_gFactory;
};



#endif
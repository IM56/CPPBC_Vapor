//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <map>
#include <fstream>
#include <sstream>
#include <vector>

#include "AdminUser.h"
#include "PlayerUser.h"
#include "Game.h"

//--
// DatabaseManager is the one central database for the whole system
// In this example it has been designed using the singleton pattern.
//--
class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	const char* adminFile = "data\\AdminUserList.txt";
	const char* playerFile = "data\\PlayerUserList.txt";
	const char* gameFile = "data\\GameList.txt";

	void load_data(); // Initialize the database from storage.

	void add_user_to_file(UserBase* pUser);        // Add user data to file
	void remove_user_from_file(UserBase* pUser);   // Remove user data from file
	void update_user_in_file(UserBase* pUser);     // Update the storage records of the user
	void add_user(UserBase* pUser);                // Adds a user to files and db manager
	void remove_user(const std::string& username); // Removes a user from files and db manager
	
	void add_game_to_file(Game* pGame);         // Add game data to file
	void remove_game_from_file(Game* pGame);    // Remove game data from file
	void remove_game_from_bag(Game::GameId game_id, PlayerUser* pPlayer); // Removes a game from a player's bag file
	void update_game_in_file(Game* pGame);      // Update the storage records of the game
	void create_game(Game* pGame);                // Adds a game to the db.
	void remove_game(Game::GameId game_id);    // Removes a game from files and db manager
	void add_game_to_bag(Game::GameId game_id, PlayerUser* pPlayer); // Adds a game to a user's bag file
	

	UserBase* find_user(const std::string& username);                       // Finds a username in db, return nullptr if the user is not found.
	Game* find_game(const Game::GameId gameid);                             // Finds a game in db, returns nullptr if the game is not found.
	std::vector<PlayerUser*> find_users_who_own_game(const Game::GameId game_id); // Returns a vector of players who own a given game.
	
	// Look for game in file
	bool is_game_in_file(Game* pgame, const char* filename);

	// Finds a game in db, return vector of found titles.
	// (0) to query by title, (1) to query by description
	std::vector<std::string> find_game(const std::string& query, SearchDescriptor flag, const char* filename);

	// iterating users using visitor pattern
	template<typename Visitor> 
	void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// iterating players using visitor pattern
	template<typename Visitor> 
	void visit_players(Visitor& func)
	{
		for (auto& it : m_users)
		{
			UserBase* pUser = it.second;
			if (pUser->get_user_type() == UserTypeId::kPlayerUser)
			{
				PlayerUser* pPlayer = static_cast<PlayerUser*>(it.second);
				func(pPlayer);
			}
		}
	}
	

	// iterating games using visitor pattern
	template<typename Visitor> 
	void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(*it.second); }
	}

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();

	// Load user data from a file
	void load_users_from_file(UserTypeId, const char* filename);

	// Load all available games from file
	void load_games_from_file(const char * filename);

	// Load a user's games from their file
	void load_game_bag_from_file(PlayerUser* pPlayer);

	//Load each player user's games from their file
	void load_games_each_player();

	// Look for user in file
	bool is_user_in_file(UserBase* pUser, const char* filename);

	


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
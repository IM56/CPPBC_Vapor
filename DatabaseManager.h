//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <map>
#include <fstream>
#include <sstream>
#include <type_traits>

#include "Users.h"
#include "AdminUser.h"
#include "PlayerUser.h"

//--
// DatabaseManager is the one central database for the whole system
// In this example it has been designed using the singleton pattern.
//--
class DatabaseManager
{
public:
	// Singleton instance definition.
	static DatabaseManager& instance();

	// Initialize the database from storage.
	void load_data();

	// Write data to storage.
	void store_user_data(UserBase* pUser);

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	// Removes a user from the db.
	void remove_user(const std::string& username);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const std::string& username);

	// iterating users using visitor pattern
	template<typename Visitor> void visit_users(Visitor& func)
	{
		for (auto it : m_users) { func(*it.second); }
	}

	// Adds a game to the db.
	void add_game(Game& rGame);

	// Finds a game by id, returns nullptr if the game is not found.
	Game* find_game(const Game::GameId gameid);

	// iterating games using visitor pattern
	template<typename Visitor> void visit_games(Visitor& func)
	{
		for (auto it : m_games) { func(it.second); }
	}

private:
	// Constructors are private for singleton pattern.
	DatabaseManager();
	~DatabaseManager();

	// Load user data from a file
	void load_users_from_file(UserTypeId, const char* filename);

	// Look for user in file
	bool find_user_in_file(UserBase* pUser, const char* filename);

	// Remove user data from file
	void remove_user_from_file(UserBase* pUser);


private:
	// Types
	using UserContainer = std::map<Username, UserBase*>;
	using GameContainer = std::map<Game::GameId, Game>;

	UserContainer m_users;
	GameContainer m_games;
	UserFactory m_uFactory;

	const char* adminFile = "data\\AdminUserList.txt";
	const char* playerFile = "data\\PlayerUserList.txt";

};



#endif
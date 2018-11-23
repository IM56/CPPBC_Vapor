//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <map>
#include <fstream>
#include <sstream>
#include <type_traits>

#include "Game.h"
#include "Users.h"

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

	// Write all the data to storage.
	void store_data();

	// Adds a user to the db.
	void add_user(UserBase* pUser);

	// Finds a user by username, return nullptr if the user is not found.
	UserBase* find_user(const Username& username);

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
	template <typename T>
	void load_users_from_file(const char* filename);

private:
	// Types
	using UserContainer = std::map<Username, UserBase*>;
	using GameContainer = std::map<Game::GameId, Game>;

	UserContainer m_users;
	GameContainer m_games;

};

template <typename T>
void DatabaseManager::load_users_from_file(const char* filename)
{
	std::string line;
	std::ifstream file(filename);

	while (std::getline(file, line))
	{
		char c;
		std::string username;
		std::string password;
		std::string email;
		double funds;
		std::istringstream iss(line);
		iss >> username >> c >> password >> c >> email >> c >> funds;
		
		if (std::is_same<T, PlayerUser>::value)
			add_user(new PlayerUser(username, password, email, funds));
		else
			add_user(new T(username, password, email));
			
	}

}


#endif
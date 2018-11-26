//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include <vector>
#include <algorithm>
#include <utility>

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) 
			delete it.second;
	}

	for (auto it : m_games)
	{
		if (it.second)
			delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}


void DatabaseManager::load_data()
{
	// Load admin users from file
	load_users_from_file(UserTypeId::kAdminUser, adminFile);
	
	// Load player users from file
	load_users_from_file(UserTypeId::kPlayerUser, playerFile);

	// Load games from file
	load_games_from_file(gameFile);
}

void DatabaseManager::add_user_to_file(UserBase* pUser)
{
	if (pUser)
	{
		switch (pUser->get_user_type())
		{
		case UserTypeId::kAdminUser :
		{
			if (!is_user_in_file(pUser, adminFile))
			{
				std::ofstream fout(adminFile, std::ios::out | std::ios::app);
				if (pUser->get_username() != "")
				{
					fout << "\n" << pUser->get_username() << " , " <<
						pUser->get_password() << " , " <<
						pUser->get_email();
				}
			}
			break;
		}
		case UserTypeId::kPlayerUser :
		{
			if (!is_user_in_file(pUser, playerFile))
			{
				std::ofstream fout(playerFile, std::ios::out | std::ios::app);
				fout << "\n" << pUser->get_username() << " , " <<
					pUser->get_password() << " , " <<
					pUser->get_email() << " , " <<
					static_cast<PlayerUser*>(pUser)->get_available_funds();
			}
			break;
		}
		default:
			break;
		}
	}
}

void DatabaseManager::remove_user_from_file(UserBase* pUser)
{
	std::string uname = pUser->get_username();
	std::string line;
	const char* filename;
	std::vector<std::string> file_contents;
	switch (pUser->get_user_type())       // Identify type of user passed to function
	{
	case UserTypeId::kAdminUser:
	{
		filename = adminFile; break;
	}
	case UserTypeId::kPlayerUser:
	{
		filename = playerFile; break;
	}
	default: return;
	}

	std::ifstream fin(filename);              // Open the file containing the user that was passed
	while (std::getline(fin, line))
	{
		if (line.find(uname + " , ") == std::string::npos)
			file_contents.push_back(line);    // Copy the contents of the file except for the line containing the user
	}

	if (!file_contents.empty())                // If everything was successfully copied into the vector
	{
		std::ofstream fout(filename, std::ios::trunc);
		for (const auto& ln : file_contents)
			fout << ln << "\n";                // Put it back in the file
	}
}

void DatabaseManager::update_user_in_file(UserBase * pUser)
{
	remove_user_from_file(pUser);
	add_user_to_file(pUser);
}

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser)
	{
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
		add_user_to_file(pUser);
	}
}

void DatabaseManager::remove_user(const std::string & username)
{
	UserBase* pUser = find_user(username);
	if (pUser)
	{
		m_users.erase(username);
		remove_user_from_file(pUser);
		delete pUser;
		std::cout << "\nUser <" << username << "> has been erased!\n";
	}
	else
		std::cout << "\nSorry, that username is not our records!\n";
}

void DatabaseManager::add_game_to_file(Game* pGame)
{
	if (pGame)
	{
		if (!is_game_in_file(pGame, gameFile))
		{
			std::ofstream fout(gameFile, std::ios::out | std::ios::app);
			if (pGame->get_game_id() > 0)
			{
				fout << "\n" << pGame->get_game_id() << " , " <<
					pGame->get_title() << " , " <<
					pGame->get_desc() << " , " <<
					pGame->get_price();
			}
		}
	}
}

void DatabaseManager::remove_game_from_file(Game* pGame)
{

	Game::GameId game_id = pGame->get_game_id();
	Game::GameId num;
	std::string line;
	std::vector<std::string> file_contents;
	
	std::ifstream fin(gameFile);              // Open the game file
	while (std::getline(fin, line))
	{
		std::istringstream iss(line);
		iss >> num;
		if (num != game_id)
			file_contents.push_back(line);    // Copy the contents of the file except for the line containing the game
	}

	if (!file_contents.empty())                // If everything was successfully copied into the vector
	{
		std::ofstream fout(gameFile, std::ios::trunc);
		for (const auto& ln : file_contents)
			fout << ln << "\n";                // Put it back in the file
	}
}

void DatabaseManager::update_game_in_file(Game* pGame)
{
	remove_game_from_file(pGame);
	add_game_to_file(pGame);
}

void DatabaseManager::add_game(Game* pGame)
{
	m_games.insert(std::make_pair(pGame->get_game_id(), pGame));
	add_game_to_file(pGame);
}

void DatabaseManager::remove_game(Game::GameId game_id)
{
	Game* pGame = find_game(game_id);
	if (pGame)
	{
		std::string title = pGame->get_title();
		m_games.erase(game_id);
		remove_game_from_file(pGame);
		delete pGame;
		std::cout << "\nGame <" << title << "> has been erased!\n";
	}
	else
		std::cout << "\nSorry, that game is not our records!\n";
}

UserBase* DatabaseManager::find_user(const std::string& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
		return it->second;
	else
		return nullptr;
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
		return it->second;
	else
		return nullptr;
}

std::vector<std::string> DatabaseManager::find_game(const std::string & query, SearchDescriptor flag, const char* filename)
{
	std::vector<std::string> found_titles;
	std::string title, desc_word, line;
	Game::GameId id;
	char c;

	std::ifstream fin(filename);     // Open file for reading
	while (std::getline(fin, line))  // While there are files to read
	{
		std::istringstream iss(line);
		if ((iss>>id) && (iss>>c) && std::getline(iss, title, ','))
		{
			title = title.substr(1, title.length() - 2);
			if (flag == SearchDescriptor::kTitle)  // If we are looking for a title...
			{
				if (query == title)  // If our title is in there
				{
					found_titles.push_back(title);    // ... add it to our matching titles
				}
			}
			else if (flag == SearchDescriptor::kDesc) // If we are looking for a description...
			{
				do
				{
					iss >> desc_word;                        // Read along the descriptive text
					if (desc_word.find(',') != std::string::npos) // until we hit a (delimiting) comma
						break;
					if (desc_word == query)   // If a word in the description matches our query
						found_titles.push_back(title); // add it to our matching titles
				} while (desc_word.find(',') == std::string::npos);
			}
		}
		
	}
	return found_titles;
}


void DatabaseManager::load_users_from_file(UserTypeId usertype, const char* filename)
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

		m_uFactory.createNewUser(usertype, username, password, email, funds);
	}
}

void DatabaseManager::load_games_from_file(const char * filename)
{
	std::string line;
	std::ifstream file(filename);
	char c;
	Game::GameId game_id;
	std::string title;
	std::string desc;
	double price;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		if ((iss >> game_id) && (iss >> c) && std::getline(iss, title, ',')
			&& std::getline(iss, desc, ',') && (iss >> price))
		{
			m_gFactory.createNewGame(game_id, title, desc, price);
		}
	}
}


bool DatabaseManager::is_user_in_file(UserBase * pUser, const char* filename)
{
	std::string uname = pUser->get_username();
	std::string listUname;
	std::ifstream fin(filename);
	while (fin >> listUname)
	{
		if (listUname == uname)
			return true;
		else
			fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return false;
}

bool DatabaseManager::is_game_in_file(Game* pgame, const char * filename)
{
	Game::GameId game_id = pgame->get_game_id();
	Game::GameId file_game_id;
	std::ifstream fin(filename);
	while (fin >> file_game_id)
	{
		if (file_game_id == game_id)
			return true;
		else
			fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return false;
}





//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include <vector>

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
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
	load_users_from_file(UserTypeId::kAdminUser, "data\\AdminUserList.txt");
	
	// Load player users from file
	load_users_from_file(UserTypeId::kPlayerUser, "data\\PlayerUserList.txt");

	// add some games.
	add_game(Game(4789, "Bounceback", "A platform puzzle game for PSP"));
	add_game(Game(5246, "Piecefall", "A tetris like 3d puzzle game for PS4"));
}

void DatabaseManager::store_user_data(UserBase* pUser)
{
	if (pUser)
	{
		switch (pUser->get_user_type())
		{
		case UserTypeId::kAdminUser :
		{
			if (!find_user_in_file(pUser, adminFile))
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
			if (!find_user_in_file(pUser, playerFile))
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

void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
	if (pUser)
	{
		m_users.insert(std::make_pair(pUser->get_username(), pUser));
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

UserBase* DatabaseManager::find_user(const std::string& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(std::make_pair(rGame.get_game_id(), rGame));
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
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

bool DatabaseManager::find_user_in_file(UserBase * pUser, const char* filename)
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

void DatabaseManager::remove_user_from_file(UserBase* pUser)
{
	std::string uname = pUser->get_username();
	std::string line;             
	const char* filename;
	std::vector<std::string> file_contents;
	switch (pUser->get_user_type())       // Identify type of user passed to function
	{
	case UserTypeId::kAdminUser :
	{
		filename = adminFile; break;
	}
	case UserTypeId::kPlayerUser :
	{
		filename = playerFile; break;
	}
	default: return;
	}

	std::ifstream fin(filename);              // Open the file containing the user that was passed
	while (std::getline(fin, line))
	{
		if(line.find(uname + " , ") == std::string::npos)
			file_contents.push_back(line);    // Copy the contents of the file except for the line containing the user
	}

	if (!file_contents.empty())                // If everything was successfully copied into the vector
	{
		std::ofstream fout(filename, std::ios::trunc);
		for (const auto& ln : file_contents)
			fout << ln << "\n";                // Put it back in the file
	}
}

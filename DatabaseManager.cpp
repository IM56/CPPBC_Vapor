//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "DatabaseManager.h"

#include <algorithm>
#include <utility>

DatabaseManager::DatabaseManager()
{}

DatabaseManager::~DatabaseManager()
{
	// m_users owns and must free all pointers to users on heap memory
	for (auto it : m_users)
	{
		if (it.second) 
			delete it.second;
	}

	// m_games owns and must free all pointers to games on heap memory
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

	// Load each user's currently owned games
	load_games_each_player();
}

void DatabaseManager::add_user_to_file(UserBase* pUser)
{
	// Check that the pointer is valid
	if (pUser)
	{
		// Different actions for different user types
		switch (pUser->get_user_type())
		{
		case UserTypeId::kAdminUser :
		{
			// Check that this user is not already in the file
			if (!is_user_in_file(pUser, adminFile))
			{
				std::ofstream fout(adminFile, std::ios::out | std::ios::app);
				
				// Safeguards against writing empty data to file
				if (fout && !pUser->get_username().empty())
				{
					fout << pUser->get_username() << " , " <<
						pUser->get_password() << " , " <<
						pUser->get_email() << "\n";
				}
			}
			break;
		}
		case UserTypeId::kPlayerUser :
		{
			// Checks that this user is not already in the file
			if (!is_user_in_file(pUser, playerFile))
			{
				std::ofstream fout(playerFile, std::ios::out | std::ios::app);

				// Safeguards against writing empty data to file
				if (fout && !pUser->get_username().empty())
				{
					fout << pUser->get_username() << " , " <<
						pUser->get_password() << " , " <<
						pUser->get_email() << " , " <<
						pUser->get_available_funds() << "\n";
				}
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
	// Dummy string to read lines from files
	std::string line;
	const char* filename;
	std::vector<std::string> file_contents;
	
	// Identify type of user passed to function to read appropriate file
	switch (pUser->get_user_type())      
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

	std::ifstream fin(filename);
	if (fin)
	{
		// Copy the contents of the file except for the line containing the user
		while (std::getline(fin, line))
		{
			if (line.find(uname + " , ") == std::string::npos)
				file_contents.push_back(line);
		}

		// Write the modified contents back into the file
		if (!file_contents.empty())                
		{
			std::ofstream fout(filename, std::ios::trunc);
			if (fout)
			{
				for (const auto& ln : file_contents)
					if (!ln.empty())
						fout << ln << "\n";
			}
			else
				std::cout << "\nError: Could not open <" << filename << "> for writing!\n";
		}
	}
	else
		std::cout << "\nError: Could not open <" << filename << "> for reading!\n";
	
}

void DatabaseManager::update_user_in_file(UserBase * pUser)
{
	// Remove the old info
	remove_user_from_file(pUser);
	// Replace with new info
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
	// Retrieve a pointer to the user being referenced
	UserBase* pUser = find_user(username);

	if (pUser) 	// If user is found
	{
		// Remove them from the list
		m_users.erase(username);

		// If they are a player, we need to remove other records
		if (pUser->get_user_type() == UserTypeId::kPlayerUser)
		{
			PlayerUser* pPlayer = static_cast<PlayerUser*>(pUser);
			std::string log_file = std::string("data\\PlayLogs\\") + pPlayer->get_username() + "_play_log.txt";
			std::string trans_file = std::string("data\\Transactions\\") + pPlayer->get_username() + ".txt";
			// Delete their game bag file
			remove(pPlayer->get_game_file().c_str());
			// Delete their play history file
			remove(log_file.c_str());
			// Delete their transactions file
			remove(trans_file.c_str());
		}

		// Remove them from the player user file;
		remove_user_from_file(pUser);
		// Finally, free the heap memory
		delete pUser;
		std::cout << "\nUser <" << username << "> has been erased!\n";
	}
	else
		std::cout << "\nSorry, that username is not our records!\n";
}

void DatabaseManager::add_game_to_file(Game* pGame)
{
	if (pGame) // If pointer is valid
	{
		if (!is_game_in_file(pGame, gameFile)) // and the game is not already recorded
		{
			std::ofstream fout(gameFile, std::ios::out | std::ios::app);
			if (fout)
			{
				// Check that the game has a valid ID
				if (pGame->get_game_id() > 0)
				{
					// Write the game info to the file
					fout << pGame->get_game_id() << " , " <<
						pGame->get_title() << " , " <<
						pGame->get_desc() << " , " <<
						pGame->get_price() << "\n";
				}
			}
			else
				std::cout << "\nError: the game file could not be opened for writing!\n";
		}
	}
}

void DatabaseManager::remove_game_from_file(Game* pGame)
{
	Game::GameId game_id = pGame->get_game_id();
	Game::GameId num;
	// Dummy string for reading lines out of the file
	std::string line;
	std::vector<std::string> file_contents;
	
	std::ifstream fin(gameFile); 
	if (fin)
	{
		while (std::getline(fin, line))
		{
			std::istringstream iss(line);
			// Copy the contents of the file except for the game to be removed
			if ((iss >> num) && (num != game_id))
				file_contents.push_back(line);    
		}

		if (!file_contents.empty())              
		{
			// Write the contents back into the file
			std::ofstream fout(gameFile, std::ios::trunc);
			if (fout)
			{
				for (const auto& ln : file_contents)
					fout << ln << "\n";                
			}
			else
				std::cout << "\nError: Could not open list of games for writing!";
		}
	}
	else
		std::cout << "\nError: Could not open list of games for reading!";
	
}

void DatabaseManager::remove_game_from_bag(Game::GameId game_id, PlayerUser * pPlayer)
{
	Game::GameId num;
	// Dummy string for reading lines from the file
	std::string line;
	std::vector<std::string> file_contents;
	// Open the player's game bag file for reading
	std::ifstream fin(pPlayer->get_game_file());
	if (fin)
	{
		while (std::getline(fin, line))
		{
			// Copy the contents of the file except for the game info
			std::istringstream iss(line);
			iss >> num;
			if (num != game_id)
				file_contents.push_back(line);    
		}

		if (!file_contents.empty())               
		{
			// Write the modified contents back into the file
			std::ofstream fout(pPlayer->get_game_file(), std::ios::trunc);
			if (fout)
			{
				for (const auto& ln : file_contents)
					fout << ln << "\n";                
			}
			else
				std::cout << "\nError: Could not open <" << pPlayer->get_username() << ">'s bag file for writing!";
		}
	}
	else
		std::cout << "\nError: Could not open <" << pPlayer->get_username() << ">'s bag file for reading!";
}


void DatabaseManager::update_game_in_file(Game* pGame)
{
	// Delete the old info
	remove_game_from_file(pGame);
	// Replace with new info
	add_game_to_file(pGame);
}

void DatabaseManager::add_game(Game* pGame)
{
	if (pGame) // If the pointer is valid
	{
		// Add to the list of games and write out to file storage
		m_games.insert(std::make_pair(pGame->get_game_id(), pGame));
		add_game_to_file(pGame);
	}
	else return;
}

void DatabaseManager::remove_game(Game::GameId game_id)
{
	// Retrieve a pointer to the game being referenced
	Game* pGame = find_game(game_id);
	if (pGame) // If pointer is valid
	{
		std::string title = pGame->get_title();
		// Remove from the list of games
		m_games.erase(game_id);
		// Delete the game from the master list file
		remove_game_from_file(pGame);
		// Free the game's memory from the heap
		delete pGame;
		std::cout << "\nGame <" << title << "> has been erased!\n";
	}
	else
		std::cout << "\nSorry, that game is not our records!\n";
}

void DatabaseManager::add_game_to_bag(Game::GameId game_id, PlayerUser * pPlayer)
{
	// Write the game's ID out to the player's bag file
	std::ofstream fout(pPlayer->get_game_file(), std::ios::out | std::ios::app);
	if (fout)
	{
		fout << game_id << "," << "\n";
	}
	else
		std::cout << "Error, could not open <" << pPlayer->get_username() << ">'s game bag file for writing.";
}

void DatabaseManager::log_playtime(const std::string & time, const std::string & game_title, const char* log_file) const
{
	// Write the play session out to the user's play log
	std::ofstream fout(log_file, std::ios::out | std::ios::app);
	if (fout)
	{
		fout << date::current_date_and_time() << " , " << game_title << " , " << time << "\n";
	}
	else
		std::cout << "\nError: Couldn't open play log!\n";
}

void DatabaseManager::log_transaction(const Transaction& trans, const char* transaction_file) const
{
	// Write the transaction out to the corresponding file
	std::ofstream fout(transaction_file, std::ios::out | std::ios::app);
	if (fout)
	{
		fout << trans.transaction_to_string(',') << "\n";
	}
	else
		std::cout << "\nError: Couldn't open transaction file!\n";
}

void DatabaseManager::play_log_header() const
{
	std::cout << "\n" << std::left << std::setw(10) << "DATE" << std::setw(3) << "|"
		<< std::setw(10) << "TIME" << std::setw(3) << "|"
		<< std::setw(MAX_GAME_TITLE) << "TITLE" << std::setw(3) << "|"
		<< std::setw(10) << "PLAY TIME";
}

void DatabaseManager::transaction_header() const
{
	std::cout << "\n" << std::left << std::setw(20) << "DATE AND TIME" << std::setw(3) << "|"
		<< std::setw(35) << "DETAILS OF TRANSACTION" << std::setw(3) << "|"
		<< std::setw(12) << "AMOUNT (\x9C)" << std::setw(3) <<  "|"
		<< std::setw(16) << "NEW BALANCE (\x9C)" << "\n\n";
}

void DatabaseManager::display_play_log(const std::string & username)
{
	// Retrieve a pointer to the user being referenced
	UserBase* pUser = find_user(username);
	if (pUser)
	{
		// Check that we have a player user
		if (pUser->get_user_type() == UserTypeId::kPlayerUser)
		{
			// Open the user's play log for reading
			std::string log_file = std::string("data\\PlayLogs\\") + pUser->get_username() + "_play_log.txt";
			std::ifstream fin(log_file.c_str());
			if (fin)
			{
				std::string line;
				char c;
				std::string date, time, title, play;
				std::vector<std::string> play_times;
				// Prepare the table header
				play_log_header();

				// Read out all non-empty file lines
				while (std::getline(fin, line) && (line != ""))
				{
					std::istringstream iss(line);
					iss >> date >> time >> c >> title >> c >> play;
					// Store the play times for statistics
					play_times.push_back(play);
					// Format the output
					std::cout << "\n" << std::left << std::setw(10) << date << std::setw(3) << "|"
						      << std::setw(10) << time << std::setw(3) << "|"
						      << std::setw(MAX_GAME_TITLE) << title << std::setw(3) << "|"
						      << std::setw(10) << play;
				}

				// Determine the cumulative play time and the average time per session in seconds
				long total_secs(0);
				for (auto t : play_times)
					total_secs += date::formatted_time_to_secs(t);
				long average_secs = total_secs / play_times.size();

				// Convert these times to HH:MM:SS format and print to console
				std::string total_time = date::secs_to_formatted_time(total_secs);
				std::string average_time = date::secs_to_formatted_time(average_secs);
			
				std::cout << "\n\n" << std::left << std::setw(15) << "TOTAL TIME: " << std::setw(10) << total_time;
				std::cout << "\n" << std::left << std::setw(15) << "AVERAGE TIME: " << std::setw(10) << average_time;
				std::cout << std::endl << std::endl;
			}
			else
				std::cout << "\nSorry, could not open that player's play log!\n";
	
		}
	}
	else
		std::cout << "\nSorry, that username is not our records!\n";
}

void DatabaseManager::display_transactions(const std::string & username)
{
	// Retrieve a pointer to the user being referenced
	UserBase* pUser = find_user(username);
	if (pUser) // If pointer is valid
	{
		// Check that we have a player user
		if (pUser->get_user_type() == UserTypeId::kPlayerUser)
		{
			// Open their transaction history for reading
			std::string trans_file = std::string("data\\Transactions\\") + pUser->get_username() + ".txt";
			std::ifstream fin(trans_file.c_str());
			if (fin)
			{
				std::string line;
				std::vector<std::string> lines;
				// Copy the contents of the file
				while (std::getline(fin, line) && (line != ""))
				{
					// Store formatted transaction strings in a vector
					Transaction temp(line);
					lines.push_back(temp.transaction_to_string('|'));
				}
				// Prepare the transaction table
				transaction_header();

				// Print the transactions to the screen
				for (auto ln : lines)
				{
					std::cout << ln << "\n";
				}
			}
			else
				std::cout << "\nSorry, " << pUser->get_username() << "'s transactions could not be viewed!\n";
		}
		else
			std::cout << "\nSorry, " << pUser->get_username() << " does not have any transactions!\n";
		
	}
	else
		std::cout << "\nSorry, that username is not on our records!\n";
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

std::vector<PlayerUser*> DatabaseManager::find_users_who_own_game(const Game::GameId game_id)
{
	std::vector<PlayerUser*> result;
	// Iterate over all player users only
	auto playerVisitorLambda = [&result, &game_id](PlayerUser* pPlayer)
	{
		// Store a pointer to the player if they own the game
		if(pPlayer->owns_game(game_id))
			result.push_back(pPlayer);
	};
	DatabaseManager::instance().visit_players(playerVisitorLambda);
	return result;
}

std::vector<std::string> DatabaseManager::search_games(const std::string & query, SearchDescriptor flag, const char* filename)
{
	std::vector<std::string> found_titles;
	std::string title, desc_word, line;
	Game::GameId id;
	char c;

	// Open the file to search in
	std::ifstream fin(filename);    
	while (std::getline(fin, line))  
	{
		std::istringstream iss(line);
		if ((iss>>id) && (iss>>c) && std::getline(iss, title, ','))
		{
			// Game titles are bordered with whitespace, extract just the characters
			title = title.substr(1, title.length() - 2);
			// If we are looking for a title...
			if (flag == SearchDescriptor::kTitle)  
			{
				if (query == title) // Store titles that match our search exactly 
				{
					found_titles.push_back(title);    
				}
			}
			// If we are looking for a description...
			else if (flag == SearchDescriptor::kDesc) 
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
		// Extract all the information required to construct a user
		std::istringstream iss(line);
		iss >> username >> c >> password >> c >> email >> c >> funds;
		// Delegate the creation to a user factory
		m_uFactory.createNewUser(usertype, username, password, email, funds);
	}
}

void DatabaseManager::load_games_from_file(const char * filename)
{
	std::string line;
	std::ifstream file(filename);
	
	while (std::getline(file, line))
	{
		char c;
		Game::GameId game_id;
		std::string title;
		std::string desc;
		double price;
		//Extract all the information required to construct a game
		std::istringstream iss(line);
		if ((iss >> game_id) && (iss >> c) && std::getline(iss, title, ',')
			&& std::getline(iss, desc, ',') && (iss >> price))
		{
			// Delegate the creation to a game factory
			m_gFactory.createNewGame(game_id, title, desc, price);
		}
	}
}

void DatabaseManager::load_game_bag_from_file(PlayerUser * pPlayer)
{
	Game::GameId game_id;
	std::string line;
	// Get the path to the player's bag file
	std::string game_file = pPlayer->get_game_file();
	std::ifstream fin(game_file);
	if (fin)
	{
		// If the player has one to open, extract the information
		while (std::getline(fin, line))
		{
			std::istringstream iss(line);
			// Add each game to the player user's list
			if ((iss >> game_id))
				pPlayer->add_to_game_list(game_id);
		}
	}
	else
	{
		// Create a bag file for the user if there is not one
		std::ofstream fout(game_file, std::ios::trunc);
	}
}

void DatabaseManager::load_games_each_player()
{
	for (auto& it : m_users)
	{
		UserBase* pUser = it.second;
		if (pUser->get_user_type() == UserTypeId::kPlayerUser)
		{
			PlayerUser* pPlayer = static_cast<PlayerUser*>(it.second);
			load_game_bag_from_file(pPlayer);
		}
	}
}


bool DatabaseManager::is_user_in_file(UserBase * pUser, const char* filename)
{
	std::string uname = pUser->get_username();
	std::string listUname;
	// Open the file for reading
	std::ifstream fin(filename);
	if (fin) // If successful
	{
		// Look for the username at the start of each line
		while (fin >> listUname)
		{
			// If it's a match, user is in this file
			if (listUname == uname)
				return true;
			// If not, move to the next line
			else
				fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		// No more lines, user not found here
		return false;
	}
	else
	{
		std::cout << "\nError: Could not open the file <" << filename << ">!";
		return false;
	}
	
}

bool DatabaseManager::is_game_in_file(Game* pgame, const char * filename)
{
	// Games are stored in files by ID first
	Game::GameId game_id = pgame->get_game_id();
	Game::GameId file_game_id;
	// Open the file for reading
	std::ifstream fin(filename);
	if (fin)
	{
		// Look for the game ID at the start of each line
		while (fin >> file_game_id)
		{
			// If it's a match, the game is found
			if (file_game_id == game_id)
				return true;
			// If not, move to the next line
			else
				fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		// No more lines, game not found
		return false;
	}
	else
	{
		std::cout << "\nError: Could not open the file <" << filename << ">!";
		return false;
	}
}





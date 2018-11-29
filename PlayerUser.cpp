//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "DatabaseManager.h"
#include "Date.h"

#include <algorithm>

void PlayerUser::add_to_game_list(const Game::GameId game_id)
{
	m_ownedGames.push_back(game_id);
}

void PlayerUser::remove_from_game_list(const Game::GameId game_id)
{
	m_ownedGames.remove(game_id);
}

void PlayerUser::list_owned_games() const
{
	for (auto const& g : m_ownedGames)
	{
		Game* pgame = DatabaseManager::instance().find_game(g);
		if (pgame)
			std::cout << *pgame << "\n";
	}
}

void PlayerUser::add_funds()
{
	// Ask the user how much money to deposit
	double amount;
	std::cout << "\nPlease enter the amount you would like to add: \x9C";

	while (!(std::cin >> amount) || amount < 0)
	{
		std::cout << "Invalid input! \nPlease try that again: \x9C";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	// If input is valid, deposit this amount
	m_wallet.deposit(amount);
	// Inform the user of successful deposit
	std::cout << "Deposit successful! \x9C";
	std::cout << std::setprecision(2) << amount << std::fixed;
	std::cout << " added to your wallet.\n\n";

	// Create a transaction and log it to the user-specific file
	std::string date_time = date::current_date_and_time();
	std::string details = get_username() + " made a deposit";
	Transaction trans(date_time, details, amount, get_available_funds());
	std::string trans_log = std::string("data\\Transactions\\") + get_username() + ".txt";
	DatabaseManager::instance().log_transaction(trans, trans_log.c_str());
	
	
	DatabaseManager::instance().update_user_in_file(this);
}

void PlayerUser::add_funds(double a)
{
	// Add the amount to their account
	m_wallet.deposit(a); 
	
	// Create a transaction and log it to the user-specific file
	std::string date_time = date::current_date_and_time();
	std::string details = get_username() + " was reimbursed";
	Transaction trans(date_time, details, a, get_available_funds());
	std::string trans_log = std::string("data\\Transactions\\") + get_username() + ".txt";
	DatabaseManager::instance().log_transaction(trans, trans_log.c_str());
	// Update the files' records of the user
	DatabaseManager::instance().update_user_in_file(this);
}

void PlayerUser::buy_game(const Game::GameId game_id)
{
	// Check for valid game ID
	if (game_id == -1)
		return;
	// Find the game with this ID in the system
	Game* pgame = DatabaseManager::instance().find_game(game_id);
	
	if (pgame) // If the game is found
	{
		// Check that it is not in the player's bag already
		if ((std::find(m_ownedGames.begin(), m_ownedGames.end(), game_id)) != m_ownedGames.end())
		{
			std::cout << "\n\nYou already own the title <" << pgame->get_title() << ">!\n";
			return;
		}
		// Provide a wall so that they confirm their purchase
		char option;
		std::cout << "\nWould you like to buy <" << pgame->get_title() << ">? (Y/N) \n";
		while (std::cin >> option)
		{
			// If the player says yes
			if (toupper(option) == 'Y')
			{
				// And they have enough money
				if (m_wallet.withdraw(pgame->get_price()))
				{
					// Add the game to the user's list
					m_ownedGames.push_back(game_id);
					// Update the files' records of the user
					DatabaseManager::instance().update_user_in_file(this);
					// Add to the player's game file
					DatabaseManager::instance().add_game_to_bag(game_id, this);
					
					// Create a transaction and log it in the database
					std::string filepath = std::string("data\\Transactions\\") + get_username() + ".txt";
					double cost = -pgame->get_price();
					std::string details = get_username() + " bought" + pgame->get_title();
					Transaction trans(date::current_date_and_time(), details, cost, get_available_funds());
					DatabaseManager::instance().log_transaction(trans, filepath.c_str());
					return;
				}
				else
					return;
			}
			else if (toupper(option) == 'N')
				return;
			else
				std::cout << "\nInvalid input! Try again. (Y/N) \n";
		}
	}
	else
		std::cout << "\nSorry, that game is not in our store.";
}

void PlayerUser::play_game(const Game::GameId game_id)
{
	Stopwatch timer; // Start a timer for their session
	bool isPlaying = true; // Flag to show they are still playing
	// Find the game in the database
	Game* pGame = DatabaseManager::instance().find_game(game_id);
	// Simulate 'launching' a game
	std::cout << "\nYou are now playing <" << pGame->get_title() << ">! \nPress <Enter> to exit";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	// Await user input to terminate the play session and exit the game
	while (isPlaying)
	{
		std::cin.sync();
		std::cin.get();
		if (std::cin)
			isPlaying = false;
	}
	// Read the stopwatch and log this session to the database
	int secs = timer.time_elapsed();
	std::string time =  date::secs_to_formatted_time(secs);
	std::string log_file = std::string("data\\PlayLogs\\") + this->get_username() + "_play_log.txt";
	DatabaseManager::instance().log_playtime(time, pGame->get_title(), log_file.c_str());
}

bool PlayerUser::owns_game(const Game::GameId game_id)
{
	auto it = std::find(m_ownedGames.begin(), m_ownedGames.end(), game_id);
	if (it != m_ownedGames.end())
		return true;
	else
		return false;
}

bool PlayerUser::is_bag_empty()
{
	if (m_ownedGames.empty())
		return true;
	else
		return false;
}


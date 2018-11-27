//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include <algorithm>


#include "DatabaseManager.h"

// ------------------------
// PlayerUser class implementation
// ------------------------

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
	double amount;
	std::cout << "\nPlease enter the amount you would like to add: \x9C";
	while (!(std::cin >> amount) || amount < 0)
	{
		std::cout << "Invalid input! \nPlease try that again: \x9C";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	m_wallet.deposit(amount);
	std::cout << "Deposit successful! \x9C";
	std::cout << std::setprecision(2) << amount << std::fixed;
	std::cout << " added to your wallet.\n\n";

	// Update the files' records of the user
	DatabaseManager::instance().update_user_in_file(this);
}

void PlayerUser::buy_game(const Game::GameId game_id)
{
	if (game_id == -1)
		return;
	Game* pgame = DatabaseManager::instance().find_game(game_id);
	
	if (pgame)
	{
		if ((std::find(m_ownedGames.begin(), m_ownedGames.end(), game_id)) != m_ownedGames.end())
		{
			std::cout << "\n You already own the title <" << pgame->get_title() << ">!\n";
			return;
		}

		char option;
		std::cout << "\nWould you like to buy <" << pgame->get_title() << ">? (Y/N) \n";
		while (std::cin >> option)
		{
			if (toupper(option) == 'Y')
			{
				if (m_wallet.withdraw(pgame->get_price()))
				{
					// Add the game to the user's list
					m_ownedGames.push_back(game_id);
					// Update the files' records of the user
					DatabaseManager::instance().update_user_in_file(this);
					// Add to the player's game file
					DatabaseManager::instance().add_game_to_bag(game_id, this);
					// Create a transaction
					return;
				}
				else
					return;
			}
			else if (toupper(option) == 'N')
				return;
			else
				std::cout << "Invalid input! Try again. (Y/N) \n";
		}
	}
	else
		std::cout << "\nSorry, that game is not in our store.";
}

void PlayerUser::play_game(const Game::GameId game_id)
{
	bool isPlaying = true;
	Game* pGame = DatabaseManager::instance().find_game(game_id);
	std::cout << "\nYou are now playing <" << pGame->get_title() << ">! \nPress <Enter> to exit";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while (isPlaying)
	{
		std::cin.sync();
		std::cin.get();
		if (std::cin)
			isPlaying = false;
	}
}

bool PlayerUser::owns_game(const Game::GameId game_id)
{
	auto it = std::find(m_ownedGames.begin(), m_ownedGames.end(), game_id);
	if (it != m_ownedGames.end())
		return true;
	else
		return false;
}

bool PlayerUser::empty_bag()
{
	if (m_ownedGames.empty())
		return true;
	else
		return false;
}

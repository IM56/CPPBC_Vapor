//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include <iomanip>

#include "MenuSystem.h"


MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::user_header() const
{
	std::cout << std::left << std::setw(MAX_USERNAME) << "\nAUTHORITY"
		<< std::setw(3) << "|" << std::setw(MAX_USERNAME) << "USERNAME" << std::setw(3) << "|"
		<< std::setw(2 * MAX_USERNAME) << "EMAIL" << std::setw(3) << "|" << std::setw(8) << "FUNDS\n";
}

void MenuSystem::game_header() const
{
	std::cout << std::left << std::setw(6) << "ID" << std::setw(3) << "|"
		<< std::setw(MAX_GAME_TITLE) << "TITLE" << std::setw(3) << "|"
		<< std::setw(MAX_DESCRIPTION) << "DESCRIPTION" << std::setw(3) << "|"
		<< std::setw(8) << "PRICE\n" << std::endl;
}

void MenuSystem::list_all_games() const
{
	game_header();

	auto gameVisitorLambda = [](const Game& rGame) 
	{
		std::cout << rGame << "\n";
	};
	DatabaseManager::instance().visit_games(gameVisitorLambda);
	std::cout << std::endl;
}

void MenuSystem::list_all_users() const
{
	user_header();

	auto userVisitorLambda = [](const UserBase& rUser) 
	{
		std::cout << rUser;
	};
	DatabaseManager::instance().visit_users(userVisitorLambda);
	std::cout << std::endl;
}

int MenuSystem::run_login_screen()
{
	m_pUser = nullptr;

	// Ask for username and password
	std::string username;
	std::string password;

	std::cout << "Login Menu\n";
	std::cout << "Username: ";
	std::cin >> username;

	std::cout << "Password: ";
	std::cin >> password;

	// Find the user and check password
	auto pUser = DatabaseManager::instance().find_user(username);
	// If login successful, greet the user
	if (pUser && pUser->get_password() == password)
	{
		m_pUser = pUser;
		std::cout << "Hi " << m_pUser->get_username() << "\n";
	}
	// If login fails, exit back to main menu
	else
	{
		std::cout << "No such username or password!\n";
	}
	return 0;
}

int MenuSystem::run_admin_user_menu()
{
	int result = 0;
	do
	{
		std::cout << "\nAdmin Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List All Users\n";
		std::cout << "(3) Add Game\n";
		std::cout << "(4) Remove Game\n";
		std::cout << "(5) Add User\n";
		std::cout << "(6) Remove User\n";
		std::cout << "(7) View Play Logs\n";
		std::cout << "(8) View Transactions\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': list_all_users(); break;
		case '3': create_game(); break;
		case '4': remove_game(); break;
		case '5': create_user(); break;
		case '6': remove_user(); break;
		case '7': view_play_log(); break;
		case '8': view_transactions(); break;
		case 'q': result = -1; break;
		default:  std::cout << "INVALID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

int MenuSystem::run_player_user_menu()
{
	// Cast the pointer to a player to access player-only methods
	PlayerUser* pPlayerUser = static_cast<PlayerUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "\nPlayer Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "Wallet \x9C" << std::setprecision(2) << std::fixed <<
			m_pUser->get_available_funds() <<  "\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List My Games\n";
		std::cout << "(3) Buy Game\n";
		std::cout << "(4) Play Game\n";
		std::cout << "(5) Delete Game\n";
		std::cout << "(6) Add Funds\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2':
		{
			// Check that the player owns games first
			if (pPlayerUser->is_bag_empty())
				std::cout << "\nYou don't own any games yet!\n";
			else
			{
				game_header();
				pPlayerUser->list_owned_games();
			}
			break;
		}
		case '3': pPlayerUser->buy_game(game_menu()); break;
		case '4': play_game(pPlayerUser); break;
		case '5': delete_game(pPlayerUser); break;
		case '6': pPlayerUser->add_funds(); break;
		case 'q': result = -1; break;
		default:  std::cout << "INVALID OPTION\n"; break;
		}
	} while (result == 0);

	// force logout.
	m_pUser = nullptr;

	return 0;
}

int MenuSystem::run_unknown_user_menu()
{
	// in this menu we get the username and password.
	int result = 0;

	std::cout << "Main Menu\n";
	std::cout << "(1) List All Games\n";
	std::cout << "(2) Login\n";
	std::cout << "(q) Quit\n";

	char option;
	std::cin >> option;

	switch (option)
	{
	case '1': list_all_games(); break;
	case '2': run_login_screen(); break;
	case 'q': result = -1;  break;
	default:  std::cout << "INVALID OPTION\n"; break;
	}

	return result;
}

void MenuSystem::create_user()
{
	/////////////////////////////////////////
	// Find out the type of user to create //
	/////////////////////////////////////////
	UserTypeId usertype;
	std::cout << "\nWhat type of user would you like to create?"
		<< "\n(1) Create an admin user"
		<< "\n(2) Create a player user"
		<< "\nPress any other key to exit this menu\n";

	char option;
	std::cin >> option;

	switch (option)
	{
	case '1':
	{
		usertype = UserTypeId::kAdminUser;
		break;
	}
	case '2':
	{
		usertype = UserTypeId::kPlayerUser;
		break;
	}
	default:
		return;
	}

	//////////////////////////////////////
	// Get new username from user input //
	//////////////////////////////////////
	Username uname;
	while (uname == "" || uname.length() > MAX_USERNAME)  // Prompt user to input the new username for the account
	{
		std::cout << "\nEnter the new username (15 characters or fewer): ";
		std::cin >> uname;


		if (DatabaseManager::instance().find_user(uname))  // Check if the username already exists
		{
			std::cout << "\nSorry, the name <" << uname << "> is already taken! Please try again.";
			uname = "";
		}

		if (uname.length() > MAX_USERNAME)
			std::cout << "Sorry, that username was too long. Please try again.";
	}

	//////////////////////////////////////////////////
	// Get and confirm new password from user input //
	//////////////////////////////////////////////////
	std::string password, verify;
	while (password == "")   // Prompt user until they provide a password
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nPlease choose a password for your account: ";
		std::cin >> password;
		if (password == "")
			std::cout << "\nSorry, that is not a valid password! Try again!";

		while (verify == "" || verify != password)  // Keep asking for confirmation if incorrect
		{
			std::cout << "\nPlease confirm your password: ";
			std::cin >> verify;

			if (verify != password)
			{
				std::cout << "\n Sorry, those passwords don't match! Try again!";
				verify.clear();
				password.clear();
				break;
			}
		}
	}


	/////////////////////////////////////////////////////
	// Get and check new email address from user input //
	/////////////////////////////////////////////////////
	std::string email;
	while (email == "")
	{
		std::cout << "\nPlease enter an email address: ";
		std::cin >> email;
		if (email == "" || email.find_first_of('@') == std::string::npos || email.find_first_of('@') != email.find_last_of('@'))
		{
			std::cout << "\nSorry, that wasn't right! Try again!";
			email = "";
		}
	}

	// Pass the extracted information to a user factory
	m_uFactory.createNewUser(usertype, uname, password, email);
}


void MenuSystem::remove_user()
{
	// Ask for the user you would like to remove
	std::string uname;
	std::cout << "\nPlease enter the username you would like to remove: ";
	std::cin >> uname;
	if (uname == m_pUser->get_username())
		std::cout << "\nYou cannot delete yourself!\n";
	else
	{
		// Delegate to the database manager to remove the user from the system
		DatabaseManager::instance().remove_user(uname);
	}
}
		

void MenuSystem::view_play_log()
{
	// Ask for the user you want to view
	std::string uname;
	std::cout << "\nPlease enter the username whose log you would like to see: ";
	std::cin >> uname;
	// Get the DB manager to display their log
	DatabaseManager::instance().display_play_log(uname);
}

void MenuSystem::view_transactions()
{
	// Ask for the user you want to view
	std::string uname;
	std::cout << "\nPlease enter the username whose transactions you would like to see: ";
	std::cin >> uname;
	// Get the DB manager to display their transaction history
	DatabaseManager::instance().display_transactions(uname);
}

Game* MenuSystem::create_game()
{
	/////////////////////////////////
	// Get Game ID from user input //
	/////////////////////////////////
	Game::GameId game_id;
	std::string title;
	std::string description;
	double price;

	do
	{
		std::cout << "\nEnter a unique 4-digit ID number for the new game: ";
		std::cin >> game_id;

		if (!std::cin) // If there was bad input, rinse and repeat
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			game_id = -1;
		}

		else if (game_id < 0 || game_id > 9999)
			std::cout << "\nSorry, that was not a valid ID number! Please try again.";

		// If the ID already belongs to a game, prompt for a different one
		else if (DatabaseManager::instance().find_game(game_id))
		{
			std::cout << "\nSorry, that ID number is already taken! Please try again.";
			game_id = -1;
		}
	} while (game_id < 0 || game_id > 9999); // ID must be positive and no longer than 4 digits

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	////////////////////////////////////
	// Get Game title from user input //
	////////////////////////////////////

	do // Prompt user to input the new (non-empty) title
	{
		std::cout << "\nEnter the new game's title (20 characters or fewer, no commas): ";
		std::getline(std::cin, title);
		// Make sure that there are no commas, these act as delimiters in file storage
		if (title.find(',') != std::string::npos)
		{
			std::cout << "\nPlease don't put commas in the title. Try again.";
			title.clear();
		}
		// Make sure the title is brief and not too long
		if (title.length() > MAX_GAME_TITLE)
		{
			std::cout << "\nThat title was too long. Try again.";
			title.clear();
		}

		// Check that the title is not already taken
		std::vector<std::string> matching_titles = DatabaseManager::instance().search_games(title, SearchDescriptor::kTitle, DatabaseManager::instance().gameFile);
		// If it is taken, prompt the user for a different title
		if (!matching_titles.empty())
		{
			std::cout << "\nSorry, that title already exists. Try again.";
			title.clear();
		}

	} while (title == ""); // Make sure the title is non-empty

	
	////////////////////////////////////////////
	// Get game's description from user input //
	////////////////////////////////////////////
	do // Prompt user to input the new (non-empty) description
	{
		std::cout << "\nEnter the new game's description (40 characters or fewer, no commas): ";
		std::getline(std::cin, description);

		if (description.find(',') != std::string::npos)
		{
			std::cout << "\nPlease don't put commas in the description. Try again.";
			description.clear();
		}

		if (description.length() > MAX_DESCRIPTION)
		{
			std::cout << "\nThat description was too long. Try again.";
			description.clear();
		}

	} while (description == "");


	//////////////////////////////////////
	// Get game's price from user input //
	//////////////////////////////////////
	do
	{
		std::cout << "\nPlease enter a price for the game (pounds.pence): ";
		std::cin >> price;

		if (!std::cin) // If there was bad input, rinse and repeat
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			price = -1;
		}

		if (price < 0)
			std::cout << "\nSorry, that wasn't a vaild price. Please try again: ";
	} while (price < 0); // Make sure the price is a positive amount


	return m_gFactory.createNewGame(game_id, title, description, price);
}

void MenuSystem::play_game(PlayerUser * pPlayer)
{
	Game::GameId game_id;
	std::string input;
	// Check that the user has any games first
	if (pPlayer->is_bag_empty())
	{
		std::cout << "\nYou don't own any games yet!\n";
		return;
	}
	// Ask them to provide the ID of the game they wish to play
	std::cout << "\nWhich game would you like to play?" << std::endl;
	while (input == "")
	{
		// List the games for a quick and easy reference
		game_header();
		pPlayer->list_owned_games();
		std::cout << "\nPlease enter the ID of the game you would like to play, or press (Q) to quit: ";
		std::cin >> input;
		if (std::cin)
		{
			std::istringstream iss(input);
			// Player has chosen to leave this menu
			if (input == "Q" || input == "q")
				return;
			// Player has entered a valid game ID
			else if (iss >> game_id && DatabaseManager::instance().find_game(game_id))
			{
				// Check that they own this game
				if (pPlayer->owns_game(game_id))
				{
					// If so, load up the game
					pPlayer->play_game(game_id);
					return;
				}
				else
				{
					std::cout << "\nThat's not a game in your bag... Try again!";
					input.clear();
				}
			}
			else
			{
				std::cout << "\nSomething wasn't right there... Try again!";
				input.clear();
			}
		}
	}
	
}

void MenuSystem::remove_game()
{
	// Ask which game you would like to remove from the system
	Game::GameId game_id;
	std::cout << "\nPlease enter the ID of the game you would like to remove: ";
	std::cin >> game_id;
	// Find out which players will be affected by this removal
	std::vector<PlayerUser*> owners = DatabaseManager::instance().find_users_who_own_game(game_id);
	for (auto& o : owners)
	{
		// Remove it from their records
		DatabaseManager::instance().remove_game_from_bag(game_id, o); // Delete it from bag file
		o->remove_from_game_list(game_id);  // Remove from player's own list
	}
	// Give the user the option to replace the game
	std::cout << "\nWould you like to replace the game? (Y/N)";
	char option;
	while (std::cin >> option)
	{
		// If so, take them to the game creation screen
		if (toupper(option) == 'Y')
		{
			Game* pGame = create_game();
			for (auto& o : owners)
			{
				// Put the new game in the affected players' bags to replace the old one
				o->add_to_game_list(pGame->get_game_id()); // Add it to the player's own list
				DatabaseManager::instance().add_game_to_bag(pGame->get_game_id(), o); // Add to the bag file
			}
			DatabaseManager::instance().add_game(pGame);
			break;
		}
		// If not replaced, refund the affected players the amount they paid for the game.
		else if (toupper(option) == 'N')
		{
			Game* pOld = DatabaseManager::instance().find_game(game_id);
			for (auto& o : owners)
			{
				o->add_funds(pOld->get_price());
			}
			break;
		}
		else
			std::cout << "Invalid input! Try again. (Y/N) \n";
	}
	// Get the DB manager to remove the game from all records
	DatabaseManager::instance().remove_game(game_id); 
}

void MenuSystem::delete_game(PlayerUser * pPlayer)
{
	Game::GameId game_id;
	// Make sure that they own games to delete
	if (pPlayer->is_bag_empty())
	{
		std::cout << "\nYou don't own any games yet!\n";
		return;
	}
	// Ask them which game they would like to get rid of
	else
	{
		std::cout << "\nEnter the ID of the game you would like to remove: ";
		std::cin >> game_id;
		if (std::cin)
		{
			// Check that this game is on record
			Game* pGame = DatabaseManager::instance().find_game(game_id);
			// Check that the player owns this game
			if (pPlayer->owns_game(game_id))
			{
				// Take it down from their list
				pPlayer->remove_from_game_list(game_id);
				// Remove it from their bag file
				DatabaseManager::instance().remove_game_from_bag(game_id, pPlayer);
				// Inform the player of successful removal
				std::cout << "<" << pGame->get_title() << "> deleted from your account.";
				return;
			}
			else
			{
				std::cout << "\nYou don't own <" << pGame->get_title() << ">!\n";
				return;
			}
				
		}
	}	
}

int MenuSystem::game_menu()
{
	int result = 0;

	std::cout << "\nShop for games\n";
	std::cout << "(1) Purchase a Title\n";
	std::cout << "(q) Quit\n";

	char option;
	std::cin >> option;

	switch (option)
	{
	case '1': 
	{
		list_all_games();
		std::cout << "\nPlease enter the ID of the game you would like to purchase: ";
		std::cin >> result;
		break;
	}
	case 'q': result = -1;  break;
	default:  std::cout << "INVALID OPTION\n"; break;
	}

	return result;
}


int MenuSystem::run()
{
	int result = 0;
	do
	{
		if (!m_pUser)
		{
			result = run_unknown_user_menu();
		}
		else
		{
			switch (m_pUser->get_user_type())
			{
				case UserTypeId::kPlayerUser: result = run_player_user_menu(); break;
				case UserTypeId::kAdminUser: result = run_admin_user_menu(); break;
				default: result = -1; break;
			}
		}
	} while (result == 0);

	return 0;
}



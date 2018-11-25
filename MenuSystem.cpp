//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include <iomanip>

#include "MenuSystem.h"

std::ostream& operator<< (std::ostream& os, const UserBase& user)
{
	if (user.get_user_type() == UserTypeId::kAdminUser)
		os << std::left << std::setw(MAX_USERNAME) << "\nAdministrator" << std::setw(3) << "|";
	else if (user.get_user_type() == UserTypeId::kPlayerUser)
		os << std::left << std::setw(MAX_USERNAME) << "\nPlayer" << std::setw(3) << "|";

	os << std::setw(MAX_USERNAME) << user.get_username() << std::setw(3) << "|"
		<< std::setw(2 * MAX_USERNAME) << user.get_email() << std::setw(3) << "|";

	double funds = user.get_available_funds();

	if (funds < 0)
		os << std::setw(8) << "N/A";
	else
		os << "\x9C" << std::setw(7) << std::setprecision(2) << std::fixed << funds;

	return os;
}

MenuSystem& MenuSystem::instance()
{
	static MenuSystem s_instance;
	return s_instance;
}

void MenuSystem::list_all_games() const
{
	auto gameVisitorLambda = [](const Game& rGame) {
		std::cout << rGame.get_title() << "\n";
	};

	DatabaseManager::instance().visit_games(gameVisitorLambda);
}

void MenuSystem::list_all_users() const
{
	std::cout << std::left << std::setw(MAX_USERNAME) << "\nAUTHORITY"
		<< std::setw(3) << "|" << std::setw(MAX_USERNAME) << "USERNAME" << std::setw(3) << "|"
		<< std::setw(2 * MAX_USERNAME) << "EMAIL" << std::setw(3) << "|" << std::setw(8) << "FUNDS";

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

	// in this menu we get the username and password.
	std::string username;
	std::string password;

	std::cout << "Login Menu\n";
	std::cout << "Username: ";
	std::cin >> username;

	std::cout << "Password: ";
	std::cin >> password;

	// find the user and check password
	auto pUser = DatabaseManager::instance().find_user(username);
	if (pUser && pUser->get_password() == password)
	{
		m_pUser = pUser;
		std::cout << "Hi " << m_pUser->get_username() << "\n";
	}
	else
	{
		std::cout << "No such username or password!\n";
	}
	return 0;
}

int MenuSystem::run_admin_user_menu()
{
	AdminUser* pAdminUser = static_cast<AdminUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "\nAdmin Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List All Users\n";
		std::cout << "(3) Add Game\n";
		std::cout << "(4) Add User\n";
		std::cout << "(5) Remove User\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': list_all_users(); break;
		case '3': std::cout << "TODO\n"; break;
		case '4': create_user(); break;
		case '5': remove_user(); break;
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
	PlayerUser* pPlayerUser = static_cast<PlayerUser*>(m_pUser);
	int result = 0;
	do
	{
		std::cout << "\nPlayer Menu (" << m_pUser->get_username() << ")\n";
		std::cout << "Wallet \x9C" << std::setprecision(2) << std::fixed <<
			pPlayerUser->get_available_funds() <<  "\n";
		std::cout << "(1) List All Games\n";
		std::cout << "(2) List My Games\n";
		std::cout << "(3) Buy Game\n";
		std::cout << "(4) Add Funds\n";
		std::cout << "(q) Logout\n";

		char option;
		std::cin >> option;

		switch (option)
		{
		case '1': list_all_games(); break;
		case '2': std::cout << "TODO\n"; break;
		case '3': pPlayerUser->buy_game(game_menu()); break;
		case '4': pPlayerUser->add_funds(); break;
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

	m_uFactory.createNewUser(usertype, uname, password, email);
}


void MenuSystem::remove_user()
{
	std::string uname;
	std::cout << "\nPlease enter the username you would like to remove: ";
	std::cin >> uname;
	DatabaseManager::instance().remove_user(uname);
}

int MenuSystem::game_menu()
{
	// user can enter the purchase screen
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

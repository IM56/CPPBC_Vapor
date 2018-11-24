//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "AdminUser.h"
#include "DatabaseManager.h"

UserFactory AdminUser::m_uFactory = UserFactory();

void AdminUser::create_user()
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
	while (uname == "")  // Prompt user to input the new username for the account
	{
		std::cout << "\nEnter the new username: ";
		std::cin >> uname;
		if (DatabaseManager::instance().find_user(uname))  // Check if the username already exists
		{
			std::cout << "\nSorry, the name <" << uname << "> is already taken! Please try again.";
			uname = "";
		}
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



void AdminUser::remove_user()
{
	std::string uname;
	std::cout << "\nPlease enter the username you would like to remove: ";
	std::cin >> uname;
	DatabaseManager::instance().remove_user(uname);
}


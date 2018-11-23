//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "Users.h"
#include "DatabaseManager.h"

////////////////////////////////
// UserBase class implementation
/////////////////////////////////


// ------------------------
// PlayerUser class implementation
// ------------------------

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
}

///////////////////////////////////
// AdminUser class implementation
///////////////////////////////////

void AdminUser::createUser()
{
	uFactory.createNewUser();
}



// -------------------------------
// UserFactory implementation
// -------------------------------
UserFactory AdminUser::uFactory;

void UserFactory::createNewUser()
{
	UserTypeId usertype;
	std::cout << "\nWhat type of user would you like to create?"
		<< "\n(1) Create an admin user"
		<< "\n(2) Create a player user"
		<< "\nPress any other key to exit this menu";

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
	{
		usertype = UserTypeId::kInvalid;
		pUserBase = nullptr;
		return;
	}
	}

	Username uname = setUsername();

	if (usertype == UserTypeId::kAdminUser)
		pUserBase = new AdminUser(uname, "", ""); // Create a new admin with this username
	else if (usertype == UserTypeId::kPlayerUser)
		pUserBase = new PlayerUser(uname, "", ""); // Create a new player with this username

	setEmail();								   // Prompt user to provide an email address
	setPassword();							   // Prompt user to provide a password
	DatabaseManager::instance().add_user(pUserBase);			   // Update the database to include the new player user
}

Username UserFactory::setUsername()
{
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
	return uname;
}

void UserFactory::setPassword()
{
	std::string password, verify;
	while (password == "")   // Prompt user until they provide a password
	{
		std::cout << "\nPlease choose a password for your account: ";
		std::cin >> password;
		if (password == "")
			std::cout << "\nSorry, that is not a valid password! Try again!";
	}
	while (verify == "" || verify != password)  // Keep asking for confirmation if incorrect
	{
		std::cout << "\nPlease confirm your password: ";
		std::cin >> verify;
		if (verify != password)
			std::cout << "\n Sorry, those passwords don't match! Try again!";
	}
	pUserBase->set_password(password);
}

void UserFactory::setEmail()
{
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
	pUserBase->set_email(email);
}


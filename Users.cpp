//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once

#include "Users.h"

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

// ------------------------
// UserBuilder class implementation
// ------------------------

UserBuilder::~UserBuilder()
{
	if (pUserBase)
	{
		delete pUserBase;
		pUserBase = nullptr;
	}
}

void UserBuilder::setPassword()
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

void UserBuilder::setEmail()
{
	std::string email;
	while (email == "" || email.find_first_of('@') == std::string::npos || email.find_first_of('@') != email.find_last_of('@'))
	{
		std::cout << "\nPlease enter an email address: ";
		std::cin >> email;
		if (email == "" || email.find_first_of('@') == std::string::npos || email.find_first_of('@') != email.find_last_of('@'))
			std::cout << "\nSorry, that wasn't right! Try again!";
	}
	pUserBase->set_email(email);
}

#pragma once

#include "Users.h"

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

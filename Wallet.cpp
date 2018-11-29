//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "Wallet.h"

void Wallet::deposit(double a)
{
	m_accountFunds += a;
}

bool Wallet::withdraw(double a)
{
	// If amount exceeds the available funds
	if (a > m_accountFunds)
	{
		// Decline the withdrawal
		std::cout << "\nSorry, insufficient funds to complete transaction!\n";
		return false;
	}
	// If withdrawal is possible
	else
	{
		// Deduct from account funds
		m_accountFunds -= a;
		// Inform the user of successful withdrawal
		std::cout << "\nTransaction successful! \x9C" << std::setprecision(2) << std::fixed << a << " has been taken from your balance.\n";
		return true;
	}
		
}

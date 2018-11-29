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
	if (a > m_accountFunds)
	{
		std::cout << "\nSorry, insufficient funds to complete transaction!\n";
		return false;
	}
	else
	{
		m_accountFunds -= a;
		std::cout << "\nTransaction successful! \x9C" << std::setprecision(2) << std::fixed << a << " has been taken from your balance.\n";
		return true;
	}
		
}

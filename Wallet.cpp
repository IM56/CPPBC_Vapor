#include "Wallet.h"

void Wallet::deposit(double a)
{
	m_accountFunds += a;
}

void Wallet::withdraw(double a)
{
	if (a > m_accountFunds)
		std::cout << "Sorry, insufficient funds to complete transaction!\n";
	else
		m_accountFunds -= a;
}

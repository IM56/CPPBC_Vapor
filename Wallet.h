//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <fstream>

class Wallet
{
public:
	Wallet(double funds = 0.0) : m_accountFunds(funds) {}
	~Wallet() {}

	const double get_available_funds() const { return m_accountFunds; }
	void deposit(double);
	bool withdraw(double);

private:
	double m_accountFunds;
};

#endif
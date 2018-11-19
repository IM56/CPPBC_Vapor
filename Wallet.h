#include <iostream>
#include <fstream>

class Wallet
{
public:
	Wallet(double funds = 0.0) : m_accountFunds(funds) {}
	~Wallet() {}

	const double get_available_funds() const { return m_accountFunds; }
	void deposit(double);
	void withdraw(double);

private:
	double m_accountFunds;
};


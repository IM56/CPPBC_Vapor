//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "Transaction.h"

Transaction::Transaction() {}

Transaction::Transaction(const std::string & line)
{
	char c;
	std::istringstream iss(line);
	std::getline(iss, m_date_time, ',');
	std::getline(iss, m_details, ',');
	iss >> m_amount >> c >> m_balance;
}

Transaction::~Transaction() {}

const std::string Transaction::transaction_to_string(char c) const
{
	std::ostringstream oss;
	oss << std::left << std::setw(20) << m_date_time << std::setw(3) << c
		<< std::setw(35) << m_details << std::setw(3) << c
	    << std::setw(12) << std::setprecision(2) << std::fixed << m_amount <<  std::setw(3) << c
		<< std::setw(16) << std::setprecision(2) << std::fixed << m_balance;

	return (oss.str());
}

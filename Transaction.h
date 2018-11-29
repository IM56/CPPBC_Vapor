//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Date.h"

class Transaction
{
public:
	Transaction();
	Transaction(const std::string& date,
				const std::string& details, double amount, double balance)
		: m_date_time(date), m_details(details), m_amount(amount), m_balance(balance){}
	
	Transaction(const std::string& line);
	~Transaction();

	const std::string get_date() const { return m_date_time; }
	const std::string get_details() const { return m_details; }
	double get_amount() const { return m_amount; }
	double get_balance() const { return m_balance; }

	const std::string transaction_to_string(char c) const;

private:
	std::string m_date_time;
	std::string m_details;
	double m_amount;
	double m_balance;
};

#endif


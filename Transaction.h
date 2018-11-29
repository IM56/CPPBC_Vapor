//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

////////////////////////////////////////////////////////////////////////////
// Transaction: Contains info regarding details of a monetary transaction //
////////////////////////////////////////////////////////////////////////////

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

	// Turns a transaction into a formatted string delimited by a chosen character
	const std::string transaction_to_string(char c) const;

private:
	std::string m_date_time; // When it took place
	std::string m_details;   // Why it took place
	double m_amount;         // How much money changed hands
	double m_balance;        // Account's remaining balance
};

#endif


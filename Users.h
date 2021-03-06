//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

//////////////////////////////////////////////////////////////////
// UserBase: An abstract base class representing a generic user //
//////////////////////////////////////////////////////////////////

#pragma once
#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <memory>

using Username = std::string;

constexpr size_t MAX_USERNAME = 15;

// UserTypeId represents an identifier for the specific user type.
enum class UserTypeId
{
	kInvalid = 0
	, kPlayerUser
	, kAdminUser
};


class UserBase
{
public:
	
	UserBase(const Username& username, const std::string& password, const std::string& email)
		: m_username(username)
		, m_password(password)
		, m_email(email)
	{}

	virtual ~UserBase() {}

	virtual const UserTypeId get_user_type() const = 0;

	virtual double get_available_funds() const;
	const std::string get_username() const { return m_username; }
	const std::string get_password() const { return m_password; }
	const std::string get_email() const { return m_email; }

	void set_email(const std::string& val) { m_email = val; }
	void set_password(const std::string& val) { m_password = val; }

protected:


private:
	const Username m_username; // The user's username (unique key)
	std::string m_password;    // User's password.
	std::string m_email;       // User's email address.
	
};

// Simplifies writing a user out to a stream
std::ostream& operator<< (std::ostream& os, const UserBase& user);

///////////////////////
// UserFactory class //
///////////////////////

class UserFactory
{
public:
	UserFactory() : pUserBase(nullptr) {}
	virtual ~UserFactory(){};

	void createNewUser(UserTypeId, const std::string& username, const std::string& password, const std::string& email, double funds = 0.0);

private:
	UserBase* pUserBase;
};

#endif
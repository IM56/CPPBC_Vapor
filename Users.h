//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include <memory>

using Username = std::string;

//--
// UserTypeId represents an identifier for the specific user type.
//--
enum class UserTypeId
{
	kInvalid = 0
	, kPlayerUser
	, kAdminUser
};

//--
// UserBase represents a user base class for the system.
//--
class UserBase
{
public:
	
	UserBase(const Username& username, const std::string& password, const std::string& email)
		: m_username(username)
		, m_password(password)
		, m_email(email)
	{}

	virtual ~UserBase() {}

	// mechanism for identifying the user type at runtime.
	virtual const UserTypeId get_user_type() const = 0;

	const std::string get_username() const { return m_username; }

	const std::string get_password() const { return m_password; }
	void set_password(const std::string& val) { m_password = val; }

	const std::string get_email() const { return m_email; }
	void set_email(const std::string& val) { m_email = val; }
	
protected:


private:
	const Username m_username; // The users username (unique key)
	std::string m_password; // Users password.
	std::string m_email; // Users email address.
	
};


//------
// UserFactory class
// ------

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
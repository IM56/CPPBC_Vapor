//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#pragma once
#ifndef ADMIN_USER_H
#define ADMIN_USER_H

#include "Users.h"

//--
// AdminUser represents a system user who has privileges to modify the system.
//--
class UserFactory;

class AdminUser : public UserBase
{
public:
	// inherit the constructor.
	using UserBase::UserBase;

	// define the specific user type.
	virtual const UserTypeId get_user_type() const override { return UserTypeId::kAdminUser; }


	void createUser();

private:
	static UserFactory uFactory;
};

#endif
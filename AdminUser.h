//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

////////////////////////////////////////////////////////////////////////////////////
// AdminUser: User who can add/remove users, add/remove games, view records, etc. //
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef ADMIN_USER_H
#define ADMIN_USER_H

#include "Users.h"

class AdminUser : public UserBase
{
public:
	// Inherit base constructor
	using UserBase::UserBase;

	// Determines type of user
	virtual const UserTypeId get_user_type() const override { return UserTypeId::kAdminUser; }

private:
};

#endif
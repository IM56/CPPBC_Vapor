//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547


#include "DatabaseManager.h"

/////////////////////////////
// UserBase implementation //
/////////////////////////////



////////////////////////////////
// UserFactory implementation //
////////////////////////////////

void UserFactory::createNewUser(UserTypeId usertype, const std::string& username, const std::string& password, const std::string& email, double funds)
{
	if (usertype == UserTypeId::kAdminUser)
		pUserBase = new AdminUser(username, password, email); // Create a new admin with this username
	else if (usertype == UserTypeId::kPlayerUser)
		pUserBase = new PlayerUser(username, password, email, funds); // Create a new player with this username

	DatabaseManager::instance().add_user(pUserBase);   // Update the database to include the new user
	DatabaseManager::instance().store_user_data(pUserBase);
}


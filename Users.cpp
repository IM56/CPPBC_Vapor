//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547


#include "DatabaseManager.h"

/////////////////////////////
// UserBase implementation //
/////////////////////////////

double UserBase::get_available_funds() const
{
	return -1;
}

std::ostream& operator<< (std::ostream& os, const UserBase& user)
{
	if (user.get_user_type() == UserTypeId::kAdminUser)
		os << std::left << std::setw(MAX_USERNAME) << "\nAdministrator" << std::setw(3) << "|";
	else if (user.get_user_type() == UserTypeId::kPlayerUser)
		os << std::left << std::setw(MAX_USERNAME) << "\nPlayer" << std::setw(3) << "|";

	os << std::setw(MAX_USERNAME) << user.get_username() << std::setw(3) << "|"
		<< std::setw(2 * MAX_USERNAME) << user.get_email() << std::setw(3) << "|";

	double funds = user.get_available_funds();

	if (funds < 0)
		os << std::setw(8) << "N/A";
	else
		os << "\x9C" << std::setw(7) << std::setprecision(2) << std::fixed << funds;

	return os;
}

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
}


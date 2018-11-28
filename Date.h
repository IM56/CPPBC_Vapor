//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

namespace date
{
	std::string secs_to_formatted_time(long secs);

	long formatted_time_to_secs(const std::string & time);

	std::string current_date_and_time();
}

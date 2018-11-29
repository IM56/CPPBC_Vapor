//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

///////////////////////////////////////////////////////////////////////
// Date: A namespace of useful methods dealing with system date/time //
///////////////////////////////////////////////////////////////////////

#pragma once

#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

namespace date
{
	// Convert seconds to HH:MM:SS format
	std::string secs_to_formatted_time(long secs);
	// Convert HH:MM:SS format to seconds
	long formatted_time_to_secs(const std::string & time);
	// Formatted string DD/MM/YYYY HH:MM:SS of current system time
	std::string current_date_and_time();
}

#endif
//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "Date.h"

using namespace date;

std::string date::secs_to_formatted_time(long secs)
{
	int hours = secs / 3600;
	int minutes = (secs % 3600) / 60;
	int seconds = secs % 60;
	std::ostringstream oss;
	oss << std::setfill('0');
	oss << std::setw(2) << hours << ":";
	oss << std::setw(2) << minutes << ":";
	oss << std::setw(2) << seconds;
	return (oss.str());
}

long date::formatted_time_to_secs(const std::string& time)
{
	char c;
	int hours, minutes, seconds;
	long output;
	std::istringstream iss(time);
	iss >> hours >> c >> minutes >> c >> seconds;
	output = hours * 3600 + minutes * 60 + seconds;
	return output;
}

std::string date::current_date_and_time()
{
	int year, month, day, hours, mins, secs;
	time_t now = time(0);
	struct tm t;
	localtime_s(&t, &now);
	year = t.tm_year + 1900;	month = t.tm_mon + 1;	day = t.tm_mday;
	hours = t.tm_hour;	mins = t.tm_min;	secs = t.tm_sec;
	
	std::ostringstream oss;
	oss << std::setfill('0');
	oss << std::setw(2) << day << "/";
	oss << std::setw(2) << month << "/";
	oss << std::setw(4) << year << " ";
	oss << std::setw(2) << hours << ":";
	oss << std::setw(2) << mins << ":";
	oss << std::setw(2) << secs;

	return (oss.str());
}

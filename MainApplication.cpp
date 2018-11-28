//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once

#include "MenuSystem.h"


// Entry point for the system
int main(int argc, char** argv)
{
	// Let the database load all the data.
	DatabaseManager::instance().load_data();
	return MenuSystem::instance().run();
}

//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <iostream>

#include "DatabaseManager.h"



class UserBase;

class MenuSystem
{
public:

	// Singleton instance definition.
	static MenuSystem& instance();

	// run the system 
	int run();

private:
	void list_all_games() const;

	void list_all_users() const;

	int run_login_screen();

	int run_admin_user_menu();

	int run_player_user_menu();

	int run_unknown_user_menu();

	void create_user();

	void remove_user();

	int game_menu();

private:
	UserBase* m_pUser = nullptr;  // the currently logged in user.
	UserFactory m_uFactory;
};

#endif
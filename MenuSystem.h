//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "DatabaseManager.h"

#include <iostream>


class UserBase;

class MenuSystem
{
public:

	// Singleton instance definition.
	static MenuSystem& instance();

	// run the system 
	int run();

private:
	// Formatting output of users and games as a table
	void user_header() const;
	void game_header() const;
	
	void list_all_games() const;
	void list_all_users() const;

	int run_login_screen();
	int run_admin_user_menu();
	int run_player_user_menu();
	int run_unknown_user_menu();
	int game_menu();

	void create_user();
	void remove_user();
	void view_play_log();
	void view_transactions();

	Game* create_game();
	void play_game(PlayerUser* pPlayer);
	void remove_game();
	void delete_game(PlayerUser* pPlayer);


private:
	UserBase* m_pUser = nullptr;  // the currently logged in user.
	UserFactory m_uFactory;
	GameFactory m_gFactory;
};

#endif
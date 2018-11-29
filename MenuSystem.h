//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

////////////////////////////////////////////////////////////////
// MenuSystem: Provides a basic UI for navigating the program //
////////////////////////////////////////////////////////////////

#pragma once
#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "DatabaseManager.h"

#include <iostream>

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
	
	void list_all_games() const; // Outputs all games on the system to a table
	void list_all_users() const; // Outputs all users on the system to a table

	int run_login_screen();        // Prompt user to login in with username and password
	int run_admin_user_menu();     // Interface for admin users
	int run_player_user_menu();    // Interface for player users
	int run_unknown_user_menu();   // Start screen for everyone
	int game_menu();               // Interface to purchase games

	void create_user();          // User creation screen
	void remove_user();          // User removal screen
	void view_play_log();        // Outputs a chosen user's play log
	void view_transactions();    // Outputs a chosen user's transaction history

	Game* create_game();                    // Game creation screen, get pointer to new game
	void play_game(PlayerUser* pPlayer);    // Interface for playing a game from a player's bag
	
	/* Interface for removing a game from the system
	 * If you choose to replace the game, then the removed game is replaced with this one
	 * If you choose to remove without replacement, affected players are reimbursed */
	void remove_game();  

	void delete_game(PlayerUser* pPlayer);  // Interface for player to remove a game


private:
	UserBase* m_pUser = nullptr;  // the currently logged in user.
	UserFactory m_uFactory;
	GameFactory m_gFactory;
};

#endif
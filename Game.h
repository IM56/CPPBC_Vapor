//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>

class Game
{
public:
	using GameId = int;

	Game(const GameId& gameId, const std::string& title, const std::string& desc, double price)
		: m_gameId(gameId)
		, m_title(title)
		, m_description(desc)
		, m_price(price)
	{}

	virtual ~Game() {}

	const Game::GameId get_game_id() const { return m_gameId; }

	const std::string get_title() const { return m_title; }

	const double get_price() const { return m_price; }

	void set_title(const std::string& val) { m_title = val; }

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key).
	std::string m_title; // The game title
	std::string m_description; // Some information about the game.
	const double m_price;
};

#endif
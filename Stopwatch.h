//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

///////////////////////////////////////////////////////////////////////
// Stopwatch: Measures time since its creation to the nearest second //
///////////////////////////////////////////////////////////////////////

#pragma once
#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class Stopwatch
{
	using clock = std::chrono::steady_clock;

public:
	Stopwatch() : m_start(clock::now()) {}

	// Determine how long the stopwatch has been around
	int time_elapsed() const;

private:
	const clock::time_point m_start;
};


#endif
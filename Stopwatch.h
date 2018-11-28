//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547
#pragma once
#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>


// A simple stopwatch that measures time in seconds

class Stopwatch
{
	using clock = std::chrono::steady_clock;

public:
	Stopwatch() : m_start(clock::now()) {}

	int time_elapsed() const;

private:
	const clock::time_point m_start;
};


#endif
//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ismail Movahedi
//Student number: 28039547

#include "Stopwatch.h"

int Stopwatch::time_elapsed() const
{
	std::chrono::duration<int> elapsed = std::chrono::duration_cast<std::chrono::duration<int>>(clock::now() - m_start);
	return elapsed.count();
}


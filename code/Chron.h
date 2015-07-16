/********************************************************************
University of Central Florida
Department of Electric Engineering & Computer Science
Fall 2012

COP 6616 - Multicore Programming
Prof.: Damian Dechev.

Research Project: 
Enhanced Parallel BZIP2 Compression with Lock-free Queue

Students: 
Edward Aymerich.
Jose Sanchez.

Copyright 2012 UCF.
********************************************************************/

#ifndef CHRON_H_INCLUDED
#define CHRON_H_INCLUDED

#include <chrono>

// Functions as a chronometer, measurng wall time elapsed
// between calls to start() and stop().
class Chron {
public:
	void start();
	void stop();
	unsigned int seconds();
	unsigned int miliseconds();
private:
	std::chrono::time_point<std::chrono::system_clock> begin, end;
};

#endif // CHRON_H_INCLUDED

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

#include "Chron.h"

// Begin time measurement.
void Chron::start(){
	begin = std::chrono::system_clock::now();
}

// Stop time measurement.
void Chron::stop(){
	end = std::chrono::system_clock::now();
}

// Returns the amount of seconds measure betweeen
// calls to start() and stop().
unsigned int Chron::seconds(){
	unsigned int sec = std::chrono::duration_cast<std::chrono::seconds>(end-begin).count();
	return sec;
}

// Returns the amount of miliseconds measure betweeen
// calls to start() and stop().
unsigned int Chron::miliseconds(){
	unsigned int msec = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
	return msec;
}

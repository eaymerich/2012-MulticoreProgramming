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

#ifndef TQUEUE_H_INCLUDED
#define TQUEUE_H_INCLUDED

#include "Block.h"

class TQueue {
public:
	TQueue(){}
	virtual ~TQueue(){}
	virtual bool enqueue(Block *b) = 0;
	virtual Block* dequeue() = 0;
protected:

};

#endif // TQUEUE_H_INCLUDED

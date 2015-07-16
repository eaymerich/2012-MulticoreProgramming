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

#ifndef TQUEUELOCKFREE_H_INCLUDED
#define TQUEUELOCKFREE_H_INCLUDED

#include <boost/lockfree/queue.hpp>

#include "TQueue.h"

// Just a wrapper around
// boost::lockfree::queue.
class TQueueLockFree : public TQueue {
public:
	TQueueLockFree(unsigned int p_size = 10);
	virtual ~TQueueLockFree();
	virtual bool enqueue(Block *b);
	virtual Block* dequeue();
protected:
	boost::lockfree::queue<Block*> fifo;
};

#endif // TQUEUELOCKFREE_H_INCLUDED

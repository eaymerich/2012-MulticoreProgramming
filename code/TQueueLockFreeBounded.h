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

#ifndef TQUEUELOCKFREEBOUNDED_H_INCLUDED
#define TQUEUELOCKFREEBOUNDED_H_INCLUDED

#include "NonBlockingQueue.h"
#include "TQueue.h"

// Just a wrapper arround the bounded lock-free queue from:
// http://www.1024cores.net/home/lock-free-algorithms/queues/bounded-mpmc-queue 
class TQueueLockFreeBounded : public TQueue {
public:
	TQueueLockFreeBounded(unsigned int p_size = 10);
	virtual ~TQueueLockFreeBounded();
	virtual bool enqueue(Block *b);
	virtual Block* dequeue();
protected:
	NonBlockingQueue fifo;
};

#endif // TQUEUELOCKFREEBOUNDED_H_INCLUDED

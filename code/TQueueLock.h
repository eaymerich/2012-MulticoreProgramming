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

#ifndef TQUEUELOCK_H_INCLUDED
#define TQUEUELOCK_H_INCLUDED

#include <pthread.h>

#include "TQueue.h"

// A simple implementation of a thread safe
// queue, using one single mutex.
class TQueueLock : public TQueue {
public:
	TQueueLock(unsigned int size_p = 10);
	virtual ~TQueueLock();
	virtual bool enqueue(Block *b);
	virtual Block* dequeue();
protected:
	pthread_mutex_t mutex;
	Block **blocks;
	unsigned int size;
	unsigned int head;
	unsigned int tail;
};

#endif // TQUEUELOCK_H_INCLUDED

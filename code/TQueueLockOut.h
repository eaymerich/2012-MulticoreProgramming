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

#ifndef TQUEUELOCKOUT_H_INCLUDED
#define TQUEUELOCKOUT_H_INCLUDED

#include "TQueueLock.h"

// This queue is a wrapper around TQueueLock,
// but it garantees that blocks are enqueue in order.
class TQueueLockOut : public TQueueLock {
public:
	TQueueLockOut(unsigned int size_p = 10);
	virtual ~TQueueLockOut();
	virtual bool enqueue(Block *b);
protected:
	unsigned int next_block;
};

#endif // TQUEUELOCKOUT_H_INCLUDED

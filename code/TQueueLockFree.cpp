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

#include "TQueueLockFree.h"

TQueueLockFree::TQueueLockFree(unsigned int p_size) : fifo(p_size) {

}

TQueueLockFree::~TQueueLockFree(){

}

// Just a wrapper around push() from
// boost::lockfree::queue.
bool TQueueLockFree::enqueue(Block *b){
	return fifo.push(b);
}

// Just a wrapper around pop() from
// boost::lockfree::queue.
Block* TQueueLockFree::dequeue() {
	Block* b = NULL;
	if(!fifo.pop(b)){
		return NULL;
	}
	return b;
}

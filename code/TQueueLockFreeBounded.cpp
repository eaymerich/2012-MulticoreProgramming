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

#include "TQueueLockFreeBounded.h"

TQueueLockFreeBounded::TQueueLockFreeBounded(unsigned int p_size) :
	fifo(p_size) {

}

TQueueLockFreeBounded::~TQueueLockFreeBounded(){

}

// A wrapper around add() function from
// NonBlockingQueue.
bool TQueueLockFreeBounded::enqueue(Block *b){
	return fifo.add(b);
}

// A wrapper around remove() function from
// NonBlockingQueue.
Block* TQueueLockFreeBounded::dequeue(){
	Block* b;
	if(fifo.remove(b)){
		return b;
	}
	return NULL;
}

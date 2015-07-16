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

#include "TQueueLockOut.h"

TQueueLockOut::TQueueLockOut(unsigned int size_p) : TQueueLock(size_p), next_block(0) {

}

TQueueLockOut::~TQueueLockOut() {

}

// Only enqueues the block if it is the next block 
// in the sequence number, ensuring that blocks
// in the queue are ordered.
bool TQueueLockOut::enqueue(Block *b) {
	bool enq = false;
	
	// If the block to enqueue is the next is line
	if(b->sequence == next_block){
		// Enqueue the block.
		enq = TQueueLock::enqueue(b);
		if(enq){
			next_block++;
		}
	}
	
	return enq;
}

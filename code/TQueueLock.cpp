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

#include <iostream>
using namespace std;

#include "TQueueLock.h"

TQueueLock::TQueueLock(unsigned int size_p) : size(size_p){
	pthread_mutex_init(&mutex, NULL);
	head = 0;
	tail = 0;
	blocks = new Block*[size];
}

TQueueLock::~TQueueLock(){
	delete [] blocks;
}

bool TQueueLock::enqueue(Block *b){

	bool r = false;
	
	// Get the lock to modify the queue.
	pthread_mutex_lock(&mutex);

	// Try to enqueue the element.
	if(tail-head < size){
		// Enqueue the element.
		blocks[tail%size] = b;
		tail++;
		r = true;
	}

	// Free the lock
	pthread_mutex_unlock(&mutex);

	return r;
}

Block* TQueueLock::dequeue(){
	
	Block *b = NULL;
	
	// Get the lock to modify the queue.
	pthread_mutex_lock(&mutex);
	
	// Try to dequeue the element.
	if(head < tail){
		// Dequeue the element.
		b = blocks[head%size];
	    head++;
	}
	
	// Free the lock.
	pthread_mutex_unlock(&mutex);
	
	return b;
}

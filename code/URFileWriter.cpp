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

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#include <time.h>
#include <sched.h>

#include "URFileWriter.h"

#define MIN_SLEEP 1000
#define MAX_SLEEP 100000
#define DELTA_SLEEP 10000

URFileWriter::URFileWriter(TQueue *q, int hFile_p) : FileWriter(q,hFile_p), sequence(0) {

}

URFileWriter::~URFileWriter(){

}

void URFileWriter::doWork(){
	Block *b = NULL;
	unsigned int wait_time = MIN_SLEEP;
	bool keep_looking = false;

	while(true){

		// Looks for next block in buffer
		map<int,Block*>::iterator it = write_buffer.find(sequence);
		if(it == write_buffer.end() ){
			//	Next block is not in the buffer. Search in queue.
			keep_looking = true;
			while(keep_looking){
				b = queue->dequeue();
				if(!b){ // If the dequeues fails
					wait_count++;
					wait_time = MIN_SLEEP;
					
					// Keep trying to dequeue.
					while(!b){
						// If there is no more work, then finish.
						if(!goon) return;
						
						// Sleeps for some time, so other
						// threads can do their work,
						usleep(wait_time);
						if(wait_time < MAX_SLEEP){
							wait_time *= 2;
						}
						
						// Try to dequeue again.
						b = queue->dequeue();
					}
				}

				if(b->sequence != sequence){
					// Not the block we need. Add this block to the buffer.
					write_buffer[b->sequence] = b;
				}else{
					// This is the block we need. No more search.
					keep_looking = false;
				}
			}
		}else{
			// Block found in the buffer. Get the block and delete from buffer.
			b = it->second;
			write_buffer.erase(it);
		}

		// Saves block to file
		int ret = do_write(hFile, b->data, b->size);
		sequence++;
		if(ret < 0){
			cerr << "Write fail..." << endl;
			exit(-1);
		}

		// Frees block memory.
		delete b;
		b = NULL;
	}
}

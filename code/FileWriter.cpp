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

#include "FileWriter.h"

#define MIN_SLEEP 1000
#define MAX_SLEEP 100000
#define DELTA_SLEEP 10000

FileWriter::FileWriter(TQueue *q, int hFile_p) : hFile(hFile_p), queue(q) {
	wait_count = 0;
}

FileWriter::~FileWriter() {

}

ssize_t FileWriter::do_write(int fd, const void *buf, size_t count){
	ssize_t bytesRemaining = count;
	ssize_t nbytes = 0;
	const char *pbuf = (const char *)buf;
	while ((bytesRemaining > 0) && ((nbytes = write(fd, pbuf, bytesRemaining)) > 0)){
		bytesRemaining -= nbytes;
		pbuf += nbytes;
	}

	if (nbytes < 0){
		cout << "Error writing output file";
		return nbytes;
	}

	return (count - bytesRemaining);
}

void FileWriter::doWork() {

	Block *b = NULL;
	unsigned int wait_time = MIN_SLEEP;

	while(true){

		// Take a block from queue
		b = queue->dequeue();
		if(!b){
			wait_count++;
			wait_time = MIN_SLEEP;
			while(!b){
				// If there is no more work, then finish.
				if(!goon) return;
				
				// Sleep for some time.
				usleep(wait_time);
				if(wait_time < MAX_SLEEP){
					wait_time *= 2;
				}
				
				// Try to dequeue again.
				b = queue->dequeue();
			}
		}
		
		// Saves block to file
		int ret = do_write(hFile, b->data, b->size);
		if(ret < 0){
			cerr << "Write fail..." << endl;
			exit(-1);
		}

		// Discarts block
		delete b;
		b = NULL;
	}
}

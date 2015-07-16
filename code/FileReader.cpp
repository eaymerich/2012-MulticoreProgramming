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

#include "FileReader.h"

#define MIN_SLEEP 1000
#define MAX_SLEEP 100000
#define DELTA_SLEEP 10000

FileReader::FileReader(TQueue *q, int hFile_p, int block_size_p) : sequence(0), queue(q), hFile(hFile_p), block_size(block_size_p) {
	wait_count = 0;
}

FileReader::~FileReader() {

}

ssize_t FileReader::do_read(int fd, void *buf, size_t count){
	ssize_t bytesRemaining = count;
	ssize_t nbytes = 0;
	char *pbuf = (char *)buf;
	while ((bytesRemaining > 0) && (nbytes = read(fd, pbuf, bytesRemaining)) > 0)	{
		bytesRemaining -= nbytes;
		pbuf += nbytes;
	}

	if (nbytes < 0)	{
		cout << "Error reading input file";
		return nbytes;
	}

	return (count - bytesRemaining);
}

void FileReader::doWork() {
	
	unsigned int wait_time = MIN_SLEEP;
	
	while(goon){
		// Creates a new block.
		Block *b = new Block(block_size, sequence);
		sequence++;
		
		// Try to read a block of data from file.
		int ret = do_read(hFile, (char *) b->data, b->size);
		if (ret == 0){ // finish
			delete b;
			close(hFile);
			break;
		}
		else if (ret < 0){
			close(hFile);
			cout << "ERROR: Could not read from file!  Aborting...\n";
			exit(-1);
		}
		if ((size_t)ret != b->size)
			b->size = ret;

		// Puts the block in the queue.
		if(!queue->enqueue(b)){
			
			// If the enqueue fails, wait some time.
			wait_count++;
			wait_time = MIN_SLEEP;
			while(!queue->enqueue(b)){
				usleep(wait_time);
				if(wait_time < MAX_SLEEP){
					wait_time *= 2;
				}
			}
		}
	}
}

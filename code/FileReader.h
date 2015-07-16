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

#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

#include "Worker.h"

#include "TQueue.h"

// Implements a thread that reads blocks of data
// from an input file and put those blocks in a queue.
// When there are no more blocks to read, the thread
// finish.
class FileReader : public Worker {
public:
	FileReader(TQueue *q, int hFile_p, int block_size_p);
	virtual ~FileReader();
	unsigned int sequence;
	int wait_count;
protected:
	ssize_t do_read(int fd, void *buf, size_t count);
	virtual void doWork();
	TQueue *queue;
	int hFile;
	unsigned int block_size;
};

#endif // FILEREADER_H_INCLUDED

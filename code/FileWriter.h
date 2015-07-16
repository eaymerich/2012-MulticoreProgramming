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

#ifndef FILEWRITER_H_INCLUDED
#define FILEWRITER_H_INCLUDED

#include "Worker.h"
#include "TQueue.h"

// Implements a thread that writes blocks from
// the out_queue to some file.
// This class is really simple, only takes the blocks
// from the queue and writes them to file, in the same
// order that the blocks are stored in the queue.
class FileWriter : public Worker {
public:
	FileWriter(TQueue *q, int hFile_p);
	virtual ~FileWriter();
	int wait_count;
protected:
	virtual ssize_t do_write(int fd, const void *buf, size_t count);
	virtual void doWork();
	int hFile;
	TQueue *queue;
};

#endif // FILEWRITER_H_INCLUDED

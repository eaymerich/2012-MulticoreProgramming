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

#ifndef COMPRESSOR_H_INCLUDED
#define COMPRESSOR_H_INCLUDED

#include "Worker.h"
#include "TQueue.h"

class TPCompressor : public Worker {
public:
	TPCompressor(TQueue *out, TQueue *in, int BWTblockSize_p);
	virtual ~TPCompressor();
	int wait_in_block;
	int wait_out_block;
protected:
	virtual void doWork();
	int BWTblockSize;
	TQueue *out_queue;
	TQueue *in_queue;
};

#endif // COMPRESSOR_H_INCLUDED

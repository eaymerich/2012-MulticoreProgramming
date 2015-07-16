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

#ifndef URFILEWRITER_H_INCLUDED
#define URFILEWRITER_H_INCLUDED

#include <map>

#include "FileWriter.h"

// Implements a thread that writes blocks from
// the out_queue to some file.
// This class writes blocks in the correct order,
// even if blocks are unordered.
class URFileWriter : public FileWriter {
public:
	URFileWriter(TQueue *q, int hFile_p);
	virtual ~URFileWriter();
protected:
	virtual void doWork();
	unsigned int sequence;
	map<int, Block*> write_buffer;
};

#endif // URFILEWRITER_H_INCLUDED

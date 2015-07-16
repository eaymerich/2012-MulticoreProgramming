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


#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

// A block of data, used to pass information from
// the input file to threads to output file.
class Block {
public:
	Block(int size_p, int sequence_p=0) : size(size_p), sequence(sequence_p) { data = new char[size]; }
	virtual ~Block(){ delete [] data; }

	unsigned int size;
	unsigned int sequence;
	char *data;
};

#endif // BLOCK_H_INCLUDED

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

#include <bzlib.h>

#include "time.h"

#include "TPCompressor.h"


TPCompressor::TPCompressor(TQueue *out, TQueue *in, int BWTblockSize_p=9) : BWTblockSize(BWTblockSize_p), out_queue(out) , in_queue(in) {
	wait_in_block = 0;
	wait_out_block = 0;
}

TPCompressor::~TPCompressor(){

}

void TPCompressor::doWork(){
	//cout << "doWork del TPCompressor" << endl;
	while(true){
	
		// Take a block from in queue.
		Block *b_in = in_queue->dequeue();
		if(!b_in) wait_in_block++;
		while(!b_in){
			// If there is no more work, then finish. 
			if(!goon) return;
			//usleep(10000);
			b_in = in_queue->dequeue();
		}
		
		// Compress the block into a new block.
		unsigned int outSize = (unsigned int) (((b_in->size)*1.01)+600);
		Block *b_out = new Block(outSize, b_in->sequence);
		int ret = BZ2_bzBuffToBuffCompress(b_out->data, &(b_out->size), b_in->data, b_in->size, BWTblockSize, 0, 30);
		if (ret != BZ_OK){
	        cerr << "Error Compressing" << endl;
	        exit(-1);
    	}
	
		// Free memory from uncompressed block.
    	delete b_in;
    	b_in = NULL;

		// Put the compressed block in out queue
		if(!out_queue->enqueue(b_out)){
			wait_out_block++;
			while( !out_queue->enqueue(b_out) ) {
				//usleep(10000);
			}
		}
	}
}

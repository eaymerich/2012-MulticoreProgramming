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
#include <time.h>
using namespace std;

#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <string>

#include "TQueue.h"
#include "TQueueLock.h"
#include "TQueueLockOut.h"
#include "TQueueLockFree.h"
#include "TQueueLockFreeBounded.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "URFileWriter.h"
#include "TPCompressor.h"

#include "Chron.h"

#define	FILE_MODE	(S_IRUSR | S_IWUSR )
#define O_BINARY 0

#define DEFAULT_BWTBLOCKSIZE 3
#define MAX_BWTBLOCKSIZE 9

#define QUEUE_SIZE 128

#define QUEUE_TYPE_LOCK 'l'
#define QUEUE_TYPE_LOCK_FREE 'f'
#define QUEUE_TYPE_LOCK_FREE_BOUNDED 'b'

int safe_open_output(const char *path){
	int ret = open(path, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, FILE_MODE);
	if (ret == -1){
		// Maybe the file already exist. Try deleting file.
		remove(path);
		ret = open(path, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, FILE_MODE);
		if (ret == -1){
			cout << "Error opening output file" << endl;
		}
	}
	return ret;
}

int safe_open_input(const char *path){
    int ret;
    ret = open(path, O_RDONLY | O_BINARY);
	if (ret == -1){
		cout << "Error opening input file" << endl;
	}
	return ret;
}

int main(int argc, char *argv[]){
	
	// Start to read time as soon as execution begins.
	Chron crono;
	crono.start();

	unsigned int blockSize;
	unsigned int BWTblockSize;
	int hInfile;
	int hOutfile;
	char *inFilename = NULL;  // example value
	
	unsigned int workers = 1;
	BWTblockSize=DEFAULT_BWTBLOCKSIZE;
	char queue_type = QUEUE_TYPE_LOCK;
	int tru_block_size = -1;
	
	// Argument parsing.
	switch (argc){
	  case 6: //
		  tru_block_size = atoi(argv[5]);
	  case 5: // Type of queue
		  queue_type = argv[4][0];
		  if(queue_type != QUEUE_TYPE_LOCK && queue_type != QUEUE_TYPE_LOCK_FREE
			  && queue_type != QUEUE_TYPE_LOCK_FREE_BOUNDED){
		  	cout << "Invalid queue type" << endl;
		  	exit(1);
		  }
      case 4: // Amount of threads
          workers=atoi(argv[3]);
          if ((workers<=0) || (workers>1024)){
            cout <<"Workers should be between 1 and 1024" << endl;
            exit(1);
          }
	  case 3: // BlockSize given.
	     BWTblockSize=atoi(argv[2]);
          if ((BWTblockSize<=0) || (BWTblockSize>MAX_BWTBLOCKSIZE)){
            cout <<"Block size should be between 1 and " << MAX_BWTBLOCKSIZE << endl;
            exit(1);
          }
	  case 2: // Input file given.
          inFilename=argv[1];
          break;
      default:
            cout <<"Parameters: InputFileName [BlockSize] [workers] [l|f|b] [-1|n]" << endl;
            exit(1);
  	    break;
    }

	if(tru_block_size == -1){
		blockSize = 100*BWTblockSize*1000;
	}else{
		blockSize = tru_block_size;
	}
	
	// Print configuration arguments.
	//cout << "tpbzip2 " << blockSize << " " << workers << " " << queue_type << " ";
	printf("tpbzip2 %u %u %c ", blockSize, workers, queue_type);
	//blockSize=100*BWTblockSize*1024;
	//blockSize=1024;
	
	// Open input and output files.
	hInfile=safe_open_input(inFilename);
	hOutfile = safe_open_output((new string(inFilename))->append(".bz2").c_str());

	/*** BEGIN ***/

	TQueue *read_queue = NULL;
	TQueue *write_queue = NULL;
	
	// Create the right type of queue.
	if (queue_type == QUEUE_TYPE_LOCK){
		read_queue =  new TQueueLock(QUEUE_SIZE);
		write_queue = new TQueueLock(QUEUE_SIZE);
	}else
	if (queue_type == QUEUE_TYPE_LOCK_FREE) {
		read_queue =  new TQueueLockFree(QUEUE_SIZE);
		write_queue = new TQueueLockFree(QUEUE_SIZE);
	}else
	if (queue_type == QUEUE_TYPE_LOCK_FREE_BOUNDED) {
		read_queue =  new TQueueLockFreeBounded(QUEUE_SIZE);
		write_queue = new TQueueLockFreeBounded(QUEUE_SIZE);
	}
	
	// Create reader and writer threads.
	FileReader fr(read_queue, hInfile, blockSize);
	URFileWriter fw(write_queue, hOutfile);

	// Create compressing threads.
	TPCompressor **c;
	c = (TPCompressor **) new TPCompressor*[workers];
	for(unsigned int i = 0; i < workers; i++){
		c[i] = (TPCompressor*) new TPCompressor(write_queue, read_queue,BWTblockSize);
	}

	// Begin reading from file.
	fr.start();
	
	// Begin compressing threads.
	for(unsigned int i = 0; i < workers; i++){
		c[i]->start();
	}
	
	// Begin writing to output file.
	fw.start();

	// Wait for reader to finish reading the input file.
	fr.join();
	
	// Send a signal to compressors that there is no
	// more work to do.
	for(unsigned int i = 0; i < workers; i++){
		c[i]->stop();
	}
	
	// Waits for compressors to finish.
	for(unsigned int i = 0; i < workers; i++){
		c[i]->join();
	}
	
	// Send a signal to writer that there is no
	// more work to do.
	fw.stop();
	
	// Waits for the writer.
	fw.join();
	
	// Finish compressing. Show statistics.
	// Used for debugging purposes.
	/*
	cout << "*** STATISTICS ***" << endl;
	int total_in_waits = 0;
	int total_out_waits = 0;
	for(unsigned int i = 0; i < workers; i++){
		total_in_waits += c[i]->wait_in_block;
		total_out_waits += c[i]->wait_out_block;
	}

	cout << "-Reader" << endl;
	cout << "Blocks readed: " << fr.sequence << endl;
	cout << "Total times waiting for enqueue a block: " << fr.wait_count << endl;
	cout << endl;

	cout << "-Compressors" << endl;
	cout << "Total times waiting for dequeue IN block: " << total_in_waits << endl;
	cout << "Total times waiting for enqueue OUT block: " << total_out_waits << endl;
	cout << endl;

	cout << "-Writer" << endl;
	cout << "Total times waiting for dequeue a block: " << fw.wait_count << endl;
	cout << endl;
	*/
	
	// Freeing memory
	for(unsigned int i = 0; i < workers; i++){
		delete c[i];
	}
	delete c;
	delete write_queue;
	delete read_queue;

	// Stops time counting.
	crono.stop();

	// Shows elapsed time.
	printf("Elapsed-time: %u\n", crono.miliseconds() );

	return 0;
}

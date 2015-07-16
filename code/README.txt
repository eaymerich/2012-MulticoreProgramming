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

******************
*** Requisites ***
This project uses a lock-free queue from the
Boost library development trunk. The files are already included
with the project, but it is necessary to have a standard Boost 
instalation before compiling.

The project also uses some C++11 features, so a compiler with
support for at least C++0x is required. This project was tested
using GCC 4.6.3.


**********************
*** How to compile ***
Just type 'make' in the project root directory.


******************
*** How to use ***
This project creates a command line tool called 'tpbzip2'
that implements a parallel version of the BZIP2 algorithm.

To use tpbzip2, four arguments must be suplied in the correct order:

	./tpbzip2 file block_size l|f|b #_of_threads 

	file
		A path to the file to be compressed. By default, the
		output file will have the same name plus the extension
		'.bz2'. If output file already exist, it will be 
		overwritten. The input file will be preserved.
	
	block_size
		The block size to use, in multiples of 100k. Valid
		values are from 1 to 9.
		
	l|f|b
		Type of queue to use in the program. Valid values are:
		l - Lock base queue.
		f - Lock free unbounded queue (Boost::lockfree::queue).
		b - Lock free bounded queue (from www.1024cores.com).
	
	#_of_threads
		# of threads to do the compression.

Examples:
	./tpbzip2 foo.txt 9 l 16
	This compress the file called 'foo.txt' using blocks
	of 900k size, with lock based queues and using 16
	compression threads.
	
	./tpbzip2 foo.txt 1 b 8
	This compress the file called 'foo.txt' using blocks
	of 100k size, with unbounded lock free based queues and 
	using 8	compression threads.

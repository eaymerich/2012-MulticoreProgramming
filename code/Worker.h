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

#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <iostream>
using namespace std;

#include <pthread.h>

// Implements a thread, with an interface similar to
// Java threads. This class uses pthreads internally.
class Worker {
public:
	Worker(): goon(true) {}
	virtual ~Worker(){}
	void start(){
		pthread_create(&t, NULL, Worker::run, this);
	}
	void join(){
		pthread_join(t, NULL);
	}
	virtual void stop(){ goon = false; }
	static void * run(void *obj){
		//cout << "Worker iniciado" << endl;
		reinterpret_cast<Worker *>(obj)->doWork();
		return NULL;
	}
protected:
	virtual void doWork() = 0;
	bool goon;
	pthread_t t;
};

#endif // WORKER_H_INCLUDED

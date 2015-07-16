CC=g++
#CFLAGS=-c -O0 -g -Wall -I./include -std=c++0x
CFLAGS=-c -O3 -Wall -I./include -std=c++0x
LDFLAGS=-Wall -lbz2 -pthread -std=c++0x
EXEC=tpbzip2
OBJS=FileReader.o FileWriter.o URFileWriter.o Worker.o TPCompressor.o TQueueLock.o TQueueLockFree.o Chron.o NonBlockingQueue.o TQueueLockFreeBounded.o main.o

all: $(EXEC)

Debug: all

clean:
	rm -f *.o $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

.cpp.o:
	$(CC) $< $(CFLAGS)

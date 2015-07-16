#include "stddef.h"
#include <atomic>

#ifndef NONBLOCKINGQUEUE_H
#define NONBLOCKINGQUEUE_H

#include "Block.h"

//typedef long data_type;
typedef Block* data_type;

#define dummy_val 0

class NonBlockingQueue{

public:
NonBlockingQueue(size_t buffer_size);

~NonBlockingQueue();

bool add(data_type const& data);

bool remove(data_type & data);



protected:

private:
    struct cell_t{
        std::atomic<size_t>     sequence_;
        data_type                data_;
    };

    static size_t const         cacheline_size = 64;
    typedef char                cacheline_pad_t [cacheline_size];

    cacheline_pad_t             pad0_;
    cell_t* const               buffer_;
    size_t const                buffer_mask_;
    cacheline_pad_t             pad1_;
    std::atomic<size_t>         enqueue_pos_;
    cacheline_pad_t             pad2_;
    std::atomic<size_t>         dequeue_pos_;
    cacheline_pad_t             pad3_;
};

#endif // NONBLOCKINGQUEUE_H

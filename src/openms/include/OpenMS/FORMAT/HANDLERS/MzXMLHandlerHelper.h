#ifndef OPENMS_FORMAT_HANDLER_MZXMLHANDLERHELPER_H
#define OPENMS_FORMAT_HANDLER_MZXMLHANDLERHELPER_H

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_types.hpp>
#include <boost/thread.hpp>
#include <omp.h>

class Semaphore {
public:
    Semaphore(long init_c) {
        count = init_c;
    }

    void wait()
    {
        boost::unique_lock<boost::mutex> lock(mutex);
        while (count == 0) {
            cond.wait(lock);
        }
        --count;
    }

    void signal()
    {
        {
            boost::lock_guard<boost::mutex> lock(mutex);
            ++count;
        }
        cond.notify_one();
    }

private:
    long count;
    boost::mutex mutex;
    boost::condition_variable cond;
};

template<typename DataType>
class SafeQueue {
public:
    SafeQueue(int max_size) :
            sem_push(max_size), sem_pop(0)
    {
        queue = new DataType[max_size];
        start = end = 0;
        this->max_size = max_size;
    }

    void push(DataType &d) {
        sem_push.wait();
        lock_end.lock();
        queue[end++] = d;
        if (end == max_size) end = 0;
        lock_end.unlock();
        sem_pop.signal();
    }

    void pop(DataType &d) {
        sem_pop.wait();
        lock_start.lock();
        d = queue[start++];
        if (start == max_size) start = 0;
        lock_start.unlock();
        sem_push.signal();
    }

private:
    DataType* queue;
    int start, end, max_size;
    boost::mutex lock_start, lock_end;
    Semaphore sem_push, sem_pop;
};

#endif //OPENMS_FORMAT_HANDLER_MZXMLHANDLERHELPER_H

#ifndef INC_2_PRODUCER_H
#define INC_2_PRODUCER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <atomic>

using namespace std;

template <typename T>
class Producer {

private:
   
    atomic <int> thread_counter;
    atomic <bool> is_publishing;
    mutex fmutex;
    vector <shared_ptr <queue <T>>> fqueue;
    condition_variable cv;

public:
    
    Producer(int number) {
        thread_counter = -1;
        is_publishing = true;

        fqueue.reserve(number);
        for (int i = 0; i < number; i++) {
            fqueue.push_back(make_shared <queue <T>>(queue <T>()));
        }
    }

    Producer(const Producer <T>& cp) = delete;

    void publish(const T& v) {
        for (const shared_ptr <queue <T>>& q : fqueue) {
            unique_lock <mutex> lock(fmutex);
            q->push(v);
            cv.notify_all();
        }
    }

    void stopPublishing() {
        for (const shared_ptr <queue <T>>& q : fqueue) {
            fmutex.lock();
            bool empty = q->empty();
            fmutex.unlock();
            
            while (!empty) {
                cv.notify_one();
                fmutex.lock();
                empty = q->empty();
                fmutex.unlock();
            }
        }
        
        is_publishing = false;
        cv.notify_all();
    }

    void subscribe(function < void(T & v) > func) {
        
        int cur_idx = ++thread_counter;
        while (is_publishing) {
            
            unique_lock <mutex> lock(fmutex);
            while (fqueue[cur_idx]->empty() && is_publishing) {
                cv.wait(lock);
            }
            
            if (!is_publishing || fqueue[cur_idx]->empty()) {
                return;
            }
            
            T value = fqueue[cur_idx]->front();
            fqueue[cur_idx]->pop();
            func(value);
        }

    }
};

#endif
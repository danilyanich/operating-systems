//
// Created by illfate on 10/3/20.
//

#ifndef INC_2_CONSUMER_PRODUCER_H
#define INC_2_CONSUMER_PRODUCER_H

#include <queue>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>
#include <functional>

template<typename T>
class ConsumersProducer {
private:
    std::atomic<int> thread_counter{-1};
    std::atomic<int> thread_end_counter{-1};
    std::atomic<bool> is_publishing{true};
    std::mutex mx;
    std::vector<std::shared_ptr<std::queue<T>>>
            queues;
    std::vector<bool> working_queues;
    std::condition_variable cv;
public:
    explicit ConsumersProducer(int num_consumers) {
        queues.reserve(num_consumers);
        working_queues.reserve(num_consumers);
        for (int i = 0; i < num_consumers; ++i) {
            queues.push_back(std::make_shared<std::queue<T>>(std::queue<T>()));
            working_queues.push_back(true);
        }
    }

    ConsumersProducer(const ConsumersProducer<T>& cp) = delete;

    void Publish(const T &v) {
        for (const auto &q:queues) {
            std::unique_lock<std::mutex> lock(mx);
            q->push(v);
            cv.notify_all();
        }
    }

    void StopPublishing() {
        for (const auto &q:queues) {
            mx.lock();
            bool empty = q->empty();
            mx.unlock();
            while (!empty) {
                cv.notify_one();
                mx.lock();
                empty = q->empty();
                mx.unlock();
            }
        }
        is_publishing = false;
        cv.notify_all();
    }

    void Subscribe(std::function<void(T &v)> func) {
        int cur_idx = ++thread_counter;
        while (is_publishing) {
            std::unique_lock<std::mutex> lock(mx);
            while (queues[cur_idx]->empty() && is_publishing) {
                cv.wait(lock);
            }
            if (!is_publishing || queues[cur_idx]->empty()) {
                return;
            }
            T value = queues[cur_idx]->front();
            queues[cur_idx]->pop();
            func(value);
        }
    }
};

#endif //INC_2_CONSUMER_PRODUCER_H

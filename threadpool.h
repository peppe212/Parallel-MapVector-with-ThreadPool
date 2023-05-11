//
// Created by Giuseppe Muschetta.
//

#ifndef THREAD_POOL_ULTIMATE_THREADPOOL_H
#define THREAD_POOL_ULTIMATE_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <iostream>

using namespace std;

class threadpool {
private:
    // Worker threads
    vector<thread> workers;
    // Task queue
    queue<function<void()>> tasks;

    // Synchronization
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
public:
    // Constructor
    explicit threadpool(size_t);

    // Destructor
    ~threadpool();

    // Add a new task to the queue
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args)
    -> future<invoke_result_t<F, Args...>>;
};

// Constructor
threadpool::threadpool(size_t threads): stop(false) {
    for(size_t i = 0; i < threads; ++i)
        workers.emplace_back(
                [this] {
                    for(;;) {
                        function<void()> task;
                        {
                            unique_lock<mutex> lock(queue_mutex);
                            condition.wait(lock,
                                           [this] { return stop || !tasks.empty(); });
                            if(stop && tasks.empty())
                                return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                }
        );
}

// Destructor
threadpool::~threadpool() {
    {
        unique_lock<mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(thread &worker: workers)
        worker.join();
}


// Add a new task to the queue
template<class F, class... Args>
auto threadpool::submit(F&& f, Args&&... args) -> future<invoke_result_t<F, Args...>> {
    using return_type = invoke_result_t<F, Args...>;

    auto task = make_shared<packaged_task<return_type()> >(
            [f=std::forward<F>(f), ...params=std::forward<Args>(args)]() { return invoke(f, params...); }
    );

    future<return_type> res = task->get_future();
    {
        unique_lock<mutex> lock(queue_mutex);
        if(stop)
            throw runtime_error("submit on stopped ThreadPool");
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}


#endif //THREAD_POOL_ULTIMATE_THREADPOOL_H

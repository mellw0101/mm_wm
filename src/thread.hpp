/* #include <future>
#include <mutex>
#include <queue> */
#ifndef THREAD__HPP
#define THREAD__HPP

#include <thread>
#include <type_traits>
#include <vector>
using namespace std;
#include <atomic>
#include <thread>
#include <functional>


#include <atomic>
#include <thread>
#include <functional>

#include <atomic>
#include <thread>
#include <functional>
#include <exception>
// #include <optional>

using namespace std;

class Thread
{
    public:
        template<typename Callable, typename... Args>
        Thread(Callable&& func, Args&&... args)
        : active(true), paused(false), worker([this, func = std::forward<Callable>(func), ...args = std::forward<Args>(args)]()
        {
            try
            {
                while (active)
                {
                    if (!paused)
                    {
                        func(forward<Args>(args)...);    
                    }

                    // Yield to avoid busy waiting
                    this_thread::yield();
                }
            }
            catch (...)
            {
                // Capture any exception
                lastException = std::current_exception();

                // Optionally, notify about the exception here
            }
        })
        {}

        ~Thread()
        {
            // Ensure the thread is signaled to stop
            stop();
        }

        // Modified operations for thread safety and error handling
        Thread(const Thread&) = delete;
        Thread& operator=(const Thread&) = delete;
        Thread(Thread&&) = delete;
        Thread& operator=(Thread&&) = delete;

        void pause()
        {
            paused = true;
        }
        
        void resume()
        {
            paused = false;
        }
        
        void stop()
        {
            if (active)
            {
                active = false;
                if (worker.joinable())
                {
                    worker.join();
                }
            }
        }

        bool isPaused() const
        {
            return paused;
        }

        bool isActive() const
        {
            return active;
        }

        thread::id getID() const
        {
            return worker.get_id();
        }

        // New Functionality: Error handling
        bool hasExceptionOccurred() const
        {
            return static_cast<bool>(lastException);
        }
        
        void rethrowException() const
        {
            if (lastException)
            {
                rethrow_exception(lastException);
            }
        }

    private:
        atomic<bool> active;
        atomic<bool> paused;
        thread worker;
        exception_ptr lastException; // Holds the last exception thrown by the thread

};

template<typename... ParamTypes>
class iter_thread_t
{
    private:
        thread worker_;
        atomic<bool> active_;
        
        // The interval between task executions, in milliseconds
        unsigned interval_;
        
        // The task to be performed, adjusted as needed
        function<void()> task_;
        
        // Parameters to be used with the task
        tuple<ParamTypes...> params_;

        void loop()
        {
            using namespace std::chrono;
            auto next_run_time = steady_clock::now() + milliseconds(interval_);

            while (active_.load())
            {
                auto now = steady_clock::now();
                if (now >= next_run_time)
                {
                    apply(task_, params_); // Use std::apply to pass tuple elements as arguments
                    next_run_time = now + milliseconds(interval_); // Schedule the next run
                }
                else
                {
                    this_thread::sleep_for(milliseconds(1)); // Sleep for a short while to prevent busy waiting
                }
            }
        }

    public:
        // Constructor: Takes the interval in milliseconds, the task to perform, and parameters for the task
        iter_thread_t(unsigned interval, std::function<void(ParamTypes...)> task, ParamTypes... params)
        : interval_(interval), task_(move(task)), active_(true), params_(make_tuple(forward<ParamTypes>(params)...))
        {
            worker_ = std::thread([this]() { this->loop(); });
        }

        ~iter_thread_t()
        {
            active_.store(false);
            if (worker_.joinable())
            {
                worker_.join();
            }
        }

        iter_thread_t(const iter_thread_t&) = delete;
        iter_thread_t& operator=(const iter_thread_t&) = delete;
        iter_thread_t(iter_thread_t&&) = delete;
        iter_thread_t& operator=(iter_thread_t&&) = delete;
};

/* #include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool {
    public:
        ThreadPool(size_t threads) : stop(false) {
            for(size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    while(true) {
                        std::function<void()> task;
                        {
                            std::unique_copy<std::mutex> lock(this->queueMutex);
                            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                            if(this->stop && this->tasks.empty())
                                return;
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) 
            -> std::future<typename std::result_of<F(Args...)>::type> {
            using return_type = typename std::result_of<F(Args...)>::type;
            auto task = std::make_shared< std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );   
            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if(stop)
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                tasks.emplace([task](){ (*task)(); });
            }
            condition.notify_one();
            return res;
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                stop = true;
            }
            condition.notify_all();
            for(std::thread &worker: workers)
                worker.join();
        }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
}; */

// For uint32_t
#include <cstdint>
#include <unordered_map>

// #include <optional>

template<typename ValueType>
class UInt32UnorderedMap
{
    unordered_map<uint32_t, ValueType> map_;
    
public:
    // Add or update a value associated with a uint32_t key
    void insert(uint32_t key, const ValueType& value)
    {
        map_[key] = value;
    }

    // Retrieve a value by its uint32_t key. Returns std::nullopt if the key does not exist
    /* std::optional<ValueType> get(uint32_t key) const {
        auto it = map_.find(key);
        if (it != map_.end()) {
            return it->second;
        }
        return std::nullopt; // Key not found
    } */

    ValueType get(uint32_t __w) {
        auto it = map_.find(__w);
        if (it == map_.end()) {
            return it->second;
        }

        if constexpr (is_pointer<ValueType>()) {
            return nullptr;
        }
        else {
            return ValueType{};
        }
    }

    // Check if a key exists in the map
    bool contains(uint32_t key) const {
        return map_.find(key) != map_.end();
    }

    // Remove a key-value pair by its key
    void remove(uint32_t key) {
        map_.erase(key);
    }

    // Optional: Define more functions as needed for your application

};

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#define enqueueT(__Name, __Pool, ...) \
    std::future<void> __Name = __Pool.enqueue(__VA_ARGS__)

template<typename ThreadPoolType, typename Func, typename... Args>
auto enqueueTask(ThreadPoolType& pool, Func&& func, Args&&... args) -> future<decltype(func(args...))>
{
    return pool.enqueue(forward<Func>(func), forward<Args>(args)...);
}

class ThreadPool
{
    public:
        ThreadPool(size_t threads)
        : stop(false)
        {
            for (size_t i = 0; i < threads; ++i)
            {
                workers.emplace_back([this]
                {
                    while (true)
                    {
                        function<void()> task;
                        {
                            unique_lock<mutex> lock(this->queueMutex);
                            this->condition.wait(lock, [this]
                            {
                                return this->stop || !this->tasks.empty();
                            });
                            
                            if (this->stop && this->tasks.empty())
                            {
                                return;
                            }
                            
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        template<class Callback, class... Args>
        auto enqueue(Callback&& f, Args&&... args) -> future<typename invoke_result<Callback, Args...>::type>
        {
            using return_type = typename std::invoke_result<Callback, Args...>::type;

            auto task = make_shared<packaged_task<return_type()>>
            (
                bind(forward<Callback>(f), forward<Args>(args)...)
            );

            future<return_type> res = task->get_future();
            {
                unique_lock<std::mutex> lock(queueMutex);
                if (stop)
                {
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                }

                tasks.emplace([task]()
                {
                    (*task)();
                });
            }
            condition.notify_one();

            return res;
        }
        
        ~ThreadPool()
        {
            {
                unique_lock<mutex> lock(queueMutex);
                stop = true;
            }
            condition.notify_all();

            for (thread &worker: workers)
            {
                worker.join();
            }
        }

    private:
        vector<thread> workers;
        queue<function<void()>> tasks;
        mutex queueMutex;
        condition_variable condition;
        bool stop;

};
/** USAGE: ->
    int main()
    {
        // Create a ThreadPool with 4 worker threads
        ThreadPool pool(4);

        // Enqueue some tasks into the pool
        future<void> result1 = pool.enqueue(workFunction, 1);
        future<void> result2 = pool.enqueue(workFunction, 2);

        // Optionally, wait for a specific task to complete
        result1.wait();
        result2.wait();

        // The ThreadPool will automatically wait for all enqueued tasks to complete
        // upon destruction, so an explicit wait is not strictly necessary unless
        // you need to synchronize with tasks' completion.

        return 0;
    }

    void foo()
    {
        // Assuming ThreadPool is defined and instantiated
        ThreadPool pool(4);

        auto futureResult = enqueueTask(pool, [](int x) -> int
        {
            return x * 2;
        }
        , 10);

        int result = futureResult.get();
        cout << "The result is: " << result << '\n';
    }
*/

class ThreadWrapper
{
    public:
        ThreadWrapper()
        : running(false)
        {}

        ~ThreadWrapper()
        {
            stop();
        }

        // Delete copy constructor and assignment operator
        ThreadWrapper(const ThreadWrapper&) = delete;
        ThreadWrapper& operator=(const ThreadWrapper&) = delete;

        // Start the thread with a new task
        template<typename Callable, typename... Args>
        void start(Callable&& task, Args&&... args)
        {
            // Ensure any running thread is stopped before starting a new one
            stop();

            // Set the running flag and start the new thread
            running = true;
            thread = thread([this, task = function<void()>(bind(forward<Callable>(task), forward<Args>(args)...))]
            {
                task();
                running = false;
            });
        }

        // Stop the thread
        void stop()
        {
            if (running)
            {
                // Set running flag to false, allowing the thread to exit if it checks this flag
                running = false;
                if (thread.joinable())
                {
                    // Wait for the thread to finish execution
                    thread.join();
                }
            }
        }

        // Check if the thread is running
        bool isRunning() const
        {
            return running;
        }

    private:
        thread thread;
        atomic<bool> running;
};

class TimedDataSender
{
    void loop()
    {
        using namespace std::chrono;
        auto next_run_time = steady_clock::now() + milliseconds(interval_);

        while (active_.load())
        {
            auto now = steady_clock::now();
            if (now >= next_run_time)
            {
                // It's time to perform the task
                task_();
                
                // Schedule the next run
                next_run_time = now + milliseconds(interval_);
            }
            else
            {
                // Sleep for a short while to prevent busy waiting
                this_thread::sleep_for(milliseconds(1));
            }
        }
    }

    thread worker_;
    atomic<bool> active_;
    
    // The interval between task executions, in milliseconds
    unsigned int interval_;
    
    // The task to be performed
    function<void()> task_;

    // Added to indicate whether the loop is paused
    atomic<bool> paused_;
 
    // Mutex for condition variable
    mutex mtx_;
    
    // Condition variable for pausing and resuming
    condition_variable cv_;

public:
    // Constructor: Takes the interval in milliseconds and the task to perform
    TimedDataSender(unsigned interval, std::function<void()> task)
    : interval_(interval), task_(std::move(task)), active_(true)
    {
        worker_ = std::thread([this]() { this->loop(); });
    }

    ~TimedDataSender()
    {
        // Signal the loop to stop and join the thread upon destruction
        active_.store(false);
        if (worker_.joinable())
        {
            worker_.join();
        }
    }

    TimedDataSender(const TimedDataSender&) = delete;
    TimedDataSender& operator=(const TimedDataSender&) = delete;
    TimedDataSender(TimedDataSender&&) = delete;
    TimedDataSender& operator=(TimedDataSender&&) = delete;

    void stop()
    {
        active_.store(false);
        resume(); // Ensure we wake up the loop for it to exit
        if (worker_.joinable())
        {
            worker_.join();
        }
    }

    void pause()
    {
        paused_.store(true);
    }

    void resume()
    {
        paused_.store(false);
        cv_.notify_one(); // Signal the condition variable to wake the loop
    }
};


class TimedDataSenderSeperateThread
{
    void loop()
    {
        using namespace std::chrono;
        auto next_run_time = steady_clock::now() + milliseconds(interval_);

        while (active_.load())
        {
            auto now = steady_clock::now();
            if (now >= next_run_time)
            {
                // Launch the task in a detached thread to allow the loop to continue immediately
                thread([this]() { task_(); }).detach();
                next_run_time += milliseconds(interval_); // Schedule the next run
            }
            else
            {
                this_thread::sleep_for(milliseconds(1)); // Sleep to prevent busy waiting
            }
        }
    }

    thread worker_;
    atomic<bool> active_;
    unsigned int interval_; // The interval between task executions, in milliseconds
    function<void()> task_; // The task to be performed

public:
    // Constructor: Takes the interval in milliseconds and the task to perform
    TimedDataSenderSeperateThread(unsigned interval, function<void()> task)
    : interval_(interval), task_(std::move(task)), active_(true)
    {
        worker_ = thread([this]() { this->loop(); });
    }

    ~TimedDataSenderSeperateThread()
    {
        // Signal the loop to stop and join the thread upon destruction
        active_.store(false);
        if (worker_.joinable())
        {
            worker_.join();
        }
    }

    // Deleted copy/move constructors and assignment operators for thread safety
    TimedDataSenderSeperateThread(const TimedDataSenderSeperateThread&) = delete;
    TimedDataSenderSeperateThread& operator=(const TimedDataSenderSeperateThread&) = delete;
    TimedDataSenderSeperateThread(TimedDataSenderSeperateThread&&) = delete;
    TimedDataSenderSeperateThread& operator=(TimedDataSenderSeperateThread&&) = delete;

    void stop()
    {
        if (active_.exchange(false))
        {
            if (worker_.joinable())
            {
                worker_.join();
            }
        }
    }
};
using tdssp = TimedDataSenderSeperateThread;

class TimedDataSender_pause
{
private:
    thread worker_;
    atomic<bool> active_;
    atomic<bool> paused_; // Added to indicate whether the loop is paused
    unsigned int interval_; // The interval between task executions, in milliseconds
    function<void()> task_; // The task to be performed
    mutex mtx_; // Mutex for condition variable
    condition_variable cv_; // Condition variable for pausing and resuming

    void loop()
    {
        using namespace std::chrono;
        auto next_run_time = steady_clock::now() + milliseconds(interval_);

        while (active_.load())
        {
            // Check if paused
            unique_lock<mutex> lock(mtx_);
            cv_.wait(lock, [this](){ return !paused_.load() || !active_.load(); });

            if (!active_.load()) break; // If active_ is false, exit loop

            lock.unlock(); // Unlock before potentially lengthy operation

            auto now = steady_clock::now();
            if (now >= next_run_time)
            {
                task_(); // It's time to perform the task
                next_run_time = now + milliseconds(interval_); // Schedule the next run
            }
            else
            {
                this_thread::sleep_for(milliseconds(1)); // Sleep for a short while to prevent busy waiting
            }
        }
    }

public:
    // Constructor: Takes the interval in milliseconds and the task to perform
    TimedDataSender_pause(unsigned interval, function<void()> task)
        : interval_(interval), task_(std::move(task)), active_(true), paused_(false)
    {
        worker_ = thread([this]() { this->loop(); });
    }

    ~TimedDataSender_pause()
    {
        stop(); // Ensure the loop is stopped and the thread is joined upon destruction
    }

    // Disallow copy and move semantics
    TimedDataSender_pause(const TimedDataSender_pause&) = delete;
    TimedDataSender_pause& operator=(const TimedDataSender_pause&) = delete;
    TimedDataSender_pause(TimedDataSender&&) = delete;
    TimedDataSender_pause& operator=(TimedDataSender_pause&&) = delete;

    void stop()
    {
        active_.store(false);
        resume(); // Ensure we wake up the loop for it to exit
        if (worker_.joinable())
        {
            worker_.join();
        }
    }

    void pause()
    {
        paused_.store(true);
    }

    void resume()
    {
        paused_.store(false);
        cv_.notify_one(); // Signal the condition variable to wake the loop
    }
};

#include "Log.hpp"

class AsyncWrapper_first {
    public:
        AsyncWrapper_first() = default;

        // Delete copy semantics
        AsyncWrapper_first(const AsyncWrapper_first&) = delete;
        AsyncWrapper_first& operator=(const AsyncWrapper_first&) = delete;

        // Allow move semantics
        AsyncWrapper_first(AsyncWrapper_first&&) = default;
        AsyncWrapper_first& operator=(AsyncWrapper_first&&) = default;

        // Start an asynchronous task
        template<typename Callable, typename... Args>
        void start(Callable&& task, Args&&... args) {
            // Ensure any previous task is not pending
            if (future.valid()) {
                try {
                    future.get(); // Attempt to retrieve the result, if any, or wait for completion
                } catch (const std::exception& e) {
                    // Handle or log exceptions from the previous task
                    loutE << "Exception from previous task: " << e.what() << loutEND;
                }
            }

            // Launch a new asynchronous task
            future = std::async(std::launch::async, std::forward<Callable>(task), std::forward<Args>(args)...);
        }

        // Attempt to retrieve the result of the asynchronous task, waiting if necessary
        template<typename ResultType>
        ResultType get() {
            if (!future.valid()) {
                throw std::runtime_error("No valid future available. Did you start a task?");
            }
            return future.get(); // This will wait for the task to complete if it hasn't already
        }

    private:
        std::future<void> future;

};

enum class tState {
    Idle,
    Waiting,
    Running,
    Completed,
    Exception
};

class AsyncWrapper {
    public:
        AsyncWrapper() : state(tState::Idle) {}

        // Prevent copy operations
        AsyncWrapper(const AsyncWrapper&) = delete;
        AsyncWrapper& operator=(const AsyncWrapper&) = delete;

        // Enable move operations
        AsyncWrapper(AsyncWrapper&&) = delete;
        AsyncWrapper& operator=(AsyncWrapper &&) = delete;

        template<typename Callable, typename... Args>
        void start(Callable&& task, Args&&... args) {
            {
                std::lock_guard<std::mutex> lock(mutex);
                if (state != tState::Idle) {
                    throw std::runtime_error("Task is already set or running.");
                }
                // Set the task but don't start it yet
                this->task = std::bind(std::forward<Callable>(task), std::forward<Args>(args)...);
                state = tState::Waiting;
            }
        }

        // Call this method to start the task
        void trigger() {
            {
                std::lock_guard<std::mutex> lock(mutex);
                if (state != tState::Waiting) {
                    throw std::runtime_error("No task is waiting to be triggered.");
                }
                state = tState::Running;
            }
            condition.notify_one(); // Signal the condition variable to start the task

            future = std::async(std::launch::async, [this]() {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this]{ return state == tState::Running; }); // Wait to be triggered
                try {
                    task(); // Execute the task
                    state = tState::Completed;
                } catch (...) {
                    state = tState::Exception;
                }
            });
        }

        tState getState() const {
            return state;
        }

        bool isFinished() const {
            return state == tState::Completed || state == tState::Exception;
        }

    private:
        std::future<void> future;
        std::function<void()> task;
        std::mutex mutex;
        std::condition_variable condition;
        std::atomic<tState> state;
};

class AsyncT {

    
};

class root_thread {
    private:


    public:
        vector<thread> threads;

};

#endif
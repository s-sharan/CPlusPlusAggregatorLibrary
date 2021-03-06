#ifndef SINGLELOCKQUEUE_CPP_
#define SINGLELOCKQUEUE_CPP_

#include "queue"
#include <mutex>
#include "graphexecutor.h"

using namespace std;
using namespace GraphExecutorService;

template<typename T>
class SingleLockQueue : public ThreadSafeQueue<T> {
private:
    mutable mutex m;
    queue<T> task_queue;
    condition_variable condition;
    
public:
    SingleLockQueue(){}
    
    void push(T task){
        lock_guard<mutex> lk(m);
        task_queue.push(task);
        condition.notify_one();
    }
    
    T wait_and_pop(){
        unique_lock<mutex> lk(m);
        condition.wait(lk,[this]{return !task_queue.empty();});
        T res = move(task_queue.front());
        task_queue.pop();
        return res;
    }
    
    bool empty() const {
        lock_guard<mutex> lk(m);
        return task_queue.empty();
    }
    
    int size() const {
        lock_guard<mutex> lk(m);
        return task_queue.size();
    }

    ~SingleLockQueue() {}
};

#endif /* SINGLELOCKQUEUE_CPP_ */

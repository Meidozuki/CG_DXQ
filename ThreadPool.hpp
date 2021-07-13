#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<iostream>
#include<queue>
#include<thread>
#include<atomic>
#include<vector>
#include<stdexcept>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<future>
using namespace std;


namespace Thread
{
    #define Pool_Max_Num 16
    
    class ThreadPool
    {
        private:
            std::vector<std::thread> pools;   //线程池
            using Task = std::function<void()>;  //任务
            std::queue<Task> tasks; //任务列表
            std::mutex locks;    //锁
            std::condition_variable condition;   //条件阻塞
            std::atomic<bool> isRun{ true };  //线程池是否运行
            std::atomic<int> idleThreadNum{ 0 };    //线程池空闲线程数
            std::atomic<bool> stopQueue{ false };

        public:
            ThreadPool(size_t size);
            ~ThreadPool();
            void AddThread(size_t size);    //添加线程数
            void terminate();//强制结束
            int idleThreadCount();  //空闲线程数
            int threadCount();  //线程数
            template<class F, class... Args>
            auto enqueue(F&&f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
            void wait_for_tasks();

    };

}


#endif
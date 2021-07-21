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

inline Thread::ThreadPool::ThreadPool(size_t size)
{
	AddThread(size);    //手动设定线程数
}

inline Thread::ThreadPool::~ThreadPool()
{
	terminate();    //强行终止
}

//任务等待分配线程
template<class F, class... Args>
auto Thread::ThreadPool::enqueue(F&&f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	while (stopQueue);   //停止入队，等待操作完成。

	using fun_type = typename std::result_of<F(Args...)>::type;
	if (!isRun)
	{
		throw "Thread is stopped\n";
	}
	auto task = make_shared<std::packaged_task<fun_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
	std::future<fun_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(locks);
		if (!isRun)
		{
			throw "Thread is stopped\n";
		}
		tasks.emplace([task]() { //放到队列后面
			(*task)();
		});
	}

	condition.notify_one();//唤醒一个线程
	return res;
}




#endif
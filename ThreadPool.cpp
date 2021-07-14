#include"ThreadPool.hpp"
#include<ctime>

inline Thread::ThreadPool::ThreadPool(size_t size)
{
    AddThread(size);    //手动设定线程数
}

inline Thread::ThreadPool::~ThreadPool()
{
    terminate();    //强行终止
}

void Thread::ThreadPool::AddThread(size_t size) //添加需要多少线程
{
    for(int i=0;pools.size() < Pool_Max_Num && i<size; i++)
    {
        pools.emplace_back([this]{
            while(isRun)
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(locks);
                    condition.wait(lock,[this]{
                        return !isRun || !tasks.empty();
                    });
                    if(!isRun && tasks.empty())
                        return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                idleThreadNum--;
                task();
                idleThreadNum++;

            }
        });
        idleThreadNum++;
    }
}

void Thread::ThreadPool::terminate()    //强制结束
{
    
    isRun = false;
    condition.notify_all(); //唤醒所有线程
    for(std::thread & thr : pools)
    {
        if(thr.joinable())
        {
            thr.join();               
        }
    }
}

int Thread::ThreadPool::idleThreadCount()   //统计空闲线程数
{
    return this->idleThreadNum;
}

int Thread::ThreadPool::threadCount()   //统计线程数
{
    return this->pools.size();
}

//任务等待分配线程
template<class F, class... Args>
auto Thread::ThreadPool::enqueue(F&&f,Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    while(stopQueue);   //停止入队，等待操作完成。

    using fun_type = typename std::result_of<F(Args...)>::type;
    if(!isRun)
    {
        throw "Thread is stopped\n";
    }
    auto task = make_shared<std::packaged_task<fun_type()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    std::future<fun_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(locks);
        if(!isRun)
        {
            throw "Thread is stopped\n";
        }
        tasks.emplace([task](){ //放到队列后面
            (*task)();
        });
    }

    condition.notify_one();//唤醒一个线程
    return res;
}

void Thread::ThreadPool::wait_for_tasks()    //等待所有任务结束
{
    stopQueue = true;
    while(tasks.size());    //存在任务，则等待分配完成。
    stopQueue = false;

}

//使用例子
void func()
{
    int sum =1;
    for(int i=0;i<1e6;i++)
    {
        sum=((sum+1)*2)/2;
    }
}

int main()
{
    Thread::ThreadPool *pool= new Thread::ThreadPool(16);//创建一个线程池，线程数16
    clock_t start1 = clock();
    int t = 20000;  //处理20000次func函数
    while(t)    //使用线程
    {
        pool->enqueue(func);
        t--;
    }
    pool->wait_for_tasks(); //等待所有任务结束
    clock_t end1 = clock();
    //多线程使用的时间
    std::cout <<"run time using threads is :"<< (double)(end1 - start1) / CLOCKS_PER_SEC <<endl;
    
    //单线程计算时间
    clock_t start2 = clock();
    t=20000;
    while(t)
    {
        func();
        t--;
    }
    clock_t end2 = clock();
    std::cout<<"run time within threadsg++ is:" << (double)(end2 - start2) / CLOCKS_PER_SEC <<endl;

}

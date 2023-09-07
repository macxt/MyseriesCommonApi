#pragma once
#include "MySafeQueue.h"
#include "MyLogFile.h"
#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <memory>
#include <atomic>
#include <condition_variable>

/*
	@线程池的实现，仅对外提供提交任务的接口
	@author:mayabin
	@date:2023/03/17
	@description:利用互斥锁与条件变量通过发送通知与等待的信号实现线程间协作的机制，
	在构造函数中完成线程池初始化工作，在析构函数中阻塞等待所有线程完成各自任务关闭线程池
*/

#define DEFAULT_THREAD_NUM	20

class MyThreadPool
{
	using SubmitTaskType = std::function<void()>;
private:
	class WorkerThread		//线程池内置私有工作线程类
	{
	private:
		size_t nThrdID;					//工作线程ID

		MyThreadPool *selfThrdPool;		//工作线程所属线程池

	public:
		WorkerThread(size_t nThrdIndex, MyThreadPool *thrdPool) : nThrdID(nThrdIndex), selfThrdPool(thrdPool) {}

		void operator ()()											//重载（）运算符从任务队列取出任务并执行
		{
			std::atomic<bool> m_bIsPopTaskDone;						//判断出队一个任务时是否成功

			SubmitTaskType getOneTask;								//从任务队列取出一个任务

			while (!selfThrdPool->IsStopExitThrdPool())				//线程池未关闭退出之前循环从任务队列中取任务执行
			{
				{
					std::unique_lock<std::mutex> uniq_lck(selfThrdPool->m_GetTaskMutex);	//为所有的工作线程加锁

					selfThrdPool->m_cvAwakeThread.wait(uniq_lck, [this]() {					//任务队列不为空或者关闭退出线程池时解除当前线程的阻塞，否则持续阻塞当前线程
						return !selfThrdPool->m_SubmitTaskQueue.IsEmpty() || selfThrdPool->m_bIsStopThrdPool;
					});

					m_bIsPopTaskDone = selfThrdPool->m_SubmitTaskQueue.PopTask(getOneTask);		//从任务队列中取出一个任务
				}

				if (m_bIsPopTaskDone) {		//取任务成功则执行执行该任务
					getOneTask();
				}
			}
		}
	};

public:
	explicit MyThreadPool(size_t nThreadNums = DEFAULT_THREAD_NUM)
		:m_threadPool(std::vector<std::thread>(nThreadNums)), m_bIsStopThrdPool(false)
	{
		InitThreadPool();
	}

	MyThreadPool(const MyThreadPool &otherPool) = delete;					//禁用拷贝构造（只通过一种方式构造线程池对象）

	MyThreadPool& operator = (const MyThreadPool &otherPool) = delete;		//禁用拷贝赋值（只通过一种方式构造线程池对象）

	MyThreadPool(MyThreadPool &&otherPool) = delete;						//禁用移动构造（只通过一种方式构造线程池对象）

	MyThreadPool& operator = (MyThreadPool &&otherPool) = delete;			//禁用移动赋值（只通过一种方式构造线程池对象）

	~MyThreadPool()
	{
		StopRunThreadPool();
	}

	/*
		线程池对外提供的唯一接口，提交任务到任务队列
		该接口可接受任意可调用对象（普通函数，成员函数，lambda匿名函数等），任意参数，利用Cpp11众多新特性异步机制来实现
	*/
	template<typename FuncType, typename... Args>							//typename... 可变参数包
	auto CommitTask(FuncType&& func, Args&&... args) -> std::future<decltype(std::forward<FuncType>(func)(std::forward<Args>(args)...))>
	{
		using TaskRetType = decltype(std::forward<FuncType>(func)(std::forward<Args>(args)...));	//定义任务返回类型别名	

		std::function<TaskRetType()> calledFuncObj = std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...);	//对传入的可调用对象及参数进行一次封装

		auto task_ptr = std::make_shared<std::packaged_task<TaskRetType()>>(calledFuncObj);	//定义一个std::packaged_task类对象指针，指向封装后的可调用对象

		SubmitTaskType submitTask = [task_ptr]() {		//将封装的可调用对象包装成一个入队任务，确保任务被执行时即可调用对象的真正运行
			(*task_ptr)();
		};

		m_SubmitTaskQueue.PushTask(submitTask);			//向线程安全的任务队列入队一个任务

		m_cvAwakeThread.notify_one();					//唤醒一个线程执行任务，此时不必唤醒所有线程避免无效竞争

		return task_ptr->get_future();					//调用get_futrue()成员函数返回保有运行结果的future对象
	}

private:
	void InitThreadPool()								//初始化线程池接口，在构造函数中直接调用
	{
		for (size_t i = 0; i < m_threadPool.size(); i++)
		{
			m_threadPool.at(i) = std::thread(WorkerThread(i, this));		//为每个线程分配工作线程
		}
		MYLOG_INFO << "Succeed to add " << m_threadPool.size() << " threads to process tasks.";
	}

	void StopRunThreadPool()							//阻塞等待所有线程执行完任务然后退出，在析构函数中自动调用
	{
		m_bIsStopThrdPool = true;
		m_cvAwakeThread.notify_all();					//唤醒所有工作线程执行任务
		for (auto &TmpThread : m_threadPool)
		{
			if (TmpThread.joinable()) {
				TmpThread.join();
			}
		}
		MYLOG_INFO << "Remain " << m_SubmitTaskQueue.TaskSize() << " tasks to be processed.";
		MYLOG_INFO << "Used " << m_threadPool.size() << " threads to process tasks.";
	}

	bool IsStopExitThrdPool()		//停止线程池标识为true且任务队列为空时关闭退出线程池
	{
		return m_bIsStopThrdPool && m_SubmitTaskQueue.IsEmpty();
	}

private:

	std::mutex							m_GetTaskMutex;			//线程取任务时的互斥锁
	std::atomic<bool>					m_bIsStopThrdPool;		//是否关闭退出线程池
	std::condition_variable				m_cvAwakeThread;		//该条件变量唤醒一个或多个线程
	std::vector<std::thread>			m_threadPool;			//处理所有任务的线程池	
	ThreadSafeQueue<SubmitTaskType>		m_SubmitTaskQueue;		//收集所有提交任务的队列

};


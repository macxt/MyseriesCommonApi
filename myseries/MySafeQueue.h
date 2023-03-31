#pragma once
#include <queue>
#include <mutex>

/*
	@实现线程安全的任务队列，作为线程池的一个内部私有类对象使用
	@author:mayabin
	@date:2023/03/16
	@description:对Cpp11标准的容器队列做一次封装实现安全的入队出队接口方便在线程池中直接调用
*/

template <class TaskType>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() = default;
	ThreadSafeQueue(ThreadSafeQueue &&other) = default;
	~ThreadSafeQueue() = default;

	/*
		判断任务队列是否为空
	*/
	bool IsEmpty()
	{
		std::unique_lock<std::mutex> unq_lck(task_mtx);
		return m_TaskQueue.empty();
	}

	/*
		返回当前任务队列的任务数
	*/
	size_t TaskSize()
	{
		std::unique_lock<std::mutex> unq_lck(task_mtx);
		return m_TaskQueue.size();
	}

	/*
		入队一个任务
	*/
	void PushTask(TaskType &TmpTask)
	{
		std::unique_lock<std::mutex> uniq_lck(task_mtx);
		m_TaskQueue.emplace(TmpTask);
	}

	/*
		出队一个任务
	*/
	bool PopTask(TaskType &TmpTask)
	{
		std::unique_lock<std::mutex> uniq_lck(task_mtx);
		if (m_TaskQueue.empty())
			return false;

		TmpTask = std::move(m_TaskQueue.front());	//取出任务队列队首任务并进行资源转移
		m_TaskQueue.pop();							//删除队首任务

		return true;
	}

private:

	std::queue<TaskType> m_TaskQueue;	//任务队列

	std::mutex task_mtx;				//任务入队/出队锁

};
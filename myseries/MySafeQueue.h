#pragma once
#include <queue>
#include <mutex>

/*
	@ʵ���̰߳�ȫ��������У���Ϊ�̳߳ص�һ���ڲ�˽�������ʹ��
	@author:mayabin
	@date:2023/03/16
	@description:��Cpp11��׼������������һ�η�װʵ�ְ�ȫ����ӳ��ӽӿڷ������̳߳���ֱ�ӵ���
*/

template <class TaskType>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue() = default;
	ThreadSafeQueue(ThreadSafeQueue &&other) = default;
	~ThreadSafeQueue() = default;

	/*
		�ж���������Ƿ�Ϊ��
	*/
	bool IsEmpty()
	{
		std::unique_lock<std::mutex> unq_lck(task_mtx);
		return m_TaskQueue.empty();
	}

	/*
		���ص�ǰ������е�������
	*/
	size_t TaskSize()
	{
		std::unique_lock<std::mutex> unq_lck(task_mtx);
		return m_TaskQueue.size();
	}

	/*
		���һ������
	*/
	void PushTask(TaskType &TmpTask)
	{
		std::unique_lock<std::mutex> uniq_lck(task_mtx);
		m_TaskQueue.emplace(TmpTask);
	}

	/*
		����һ������
	*/
	bool PopTask(TaskType &TmpTask)
	{
		std::unique_lock<std::mutex> uniq_lck(task_mtx);
		if (m_TaskQueue.empty())
			return false;

		TmpTask = std::move(m_TaskQueue.front());	//ȡ��������ж������񲢽�����Դת��
		m_TaskQueue.pop();							//ɾ����������

		return true;
	}

private:

	std::queue<TaskType> m_TaskQueue;	//�������

	std::mutex task_mtx;				//�������/������

};
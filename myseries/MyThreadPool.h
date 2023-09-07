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
	@�̳߳ص�ʵ�֣��������ṩ�ύ����Ľӿ�
	@author:mayabin
	@date:2023/03/17
	@description:���û���������������ͨ������֪ͨ��ȴ����ź�ʵ���̼߳�Э���Ļ��ƣ�
	�ڹ��캯��������̳߳س�ʼ�������������������������ȴ������߳���ɸ�������ر��̳߳�
*/

#define DEFAULT_THREAD_NUM	20

class MyThreadPool
{
	using SubmitTaskType = std::function<void()>;
private:
	class WorkerThread		//�̳߳�����˽�й����߳���
	{
	private:
		size_t nThrdID;					//�����߳�ID

		MyThreadPool *selfThrdPool;		//�����߳������̳߳�

	public:
		WorkerThread(size_t nThrdIndex, MyThreadPool *thrdPool) : nThrdID(nThrdIndex), selfThrdPool(thrdPool) {}

		void operator ()()											//���أ�����������������ȡ������ִ��
		{
			std::atomic<bool> m_bIsPopTaskDone;						//�жϳ���һ������ʱ�Ƿ�ɹ�

			SubmitTaskType getOneTask;								//���������ȡ��һ������

			while (!selfThrdPool->IsStopExitThrdPool())				//�̳߳�δ�ر��˳�֮ǰѭ�������������ȡ����ִ��
			{
				{
					std::unique_lock<std::mutex> uniq_lck(selfThrdPool->m_GetTaskMutex);	//Ϊ���еĹ����̼߳���

					selfThrdPool->m_cvAwakeThread.wait(uniq_lck, [this]() {					//������в�Ϊ�ջ��߹ر��˳��̳߳�ʱ�����ǰ�̵߳��������������������ǰ�߳�
						return !selfThrdPool->m_SubmitTaskQueue.IsEmpty() || selfThrdPool->m_bIsStopThrdPool;
					});

					m_bIsPopTaskDone = selfThrdPool->m_SubmitTaskQueue.PopTask(getOneTask);		//�����������ȡ��һ������
				}

				if (m_bIsPopTaskDone) {		//ȡ����ɹ���ִ��ִ�и�����
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

	MyThreadPool(const MyThreadPool &otherPool) = delete;					//���ÿ������죨ֻͨ��һ�ַ�ʽ�����̳߳ض���

	MyThreadPool& operator = (const MyThreadPool &otherPool) = delete;		//���ÿ�����ֵ��ֻͨ��һ�ַ�ʽ�����̳߳ض���

	MyThreadPool(MyThreadPool &&otherPool) = delete;						//�����ƶ����죨ֻͨ��һ�ַ�ʽ�����̳߳ض���

	MyThreadPool& operator = (MyThreadPool &&otherPool) = delete;			//�����ƶ���ֵ��ֻͨ��һ�ַ�ʽ�����̳߳ض���

	~MyThreadPool()
	{
		StopRunThreadPool();
	}

	/*
		�̳߳ض����ṩ��Ψһ�ӿڣ��ύ�����������
		�ýӿڿɽ�������ɵ��ö�����ͨ��������Ա������lambda���������ȣ����������������Cpp11�ڶ��������첽������ʵ��
	*/
	template<typename FuncType, typename... Args>							//typename... �ɱ������
	auto CommitTask(FuncType&& func, Args&&... args) -> std::future<decltype(std::forward<FuncType>(func)(std::forward<Args>(args)...))>
	{
		using TaskRetType = decltype(std::forward<FuncType>(func)(std::forward<Args>(args)...));	//�������񷵻����ͱ���	

		std::function<TaskRetType()> calledFuncObj = std::bind(std::forward<FuncType>(func), std::forward<Args>(args)...);	//�Դ���Ŀɵ��ö��󼰲�������һ�η�װ

		auto task_ptr = std::make_shared<std::packaged_task<TaskRetType()>>(calledFuncObj);	//����һ��std::packaged_task�����ָ�룬ָ���װ��Ŀɵ��ö���

		SubmitTaskType submitTask = [task_ptr]() {		//����װ�Ŀɵ��ö����װ��һ���������ȷ������ִ��ʱ���ɵ��ö������������
			(*task_ptr)();
		};

		m_SubmitTaskQueue.PushTask(submitTask);			//���̰߳�ȫ������������һ������

		m_cvAwakeThread.notify_one();					//����һ���߳�ִ�����񣬴�ʱ���ػ��������̱߳�����Ч����

		return task_ptr->get_future();					//����get_futrue()��Ա�������ر������н����future����
	}

private:
	void InitThreadPool()								//��ʼ���̳߳ؽӿڣ��ڹ��캯����ֱ�ӵ���
	{
		for (size_t i = 0; i < m_threadPool.size(); i++)
		{
			m_threadPool.at(i) = std::thread(WorkerThread(i, this));		//Ϊÿ���̷߳��乤���߳�
		}
		MYLOG_INFO << "Succeed to add " << m_threadPool.size() << " threads to process tasks.";
	}

	void StopRunThreadPool()							//�����ȴ������߳�ִ��������Ȼ���˳����������������Զ�����
	{
		m_bIsStopThrdPool = true;
		m_cvAwakeThread.notify_all();					//�������й����߳�ִ������
		for (auto &TmpThread : m_threadPool)
		{
			if (TmpThread.joinable()) {
				TmpThread.join();
			}
		}
		MYLOG_INFO << "Remain " << m_SubmitTaskQueue.TaskSize() << " tasks to be processed.";
		MYLOG_INFO << "Used " << m_threadPool.size() << " threads to process tasks.";
	}

	bool IsStopExitThrdPool()		//ֹͣ�̳߳ر�ʶΪtrue���������Ϊ��ʱ�ر��˳��̳߳�
	{
		return m_bIsStopThrdPool && m_SubmitTaskQueue.IsEmpty();
	}

private:

	std::mutex							m_GetTaskMutex;			//�߳�ȡ����ʱ�Ļ�����
	std::atomic<bool>					m_bIsStopThrdPool;		//�Ƿ�ر��˳��̳߳�
	std::condition_variable				m_cvAwakeThread;		//��������������һ�������߳�
	std::vector<std::thread>			m_threadPool;			//��������������̳߳�	
	ThreadSafeQueue<SubmitTaskType>		m_SubmitTaskQueue;		//�ռ������ύ����Ķ���

};


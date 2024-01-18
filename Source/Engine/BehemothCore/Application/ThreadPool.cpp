#include "pch.h"
#include "ThreadPool.h"

namespace Behemoth
{


	ThreadPool& ThreadPool::GetInstance()
	{
		static ThreadPool instance;
		return instance;
	}

	void ThreadPool::WaitForCompletion()
	{
		std::unique_lock<std::mutex> lock(queueMutex);
		completionCondition.wait(lock, [this] { return this->tasks.empty() && this->activeTasks == 0; });
	}

	ThreadPool::ThreadPool() : stop(false)
	{
		int numThreads = (int)(std::thread::hardware_concurrency() * 0.5);

		for (size_t i = 0; i < numThreads; ++i)
		{
			workers.emplace_back([this]
				{
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(queueMutex);
							condition.wait(lock, [this]
								{
									return stop || !tasks.empty();
								});
							if (stop && tasks.empty())
							{
								return;
							}
							task = std::move(tasks.front());
							tasks.pop();
							activeTasks++;
						}
						task();
						{
							std::lock_guard<std::mutex> lock(this->queueMutex);
							if (--this->activeTasks == 0 && this->tasks.empty()) {
								this->completionCondition.notify_all();
							}
						}
					}
				});
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			stop = true;
		}
		condition.notify_all();
		for (std::thread& worker : workers)
			worker.join();
	}
}
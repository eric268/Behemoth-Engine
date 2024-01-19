#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <stdexcept>
#include <queue>

#include "Core/Log.h"

namespace Behemoth
{
	class ThreadPool
	{
	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		~ThreadPool();

		static ThreadPool& GetInstance();

		void Enqueue(const std::function<void()>& task) 
		{
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				tasks.push(task);
			}
			condition.notify_one();
		}

		void WaitForCompletion();

	private:
		ThreadPool();

		std::vector<std::thread> workers;
		std::queue<std::function<void()>> tasks;

		std::mutex queueMutex;
		std::condition_variable  condition;

		std::condition_variable completionCondition;
		std::atomic<size_t> activeTasks;

		bool stop;
	};
}
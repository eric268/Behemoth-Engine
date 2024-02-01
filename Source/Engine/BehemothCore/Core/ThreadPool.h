#pragma once

#include "Core/Log.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <stdexcept>
#include <queue>

namespace Behemoth
{
	class ThreadPool
	{
	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		~ThreadPool();

		static ThreadPool& GetInstance();
		void Enqueue(const std::function<void()>& task);
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
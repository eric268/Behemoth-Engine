#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <stdexcept>
#include <queue>

#include "Misc/Log.h"

namespace Behemoth
{
	class ThreadPool
	{
	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		~ThreadPool();

		static ThreadPool& GetInstance();

		template<class F, class... Args>
		auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>
		{
			using return_type = typename std::invoke_result<F, Args...>::type;

			auto task = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...));

			std::future<return_type> res = task->get_future();
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				if (stop) 
				{
					LOGMESSAGE(Error, "enqueue on stopped ThreadPool");
					throw;
				}
				tasks.emplace([task]() { (*task)(); });
			}
			condition.notify_one();
			return res;
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
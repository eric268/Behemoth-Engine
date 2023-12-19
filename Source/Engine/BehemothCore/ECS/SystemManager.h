#pragma once

#include <unordered_set>

class SystemManager
{
public:
	SystemManager(SystemManager&) = delete;
	SystemManager& operator=(const SystemManager&) = delete;

	static SystemManager& GetInstance()
	{
		static SystemManager instance;
		return instance;
	}

private:
	SystemManager() = default;

	std::unordered_set<int> systemTypeID;


};


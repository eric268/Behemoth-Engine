#pragma once

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

#include "Log.h"

class Stopwatch
{
public:
	Stopwatch(bool output = false, std::string filename = "") : outputToLog(output), filename(filename)
	{
		Start();
	}
	~Stopwatch()
	{
		Stop();
	}

private:
	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	void Stop()
	{
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
		
		std::string output = "Duration: " + std::to_string(duration.count()) + " microseconds";
		std::cout << output << std::endl;

		if (outputToLog)
		{
			OutputToLog(output);
		}
	}

	void OutputToLog(std::string& output)
	{
		std::ofstream file("../Log/" + filename, std::ios::app);
		if (file.is_open())
		{
			std::string configuration = "Config: ";

#ifdef DEBUG
			configuration += "Debug ";
#elif RELEASE
			configuration += "Release ";
#elif DIST
			configuration += "Dist ";
#endif
			file << configuration << output << std::endl;
			file.close();
		}
		else
		{
			LOG_ERROR("Unable to output stopwatch to log");
		}
	}

	bool outputToLog;
	std::string filename;
	std::chrono::high_resolution_clock::time_point start;
};
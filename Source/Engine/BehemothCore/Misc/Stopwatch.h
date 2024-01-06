#pragma once

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

#include "Log.h"

template <typename T>
struct UnitOfMeasurment;

template <>
struct UnitOfMeasurment<std::chrono::nanoseconds> {
	static std::string name() { return " nanoseconds"; }
};

template <>
struct UnitOfMeasurment<std::chrono::microseconds> {
	static std::string name() { return " microseconds"; }
};

template <>
struct UnitOfMeasurment<std::chrono::milliseconds> {
	static std::string name() { return " milliseconds"; }
};

template <>
struct UnitOfMeasurment<std::chrono::seconds> {
	static std::string name() { return " seconds"; }
};

template <>
struct UnitOfMeasurment<std::chrono::minutes> {
	static std::string name() { return " minutes"; }
};

template <>
struct UnitOfMeasurment<std::chrono::hours> {
	static std::string name() { return " hours"; }
};

template <typename T>
concept IsChronoType = requires(T a) {
	typename T::rep;
	typename T::period;
	{ T::zero() } -> std::same_as<T>;
	{ T::min() } -> std::same_as<T>;
	{ T::max() } -> std::same_as<T>;
};

template <IsChronoType T = std::chrono::microseconds>
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
		auto duration = std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - start);
		
		std::string output = "Duration: " + std::to_string(duration.count()) + UnitOfMeasurment<T>::name();
		std::cout << std::to_string(duration.count()) << std::endl;

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
			LOG_MESSAGE(MessageType::Warning, "Unable to output stopwatch to log");
		}
	}

	bool outputToLog;
	std::string filename;
	std::chrono::high_resolution_clock::time_point start;
};
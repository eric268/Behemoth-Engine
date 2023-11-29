#include "Log.h"

std::string TrimFilenamePath(const std::string& fullPath)
{
	size_t lastSlash = fullPath.find_last_of("\\/");
	if (lastSlash == std::string::npos)
	{
		return fullPath;
	}

	size_t secondLastSlash = fullPath.substr(0, lastSlash).find_last_of("\\/");
	if (secondLastSlash == std::string::npos)
	{
		return fullPath.substr(lastSlash + 1); // Return one segment if only one backslash found
	}

	return fullPath.substr(secondLastSlash + 1); // Return last two segments
}

void Log(const std::string& message, const std::string& filename, int line)
{
	std::string file = TrimFilenamePath(filename);
	std::cerr << "LOG: " << message << " (File: " << file << ", Line: " << line << ")\n";
}
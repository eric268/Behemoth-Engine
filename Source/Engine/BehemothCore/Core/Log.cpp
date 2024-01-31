#include "pch.h"
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
		// Return one segment if only one backslash found
		return fullPath.substr(lastSlash + 1);
	}
	// Return last two segments
	return fullPath.substr(secondLastSlash + 1);
}

std::string GetMessageType(MessageType type)
{
	std::string messageType = "";

#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (type)
	{
	case MessageType::General:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		messageType = "General: ";
		break;
	case MessageType::Warning:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		messageType = "Warning: ";
		break;
	case MessageType::Error:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		messageType = "Error: ";
		break;
	}
	return messageType;

#else
	switch (type)
	{
	case MessageType::General:
		messageType = "General: ";
		break;
	case MessageType::Warning:
		messageType = "Warning: ";
		break;
	case MessageType::Error:
		messageType = "Error: ";
		break;
	}
	return messageType;

#endif
}

void Log(MessageType type, const std::string& message, const std::string& filename, int line)
{
	std::string messageType = GetMessageType(type);
	std::string file = TrimFilenamePath(filename);
	std::cerr << "LOG - " << messageType << message << " (File: " << file << ", Line: " << line << ")\n";

#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);
#endif
}
#pragma once
#include <iostream>
#include <string>

enum MessageType
{
	General,
	Warning,
	Error
};

std::string GetMessageType(MessageType type);
void Log(MessageType type, const std::string& message, const std::string& filename, int line);
std::string TrimFilenamePath(const std::string& fullPath);

#define LOGMESSAGE(type, message) Log(type, message, __FILE__, __LINE__)
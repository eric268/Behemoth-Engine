#pragma once
#include <iostream>
#include <string>

enum class MessageType
{
	General,
	Warning,
	Error
};

std::string GetMessageType(MessageType type);
void Log(MessageType type, const std::string& message, const std::string& filename, int line);
std::string TrimFilenamePath(const std::string& fullPath);


#define LOG_ERROR(type, message) Log(type, message, __FILE__, __LINE__)
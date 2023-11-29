#pragma once
#include <iostream>
#include <string>

void Log(const std::string& message, const std::string& filename, int line);
std::string TrimFilenamePath(const std::string& fullPath);


#define LOG_ERROR(message) Log(message, __FILE__, __LINE__)
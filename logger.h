#pragma once
#include <string>
class logger {
private:
public:
	static void log(const char* s, ...);
	static void safe_exit(int code);
};


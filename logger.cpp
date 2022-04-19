#include "logger.h"
#include "config_parser/data.h"
#include <fstream>
#include <cstdarg>


void logger::log(const char* s, ...) {

	va_list list;

	va_start(list, s);

	char b[200];

	vsprintf_s(b, 200, s, list);

	printf(b);

	va_end(list);
	std::fstream file("log0.txt", std::fstream::out | std::fstream::app);

	std::string _final = "[MAIN]: ";
	_final += b;
	file << _final;

	file.close();
}

void logger::safe_exit(int code) {
	std::string script = data::current_path + "\\dep\\lua\\ps.exe -exec_revert";
	system(script.c_str());
	logger::log("{END}\n\n");

	exit(code);
}
#pragma once
#include "windows.h"
#include "fstream"
using namespace std;
class Log {
public:
	static Log*getInstance();

	void log_error(int line,char* linesentence, char *reason);
	void init();
private:
	bool isInit;
	Log();
	HANDLE mutex;
	std::ofstream out;
};
#include "log.h"


 Log* Log::getInstance()
{
	static Log log;
	return &log;
}

void Log::log_error(int line, char* linesentence, char *reason)
{
	WaitForSingleObject(mutex, INFINITE);
	if (reason != NULL)
		out << "line " << line << "  " << linesentence << "  " << reason << endl;
	else
		out << "line " << line << "  " << linesentence << "  "  << endl;
	ReleaseMutex(mutex);
}

Log::Log()
{
	mutex = CreateMutex(NULL, false, NULL);
	out.open("C://pe_log.txt", std::ios::app);
}

void Log::init()
{

}
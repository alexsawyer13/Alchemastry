#include "log.h"

#include <stdio.h>
#include <stdarg.h>

Logger logger;

int log_init(Log_Level level)
{
	logger.level = level;

	return 1;
}

void log_shutdown()
{
}

void alog(Log_Level level, const char *fmt, ...)
{
	va_list argptr;
	
	if (level < logger.level)
		return;

	va_start(argptr, fmt);

	switch(level)
	{
	case LOG_LEVEL_DEBUG:
		printf("[DEBUG]   : ");
		vfprintf(stdout, fmt, argptr);
	break;
	case LOG_LEVEL_VERBOSE:
		printf("[VERBOSE] : ");
		vfprintf(stdout, fmt, argptr);
	break;
	case LOG_LEVEL_INFO:
		printf("[INFO]    : ");
		vfprintf(stdout, fmt, argptr);
	break;
	case LOG_LEVEL_WARNING:
		printf("[WARNING] : ");
		vfprintf(stdout, fmt, argptr);
	break;
	case LOG_LEVEL_ERROR:
		fprintf(stderr, "[ERROR]   : ");
		vfprintf(stderr, fmt, argptr);
	break;
	case LOG_LEVEL_FATAL:
		fprintf(stderr, "[FATAL]   : ");
		vfprintf(stderr, fmt, argptr);
	break;
	}

	va_end(argptr);
}

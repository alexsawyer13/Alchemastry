#ifndef _LOG_H_
#define _LOG_H_

typedef enum
{
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_VERBOSE,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL
} Log_Level;

typedef struct
{
	Log_Level level;
} Logger;

int	log_init(Log_Level level);
void	log_shutdown();

void	alog(Log_Level level, const char *fmt, ...);

#define log_debug(fmt, ...) alog(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define log_verbose(fmt, ...) alog(LOG_LEVEL_VERBOSE, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) alog(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) alog(LOG_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) alog(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) alog(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

#endif

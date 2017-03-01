/**
 * @file llogDbg.c
 * @author Lucio Pintanel
 * @date 9 Aug 2016
 * @brief File containing includes, defines, and demias externados.
 *
 */

#ifndef __SLOG_H__
#define __SLOG_H__


/* For include header in CPP code */
#ifdef __cplusplus
extern "C" {
#endif

/***	SESSION INCLUDE	***********************************************/
#include <pthread.h>

/**********************************************************************/

/***	SESSION DEFINE, STRUCT, TYPEDEF	*******************************/
/* Loging flags */
#define SLOG_NONE   0
#define SLOG_LIVE   1
#define SLOG_INFO   2
#define SLOG_WARN   3
#define SLOG_DEBUG  4
#define SLOG_ERROR  5
#define SLOG_FATAL  6
#define SLOG_PANIC  7


/* Supported colors */
#define CLR_NORMAL   "\x1B[0m"
#define CLR_RED      "\x1B[31m"
#define CLR_GREEN    "\x1B[32m"
#define CLR_YELLOW   "\x1B[33m"
#define CLR_BLUE     "\x1B[34m"
#define CLR_NAGENTA  "\x1B[35m"
#define CLR_CYAN     "\x1B[36m"
#define CLR_WHITE    "\x1B[37m"
#define CLR_RESET    "\033[0m"


/* Flags */
typedef struct {
    const char* fname;
    short file_level;
    short level;
    short to_file;
    short pretty;
    short filestamp;
    short td_safe;
} SlogFlags;


/* Date variables */
typedef struct {
    int year; 
    int mon; 
    int day;
    int hour;
    int min;
    int sec;
    int usec;
} SlogDate;

/**********************************************************************/

/***	SESSION VARIAVEIS GLOBAIS	*******************************/

/**********************************************************************/

/***	SESSION PROTOTIPO DE FUNCOES	*******************************/
/* 
 * Get library version. Function returns version and build number of slog 
 * library. Return value is char pointer. Argument min is flag for output 
 * format. If min is 1, function returns version in full  format, if flag 
 * is 0 function returns only version numbers, For examle: 1.0.52.
 */
const char* slog_version(int min);

/*
 * strclr - Colorize string. Function takes color value and string 
 * and returns colorized string as char pointer. First argument clr 
 * is color value (if it is invalid, function retunrs NULL) and second 
 * is string with va_list of arguments which one we want to colorize.
 */
char* strclr(const char* clr, char* str, ...);

/*
 * Return string in slog format. Function takes arguments
 * and returns string in slog format without printing and
 * saveing in file. Return value is char pointer.
 */
char* slog_get(SlogDate *pDate, char *msg, ...);

/*
 * slog - Log exiting process. Function takes arguments and saves
 * log in file if LOGTOFILE flag is enabled from config. Otherwise
 * it just prints log without saveing in file. Argument level is
 * logging level and flag is slog flags defined in slog.h header.
 */
void slog(int level, int flag, const char *msg, ...);

/*
 * Initialize slog library. Function parses config file and reads log
 * level and save to file flag from config. First argument is file name
 * where log will be saved and second argument conf is config file path
 * to be parsed and third argument lvl is log level for this message.
 */
void slog_init(const char* fname, const char* conf, int lvl, int flvl, int t_safe);


/* For include header in CPP code */
#ifdef __cplusplus
}
#endif


#endif /* __SLOG_H__ */


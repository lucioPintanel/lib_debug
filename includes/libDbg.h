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
#include <stdint.h>
#include <pthread.h>

#include "ini.h"

    /**********************************************************************/

    /***	SESSION DEFINE, STRUCT, TYPEDEF	*******************************/
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define LOCATION __FILE__ " : " TOSTRING(__LINE__)

    /* Loging flags */
#define LDBG_NONE   0
#define LDBG_LIVE   1
#define LDBG_INFO   2
#define LDBG_WARN   3
#define LDBG_DEBUG  4
#define LDBG_ERROR  5
#define LDBG_FATAL  6
#define LDBG_PANIC  7

    /***	Supported colors ************************************************************************/
#define INIT_MESSAGES(INIT_MESSAGE)\
INIT_MESSAGE(_CLR_NORMAL, "\x1B[0m")\
INIT_MESSAGE(_CLR_RED, "\x1B[31m")\
INIT_MESSAGE(_CLR_GREEN, "\x1B[32m")\
INIT_MESSAGE(_CLR_YELLOW, "\x1B[33m") \
INIT_MESSAGE(_CLR_BLUE, "\x1B[34m") \
INIT_MESSAGE(_CLR_NAGENTA, "\x1B[35m") \
INIT_MESSAGE(_CLR_CYAN, "\x1B[36m") \
INIT_MESSAGE(_CLR_WHITE, "\x1B[37m") \
INIT_MESSAGE(_CLR_RESET, "\033[0m")

    enum _colorID_t {
#define EXPAND_ENUM(ENCOLORID, STRCOLOR)   ENCOLORID,
	INIT_MESSAGES(EXPAND_ENUM)
#undef EXPAND_ENUM
    };

    typedef enum _colorID_t colorID_t;
    /*********************************************************************************************/

    /* Flags */
    typedef struct libDbgFlags_t srLibDbgFlags_t;
    /* Date variables */
    typedef struct libDbgDate_t stLibDbgDate_t;

    /**********************************************************************/

    /***	SESSION VARIAVEIS GLOBAIS	*******************************/

    /**********************************************************************/

    /***	SESSION PROTOTIPO DE FUNCOES	*******************************/
    /**
     * \brief 
     * _CLR_NORMAL = 0 \n
     * _CLR_RED = 1 \n
     * _CLR_GREEN = 2 \n
     * _CLR_YELLOW = 3 \n
     * _CLR_BLUE = 4 \n
     * _CLR_NAGENTA = 5 \n
     * _CLR_CYAN = 6 \n
     * _CLR_WHITE = 7 \n
     * 
     * \author 
     * \date que foi criado 18/10/16 20:49
     * @param colorId
     * @return 
     */
    char* libDbgGetColor(colorID_t);

    /* 
     * Get library version. Function returns version and build number of slog 
     * library. Return value is char pointer. Argument min is flag for output 
     * format. If min is 1, function returns version in full  format, if flag 
     * is 0 function returns only version numbers, For examle: 1.0.52.
     */
    const char* libDbgVersion(int min);

    /*
     * strclr - Colorize string. Function takes color value and string 
     * and returns colorized string as char pointer. First argument clr 
     * is color value (if it is invalid, function retunrs NULL) and second 
     * is string with va_list of arguments which one we want to colorize.
     */
    char* libDbgStrClr(char* clr, char* str, ...);

    /*
     * Return string in slog format. Function takes arguments
     * and returns string in slog format without printing and
     * saveing in file. Return value is char pointer.
     */
    char* libDbgGet(stLibDbgDate_t *pDate, char *msg, ...);

    /*
     * slog - Log exiting process. Function takes arguments and saves
     * log in file if LOGTOFILE flag is enabled from config. Otherwise
     * it just prints log without saveing in file. Argument level is
     * logging level and flag is slog flags defined in slog.h header.
     */
    void libDbg(const char *location, const char* context, int level, int flag, const char *msg, ...);

    /*
     * Initialize slog library. Function parses config file and reads log
     * level and save to file flag from config. First argument is file name
     * where log will be saved and second argument conf is config file path
     * to be parsed and third argument lvl is log level for this message.
     */
    void libDbgInit(const char* fname, const char* conf, int lvl, int flvl, int t_safe);


    /* For include header in CPP code */
#ifdef __cplusplus
}
#endif


#endif /* __SLOG_H__ */


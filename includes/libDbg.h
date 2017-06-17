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
     * \brief Get library version. Function returns version and build number of llogDbg 
     * library. Return value is char pointer. Argument min is flag for output 
     * format. If min is 1, function returns version in full  format, if flag 
     * is 0 function returns only version numbers, For examle: 1.3.0
     * 
     * \author Lucio Pintanel
     * \date que foi criado 18/10/16 20:49
     * \param min - inteiro para solicitar versao simplificado ou detalhada.
     * \return verstr - string with version of library
     */
    const char* libDbgVersion(char* verstr, size_t sizeVrs, int min);

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
    void libDbgInit(const char* conf, int t_safe);


    /* For include header in CPP code */
#ifdef __cplusplus
}
#endif


#endif /* __SLOG_H__ */


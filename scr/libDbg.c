/**
 * @file llogDbg.c
 * @author Lucio Pintanel
 * @date 9 Aug 2016
 * @brief File containing includes, defines, and demias externados.
 *
 */

/***	SESSION INCLUDE	***********************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libDbg.h"

/**********************************************************************/

/***	SESSION DEFINE, STRUCT, TYPEDEF	*******************************/
/*! \def VERSAO_MAX
	\brief Definicao da versao maxima, esta definicao vem do Makefile.
*/
#define VERSAO_MAX _VERSAO_MAX
/*! \def VERSAO_MIN
	\brief Definicao da versao minima, esta definicao vem do Makefile.
*/
#define VERSAO_MIN _VERSAO_MIN
/*! \def VERSAO_NRO
	\brief Definicao da versao numero, esta definicao vem do Makefile.
*/
#define VERSAO_NRO _VERSAO_NRO

/*! \def MAXMSG
	\brief Max size of string.
*/
#define MAXMSG 8196

/*! \def MAXSTR
	\brief Max size of vetor.
*/
#define MAXSTR 256

/*! \struct colorLen_t libDbg.c
	\brief Supported colors.

	Esta estrutura eh criada via xmacro 
*/
typedef struct {
#define EXPAND_STRUCT(ENCOLORID, STRCOLOR)   uint8_t ENCOLORID;
    INIT_MESSAGES(EXPAND_STRUCT)
#undef EXPAND_STRUCT
} colorLen_t;

/*! \def MESSAGES
	\brief Tamanho da estrutura colorLen_t
*/
#define MESSAGES sizeof(colorLen_t)

/*! \var const colorTable[]
	\brief Vetor com a string inicializada com as cores e serem utilidas no printf

	Esta estrutura eh criada via xmacro
*/
static char const * const colorTable[] = {
#define EXPAND_STRINGS(ENCOLORID, STRCOLOR)   STRCOLOR,
    INIT_MESSAGES(EXPAND_STRINGS)
#undef EXPAND_STRINGS
};

/*!
	\var gFName[MAXSTR]
	\brief Vetor para guardar o nome do arquivo onde 
			vai ser gravado o log se a bibliotaca for configurada para tal
*/
char gFName[MAXSTR];

/*!
	\var td_safe
	\brief Variavel que controla se a biblioteca eh thread safe
*/
short td_safe = 1;

/*! \struct libDbgFlags_t libDbg.c
	\brief define the config struct type.
*/
typedef struct {
    int file_level;
    int level;
    int to_file;
    int pretty;
    int filestamp;
} libDbgFlags_t;

/*! \struct libDbgDate_t libDbg.c
	\brief Date variables.
*/
struct libDbgDate_t {
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
    int usec;
};

/*! \def LIBDBGDATE_DECLARE(name)
	\brief declare a initialized estrutura da data/hora
*/
#define LIBDBGDATE_DECLARE(name)  \
        stLibDbgDate_t name = {         \
        .year = 0, \
	.mon = 0, \
	.day = 0, \
	.hour = 0, \
	.min = 0, \
	.sec = 0, \
	.usec = 0, \
}

/*! \def CONFIGREADFILE
	\brief define o nome do arquivo de configuracao da biblioteca
*/
#define CONFIGREADFILE  "config.cnf"
/**********************************************************************/

/***	SESSION VARIAVEIS GLOBAIS	***********************************/
/*! \var static pthread_mutex_t lDbgmutex
	\brief declaracao da variavel de controle de lock/unlock da thread
*/
static pthread_mutex_t lDbgmutex;

/*! \var libDbgFlags_t libDbgFlags
	\brief declaracao e inicializacao da estrutura de data/hora
*/
libDbgFlags_t libDbgFlags;

/*! \var Config *gConfig
	\brief declaracao do ponteiro para estrura de controle do parse 
	dos arquivo .INI
*/
Config *gConfig = NULL;
/**********************************************************************/

/***	SESSION MACROS	***********************************************/
/* This is a program to measure the improvement of replacing the */
/* normal strcmp-function with the following macro. */

/*! \def STRICMP(x,y)
	\brief This is a program to measure the improvement of replacing the
			normal strcmp-function with the following macro.

*/
#define STRICMP(x,y) (*(x) == *(y) ? strcmp((x),(y)) : *(x) - *(y))

/**********************************************************************/

/***	SESSION PROTOTIPO DE FUNCOES	*******************************/

/**********************************************************************/

/***	SESSION FUNCOES	***********************************************/

/**
 * \brief funcao que informa a cor em funcao do enum colorID_t
 * _CLR_NORMAL = 0\n
 * _CLR_RED = 1\n
 * _CLR_GREEN = 2\n
 * _CLR_YELLOW = 3\n
 * _CLR_BLUE = 4\n
 * _CLR_NAGENTA = 5\n
 * _CLR_CYAN = 6\n
 * _CLR_WHITE = 7\n
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param colorId - Index with the position in vector of colorfull
 * \return lstrColor - String with of color
 */
char* libDbgGetColor(colorId)
colorID_t colorId;
{
    int index = (int) colorId;
    static char lstrColor[MAXMSG];
    if (index >= 0 && index < MESSAGES) {
	snprintf(lstrColor, sizeof (lstrColor), "%s", colorTable[index]);
	return lstrColor;
    }
    snprintf(lstrColor, sizeof (lstrColor), "%s", colorTable[index]);
    return lstrColor;
}

/**
 * \brief Intialize date with system date.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param sdate - pointer of SlogDate structure.
 */
void libDbgGetDate(stLibDbgDate_t *sdate)
{
    time_t rawtime;
    struct tm timeinfo;
    struct timespec now;
    rawtime = time(NULL);
    localtime_r(&rawtime, &timeinfo);

    /* Get System Date */
    sdate->year = timeinfo.tm_year + 1900;
    sdate->mon = timeinfo.tm_mon + 1;
    sdate->day = timeinfo.tm_mday;
    sdate->hour = timeinfo.tm_hour;
    sdate->min = timeinfo.tm_min;
    sdate->sec = timeinfo.tm_sec;

    /* Get micro seconds */
    clock_gettime(CLOCK_MONOTONIC, &now);
    sdate->usec = now.tv_nsec / 10000000;
}

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
const char* libDbgVersion(int min)
{
    static char verstr[128];

    /* Version short */
    if (min) sprintf(verstr, "%d.%d.%d",
	    VERSAO_MAX, VERSAO_MIN, VERSAO_NRO);

	/* Version long */
    else sprintf(verstr, "%d.%d build %d (%s)",
	    VERSAO_MAX, VERSAO_MIN, VERSAO_NRO, __DATE__);

    return verstr;
}

/**
 * \brief Colorize string. Function takes color value and string 
 * and returns colorized string as char pointer. First argument clr 
 * is color value (if it is invalid, function retunrs NULL) and second 
 * is string with va_list of arguments which one we want to colorize.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param clr - 
 * \param str - 
 * \return output - 
 */
char* libDbgStrClr(char* clr, char* str, ...)
{
    /* String buffers */
    static char output[MAXMSG];
    char string[MAXMSG];

    /* Read args */
    va_list args;
    va_start(args, str);
    vsnprintf(string, sizeof (string), str, args);
    va_end(args);

    /* Colorize string */
    snprintf(output, sizeof (output), "%s%s%s", clr, string, colorTable[_CLR_RESET]);

    return output;
}

/**
 * \brief Save log in file. Argument aut is string which
 * we want to log. Argument fname is log file path and sdate is
 * SlogDate structure variable, we need it to create filename.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param filestamp - 
 * \param out - 
 * \param fname - 
 * \param sdate - 
 */
void libDbgToFile(int filestamp, char *out, const char *fname, stLibDbgDate_t *sdate)
{
    /* Used variables */
    char filename[PATH_MAX];

    /* Create log filename with date */
    if (filestamp) {
	snprintf(filename, sizeof (filename), "%s-%02d-%02d-%02d.log",
		fname, sdate->year, sdate->mon, sdate->day);
    } else snprintf(filename, sizeof (filename), "%s.log", fname);

    /* Open file pointer */
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) return;

    /* Write key in file */
    fprintf(fp, "%s", out);

    /* Close file pointer */
    fclose(fp);
}

/**
 * \brief 
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param line - 
 * \return pchFirst - 
 */
const char* strToNro(const char* line)
{
    static char* pchFirst;
    char* pchLast;
    pchFirst = (strchr(line, '=') + 1);
    pchLast = (strpbrk(pchFirst, "# ") - 1);

    pchFirst[pchLast - pchFirst] = '\0';
    return pchFirst;
}

/**
 * \brief Retunr string in slog format. Function takes arguments 
 * and returns string in slog format without printing and 
 * saveing in file. Return value is char pointer.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param pDate - 
 * \param msg - 
 * \return output - 
 */
char* libDbgGet(stLibDbgDate_t *pDate, char *msg, ...)
{
    /* Used variables */
    static char output[MAXMSG];
    char string[MAXMSG];

    /* Read args */
    va_list args;
    va_start(args, msg);
    vsnprintf(string, sizeof (string), msg, args);
    va_end(args);

    /* Generate output string with date */
    snprintf(output, sizeof (output), "%02d.%02d.%02d-%02d:%02d:%02d.%02d - %s",
	    pDate->year, pDate->mon, pDate->day, pDate->hour,
	    pDate->min, pDate->sec, pDate->usec, string);

    /* Return output */
    return output;
}

/**
 * \brief 
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param path - 
 * \return S_ISREG - 
 */
int fileExists(const char* path)
{
    struct stat tmpStat;
    stat(path, &tmpStat);
    return S_ISREG(tmpStat.st_mode);
}

/**
 * \brief funcao que obtem os valores das flags para saber o que 
 * fazer com a informaacao recebida
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param sect - 
 * \param cfg - 
 * \return ret - 
 */
int getConfig(const char *sect, Config *cfg)
{    
	int ret = 0;
    //obtem o level de depuracao em console
    if (CONFIG_OK != ConfigReadInt(cfg, sect, "level", (int *)&libDbgFlags.level, 0)) {
    	//printf("libDbgFlags.level: %d\n", libDbgFlags.level);
    	ret = -1;
    }
        
    //obtem o leve de depuracao em arquivo
    if (CONFIG_OK != ConfigReadInt(cfg, sect, "file_level", (int *)&libDbgFlags.file_level, 0)) {
    	//printf("libDbgFlags.file_level: %d\n", libDbgFlags.file_level);
    	ret = -1;
    }
    
    //obtem o valor que habilita ou nao a depuracao em arquivo
    if (CONFIG_OK != ConfigReadInt(cfg, sect, "to_file", (int *)&libDbgFlags.to_file, 0)) {
    	//printf("libDbgFlags.to_file: %d\n", libDbgFlags.to_file);
    	ret = -1;
    }
    
    return ret;
}

/**
 * \brief Log exiting process. Function takes arguments and saves
 * log in file if LOGTOFILE flag is enabled from config. Otherwise
 * it just prints log without saveing in file. Argument level is
 * logging level and flag is slog flags defined in slog.h header.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param location - 
 * \param context - 
 * \param level - 
 * \param flag - 
 * \param msg - 
 */
void libDbg(const char *location, const char* context, int level, int flag, const char *msg, ...)
{

    /* Lock for safe */
    if (td_safe) {
	if (pthread_mutex_lock(&lDbgmutex)) {
	    printf("<%s:%d> %s: [ERROR] Can not lock mutex: %d\n",
		    __FILE__, __LINE__, __FUNCTION__, errno);
	    exit(EXIT_FAILURE);
	}
    }

    /* Used variables */
    LIBDBGDATE_DECLARE(mdate);

    char string[MAXMSG];
    char prints[MAXMSG];
    char color[32], alarm[32];
    char *output;

    libDbgGetDate(&mdate);
    memset(string, 0, sizeof (string));
    memset(prints, 0, sizeof (prints));
    memset(color, 0, sizeof (color));
    memset(alarm, 0, sizeof (alarm));

    /* Read args */
    va_list args;
    va_start(args, msg);
    vsnprintf(string, sizeof (string), msg, args);
    va_end(args);

    /* get configuration de levels */
    if (0 > getConfig(context, gConfig)) {
    	printf("\n\t***\tNao foi encontrado a secao passado como parametro.\t***\n" 
    		"\t\t-->Procurando a secao padrao no arquivo de configuracao\n\n");
    	if (0 > getConfig("CONFIG", gConfig)) {
    		printf("\n\t***\tNao foi encontrado a secao PADRAO no arquivo de configuracao\t***\n\n");
    		return;
    	}
    }

    /* Check logging levels */
    if((!level) || (level <= libDbgFlags.level) || (level <= libDbgFlags.file_level)) {
    //if ((0 <= level) && (level <= libDbgFlags.level)) {
		/* Handle flags */
		switch (flag)
		{
		    case LDBG_LIVE:
			strncpy(color, libDbgGetColor(_CLR_NORMAL), sizeof (color));
			strncpy(alarm, "LIVE", sizeof (alarm));
			break;
		    case LDBG_INFO:
			strncpy(color, libDbgGetColor(_CLR_GREEN), sizeof (color));
			strncpy(alarm, "INFO", sizeof (alarm));
			break;
		    case LDBG_WARN:
			strncpy(color, libDbgGetColor(_CLR_YELLOW), sizeof (color));
			strncpy(alarm, "WARN", sizeof (alarm));
			break;
		    case LDBG_DEBUG:
			strncpy(color, libDbgGetColor(_CLR_BLUE), sizeof (color));
			strncpy(alarm, "DEBUG", sizeof (alarm));
			break;
		    case LDBG_ERROR:
			strncpy(color, libDbgGetColor(_CLR_RED), sizeof (color));
			strncpy(alarm, "ERROR", sizeof (alarm));
			break;
		    case LDBG_FATAL:
			strncpy(color, libDbgGetColor(_CLR_RED), sizeof (color));
			strncpy(alarm, "FATAL", sizeof (alarm));
			break;
		    case LDBG_PANIC:
			strncpy(color, libDbgGetColor(_CLR_WHITE), sizeof (color));
			strncpy(alarm, "PANIC", sizeof (alarm));
			break;
		    case LDBG_NONE:
			strncpy(prints, string, sizeof (string));
			break;
		    default:
			strncpy(prints, string, sizeof (string));
			flag = LDBG_NONE;
			break;
		}

		/* Print output */
		if (level <= libDbgFlags.level || libDbgFlags.pretty) {
		    if (flag != LDBG_NONE) snprintf(prints, sizeof (prints), "[%s] %s",
			    libDbgStrClr(color, alarm), string);
		    if (level <= libDbgFlags.level) printf("[%s] - %s", location, libDbgGet(&mdate, "%s\n", prints));
		}

		/* Save log in file */
		if(libDbgFlags.to_file && (level <= libDbgFlags.file_level)) {
		    if (libDbgFlags.pretty) output = libDbgGet(&mdate, "%s\n", prints);
		    else {
			if (flag != LDBG_NONE) snprintf(prints, sizeof (prints), "[%s] %s", alarm, string);
			output = libDbgGet(&mdate, "%s\n", prints);
		    }

		    /* Add log line to file */
		    libDbgToFile(libDbgFlags.filestamp, output, gFName, &mdate);
		}
    }

    /* Done, unlock mutex */
    if (td_safe) {
	if (pthread_mutex_unlock(&lDbgmutex)) {
	    printf("<%s:%d> %s: [ERROR] Can not deinitialize mutex: %d\n",
		    __FILE__, __LINE__, __FUNCTION__, errno);
	    exit(EXIT_FAILURE);
	}
    }
}

/**
 * \brief Log exiting process. Function takes arguments and saves
 * log in file if LOGTOFILE flag is enabled from config. Otherwise
 * it just prints log without saveing in file. Argument level is
 * logging level and flag is slog flags defined in slog.h header.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param cfg - 
 * \return 
 */
int initConfig(Config *cfg)
{
    char tmpStr[MAXSTR];
    //obtem o nome do arquivo de log a ser gerado
    ConfigReadString(cfg, "CONFIG", "NAMFILE", tmpStr, sizeof (tmpStr), "Poet");
    //printf("NAMFILE: %s\n", tmpStr);
    strncpy(gFName, tmpStr, sizeof (gFName));
    
    //obtem o level de depuracao em console
    ConfigReadInt(cfg, "CONFIG", "level", &libDbgFlags.level, 0);
    //printf("libDbgFlags.level: %d\n", libDbgFlags.level);
    
    //obtem o leve de depuracao em arquivo
    ConfigReadInt(cfg, "CONFIG", "file_level", &libDbgFlags.file_level, 0);
    //printf("libDbgFlags.file_level: %d\n", libDbgFlags.file_level);
    
    //obtem o valor que habilita ou nao a depuracao em arquivo
    ConfigReadInt(cfg, "CONFIG", "to_file", &libDbgFlags.to_file, 0);
    //printf("libDbgFlags.to_file: %d\n", libDbgFlags.to_file);

    //obtem o valor que habilita ou nao a depuracao em arquivo
    ConfigReadInt(cfg, "CONFIG", "pretty", &libDbgFlags.pretty, 0);
    printf("libDbgFlags.pretty: %d\n", libDbgFlags.pretty);

    //obtem o valor que habilita ou nao a depuracao em arquivo
    ConfigReadInt(cfg, "CONFIG", "filestamp", &libDbgFlags.filestamp, 0);
    printf("libDbgFlags.filestamp: %d\n", libDbgFlags.filestamp);

    return 0;
}

/**
 * \brief Initialize slog library. Function parses config file and reads log 
 * level and save to file flag from config. First argument is file name 
 * where log will be saved and second argument conf is config file path 
 * to be parsedand third argument lvl is log level for this message.
 * 
 * \author Lucio Pintanel
 * \date que foi criado 18/10/16 20:49
 * \param fname - 
 * \param conf - 
 * \param lvl - 
 * \param flvl - 
 * \param t_safe - 
 */
void libDbgInit(const char* fname, const char* conf, int lvl, int flvl, int t_safe)
{
    int status = 0;

    /* Init mutex sync */
    if (t_safe) {
		/* Init mutex attribute */
		pthread_mutexattr_t m_attr;
		if (pthread_mutexattr_init(&m_attr) ||
			pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE) ||
			pthread_mutex_init(&lDbgmutex, &m_attr) ||
			pthread_mutexattr_destroy(&m_attr)) {
		    printf("<%s:%d> %s: [ERROR] Can not initialize mutex: %d\n",
			    __FILE__, __LINE__, __FUNCTION__, errno);
		    td_safe = 0;
		}
    }

    
    /* set settings */
    gConfig = ConfigNew();
    ConfigSetBoolString(gConfig, "yes", "no");

    if (!fileExists(CONFIGREADFILE)) {
	printf("Nao existe o arquivo de configuracao - %s\n", CONFIGREADFILE);
	exit(-1);
    }

    /* we can give initialized handle (rules has been set) */
    if (ConfigReadFile(conf, &gConfig) != CONFIG_OK) {
	printf("<%s:%d> %s: [ERROR] ConfigOpenFile failed for %s\n",
		__FILE__, __LINE__, __FUNCTION__, CONFIGREADFILE);
	return;
    }

    ConfigPrintSettings(gConfig, stdout);

    initConfig(gConfig);

    /* Handle config parser status */
    if (!status) libDbg(LOCATION, "CONFIG", 0, LDBG_INFO, "Initializing logger values without config");
    else libDbg(LOCATION, "CONFIG", 0, LDBG_INFO, "Loading logger config from: %s", conf);
}

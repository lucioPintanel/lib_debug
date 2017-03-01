/**
 * @file llogDbg.c
 * @author Lucio Pintanel
 * @date 9 Aug 2016
 * @brief File containing includes, defines, and demias externados.
 *
 */

#ifndef CONFIGINI_H_
#define CONFIGINI_H_

#include <stdio.h>


#ifndef __cplusplus

typedef unsigned char bool;
#undef  false
#define false 	0
#undef  true
#define true	(!false)

#endif


typedef struct Config Config;


#define CONFIG_SECTION_FLAT		NULL	/* config is flat data (has no section) */


/**
 * \brief Return types
 */
typedef enum
{
	CONFIG_OK,                    /* ok (no error) */
	CONFIG_ERR_FILE,              /* file io error (file not exists, cannot open file, ...) */
	CONFIG_ERR_NO_SECTION,        /* section does not exist */
	CONFIG_ERR_NO_KEY,            /* key does not exist */
	CONFIG_ERR_MEMALLOC,          /* memory allocation failed */
	CONFIG_ERR_INVALID_PARAM,     /* invalid parametrs (as NULL) */
	CONFIG_ERR_INVALID_VALUE,     /* value of key is invalid (inconsistent data, empty data) */
	CONFIG_ERR_PARSING,           /* parsing error of data (does not fit to config format) */
} ConfigRet;



#ifdef __cplusplus
extern "C" {
#endif



Config*     ConfigNew              (void);
void        ConfigFree             (Config *cfg);

const char *ConfigRetToString      (ConfigRet ret);

ConfigRet   ConfigRead             (FILE *fp, Config **cfg);
ConfigRet   ConfigReadFile         (const char *filename, Config **cfg);

ConfigRet   ConfigPrint            (const Config *cfg, FILE *stream);
ConfigRet   ConfigPrintToFile      (const Config *cfg, char *filename);
ConfigRet   ConfigPrintSettings    (const Config *cfg, FILE *stream);

int         ConfigGetSectionCount  (const Config *cfg);
int         ConfigGetKeyCount      (const Config *cfg, const char *sect);

ConfigRet   ConfigSetCommentCharset(Config *cfg, const char *comment_ch);
ConfigRet   ConfigSetKeyValSepChar (Config *cfg, char ch);
ConfigRet   ConfigSetBoolString    (Config *cfg, const char *true_str, const char *false_str);

ConfigRet   ConfigReadString       (const Config *cfg, const char *sect, const char *key, char *        val, int size, const char * dfl_val);
ConfigRet   ConfigReadInt          (const Config *cfg, const char *sect, const char *key, int *         val,           int          dfl_val);
ConfigRet   ConfigReadUnsignedInt  (const Config *cfg, const char *sect, const char *key, unsigned int *val,           unsigned int dfl_val);
ConfigRet   ConfigReadFloat        (const Config *cfg, const char *sect, const char *key, float *       val,           float        dfl_val);
ConfigRet   ConfigReadDouble       (const Config *cfg, const char *sect, const char *key, double *      val,           double       dfl_val);
ConfigRet   ConfigReadBool         (const Config *cfg, const char *sect, const char *key, bool *        val,           bool         dfl_val);

ConfigRet   ConfigAddString        (Config *cfg, const char *sect, const char *key, const char  *val);
ConfigRet   ConfigAddInt           (Config *cfg, const char *sect, const char *key, int          val);
ConfigRet   ConfigAddUnsignedInt   (Config *cfg, const char *sect, const char *key, unsigned int val);
ConfigRet   ConfigAddFloat         (Config *cfg, const char *sect, const char *key, float        val);
ConfigRet   ConfigAddDouble        (Config *cfg, const char *sect, const char *key, double       val);
ConfigRet   ConfigAddBool          (Config *cfg, const char *sect, const char *key, bool         val);

bool        ConfigHasSection       (const Config *cfg, const char *sect);

ConfigRet   ConfigRemoveSection    (Config *cfg, const char *sect);
ConfigRet   ConfigRemoveKey        (Config *cfg, const char *sect, const char *key);


#ifdef __cplusplus
}
#endif


#endif /* CONFIGINI_H_ */

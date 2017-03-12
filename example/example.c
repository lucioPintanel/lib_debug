/*
 *  example.c 
 *  Copyleft (C) 2015  Sun Dro (a.k.a 7th Ghost)
 *
 * Source example of slog library usage. Use GCC.
 */

#include <stdio.h>
#include <string.h>
#include "libDbg.h"

extern int teste1();
extern int teste2();

int greet() 
{
    /* Get and print slog version */
    printf("=========================================\n");
    printf("libDbg Version: %s\n", libDbgVersion(0));
    printf("=========================================\n");
    
    return 0;
}

int main()
{
    /* Used variables */
    char char_arg[32];
    int int_arg;

    /* Init args */
    strncpy(char_arg, "test string", strlen("test string"));
    int_arg = 69;

    /* Greet users */
    greet();

    /* 
    * slog_init - Initialise slog 
    * First argument is log filename 
    * Second argument is config file
    * Third argument is max log level on console
    * Fourth is max log level on file   
    * Fifth is thread safe flag.
    */
    libDbgInit("example", "config.cnf", 2, 3, 1);

    /* Log and print something with level 0 */
    libDbg(LOCATION, "CNTXT1", 0, LDBG_LIVE, "Test message with level 0");

    /* Log and print something with level 1 */
    libDbg(LOCATION, "CNTXT2", 1, LDBG_WARN, "Warn message with level 1");

    /* Log and print something with level 2 */
    libDbg(LOCATION, "CNTXT2", 2, LDBG_INFO, "Info message with level 2");
    

    /* Log and print something with level 3 */
    libDbg(LOCATION, "CNTXT1", 3, LDBG_LIVE, "Test message with level 3");

    /* Log and print something with char argument */
    libDbg(LOCATION, "CNTXT1", 0, LDBG_DEBUG, "Debug message with char argument: %s", char_arg);

    /* Log and print something with int argument */
    libDbg(LOCATION, "CNTXT1", 0, LDBG_ERROR, "Error message with int argument: %d", int_arg);

    /* Test log with higher level than log max value 
    * This will never be printed while log level argument is higher than max log level */
    libDbg(LOCATION, "CNTXT2", 4, LDBG_NONE, "[LIVE] Test log with higher level than log max value");

    /* Print something with our own colorized line */
    libDbg(LOCATION, "CNTXT2", 0, LDBG_NONE, "[%s] This is our own colorized string", libDbgStrClr(libDbgGetColor(_CLR_GREEN),"TEST"));

    return 0;
}


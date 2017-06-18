## -*- Makefile -*-
##
## @author: Lucio Pintanel
## @date: 14/02/2017	10:47
##
##  Este Makefile cria o arquivo executavel baseado nos arquivos .c .h 
## existem no diretorio onde se encontra
##

#para evitar problemas em sistemas onde a variável SHELL pode ser herdadas 
#do ambiente.
SHELL=/bin/sh

# parâmetro para habilitar o debug da aplicação.
APP_DEBUG := OFF
#APP_DEBUG := ON

# source files.
SRC = $(wildcard *.c)
 
OBJ = $(SRC:.c=.o)

PATHOUT = ../lib
NAME = libllogDbg.a
OUT = $(PATHOUT)/$(NAME)

# Versao da biblioteca (será passado para o código)
LIBDBGVERSAO_MAX=0
LIBDBGVERSAO_MIN=0
LIBDBGVERSAO_NRO=1
#versao da biblioteca no formato de um inteiro a ser passada para a aplicacao
DEFVRS_LIB = $(LIBDBGVERSAO_MAX)$(LIBDBGVERSAO_MIN)$(LIBDBGVERSAO_NRO)
 
# include directories
INCLUDES = -I../includes/ 
 
# compiler
CCC = gcc
SZ = size
 
# library paths
LIBS = 
 
ifeq ($(APP_DEBUG),ON)
    OPT	    =-O0
    DEBUG   =-g
else
    OPT	    =-O2
    DEBUG   =
endif

# compile flags
CCFLAGS = $(DEBUG) $(OPT) -Wall -std=gnu99 $(INCLUDES) -lparseFile -lpthread
#parametro a ser paasado para a aplicacao em temppo de compilacao
CCFLAGS += -D_VERSAO_MAX=$(LIBDBGVERSAO_MAX) \
			-D_VERSAO_MIN=$(LIBDBGVERSAO_MIN) \
			-D_VERSAO_NRO=$(LIBDBGVERSAO_NRO) \
			-D NAMELIB=\"$(NAME)\" 
 
.SUFFIXES: .c
 
.PHONY: all clean mrproper

all: $(OUT)
 
%.o: %.c
	$(CCC) $(CCFLAGS) -c $< -o $@
 
$(OUT): $(OBJ)
	ar rcs $@ $(OBJ)
ifeq ($(APP_DEBUG),OFF)
	@strip --strip-unneeded $@
	@echo "Stripedded..."
endif
	@$(SZ) $@
 
clean:
	@echo Limpando projeto
	@$(RM) *.o *~ core

mrproper: clean
	@echo Removendo lib
	@$(RM) $(OUT)

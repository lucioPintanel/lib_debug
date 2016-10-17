LIBDBGVERSAO_MAX=0
LIBDBGVERSAO_MIN=0
LIBDBGVERSAO_NRO=1

CFLAGS = -g -O2 -Wall -lpthread -D_VERSAO_MAX=$(LIBDBGVERSAO_MAX) \
	-D_VERSAO_MIN=$(LIBDBGVERSAO_MIN) -D_VERSAO_NRO=$(LIBDBGVERSAO_NRO)

LIB = -lrt
OBJS = llogDebug.o

.c.o:
	$(CC) $(CFLAGS) -c $< $(LIB)

libllogDebug.a: $(OBJS)
	$(AR) rcs libllogDebug.a $(OBJS)
	@echo [-] Syncing static library
	sync

llogDebug.o: llogDebug.h

.PHONY: clean
clean:
	$(RM) libllogDebug.a $(OBJS)


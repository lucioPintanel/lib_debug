LIB = -L ../lib -lllogDbg -lparseFile -lpthread -lrt -I ../includes
CFLAGS = -g -O2 -Wall

example: *.c
	gcc $(CFLAGS) -o $@ $@.c $(LIB)

.PHONY: clean

clean:
	$(RM) example *.log $(OBJ)


CC     = gcc
CFLAGS = -Wall -Werror
TARGET = arena
DEBUG ?= NO
ifeq (${DEBUG}, YES)
DEBUG_FLAGS = -DDEBUG_PRINT_ADDR
endif

O_SRC =

%.o: %.c %.h
	${CC} ${CFLAGS} ${DEBUG_FLAGS} -o $@ -c $<

${TARGET}: ${O_SRC}
	${CC} $^ -o $@

.PHONY: all clean distclean

all: ${TARGET}
clean:
	rm *.o
distclean:
	rm ${TARGET}


SRCS = matrix.c
	
OBJS = ${SRCS:.c=.o}
CC = gcc
CFLAGS = -Wall -Werror -Wextra
LINUX_FLAGS= -lm -lsubunit -lpthread -lrt 
OS:=$(shell uname -s)


all: clean matrix.a

matrix.a: ${SRCS}
	${CC} ${CFLAGS} -c ${SRCS} -lm 
	@ar rcs matrix.a ${OBJS}
	@ranlib matrix.a

clean:
	@/bin/rm -rf *.o *.a test unit-test *.gcno *gcda report *.info leaks


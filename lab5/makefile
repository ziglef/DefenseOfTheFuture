# Makefile for the mv driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab5
SRCS=	lab5.c test5.c mouse.c timer.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>


# Makefile for lab4 drivers
COMPILER_TYPE= gnu

CC=gcc

PROG=	lab4
SRCS=	test4.c lab4.c timer.c kbc.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-ldriver -lsys -lm

LDFLAGS+= -L.

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
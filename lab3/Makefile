# Makefile for lab3 drivers
COMPILER_TYPE= gnu

CC=gcc

PROG=	lab3
SRCS=	speaker.c timer.c lab3.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-ldriver -lsys -lm

LDFLAGS+= -L.

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
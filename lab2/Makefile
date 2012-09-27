# Makefile for lab2 drivers
COMPILER_TYPE= gnu

CC=gcc

PROG=	lab2
SRCS=	lab2.c vbe.c video_gr.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+=	-llm -ldriver -lsys -lm

LDFLAGS+= -L.

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
# Makefile for dotf
COMPILER_TYPE= gnu

CC=gcc

PROG=	dotf
SRCS=	vbe.c video_gr.c kbc.c mouse.c rtc.c speaker.c timer.c dotf.c read_xpm.c sprite.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+=	-llm -ldriver -lsys -lm

LDFLAGS+= -L.

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
# Makefile

CC = gcc
CPPFLAGS = -MMD -D_XOPEN_SOURCE=500
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS =
LDLIBS =

all: solver

SRC = solver.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} ${DEP}

# END

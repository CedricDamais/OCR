# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm
LDLIBS = -lm

SRC = XorNeural.c MathTools.c Persist.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main


main: ${OBJ}
		$(CC) -o main $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program

-include ${DEP}

# END

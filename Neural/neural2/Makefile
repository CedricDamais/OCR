# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -D__NO_INLINE__ -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -std=c99 -O2 -fsanitize=address -g
LDFLAGS = -lm -ldl
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = Neural.c MathTools.c Persist.c DataLoad.c
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

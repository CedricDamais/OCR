# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -D__NO_INLINE__ -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -lm -ldl
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

all: grid_creation

SRC = ./grid_creation.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

grid_creation : ${OBJ}
		$(CC) -o grid_creation $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END

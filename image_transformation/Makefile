# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -lm -ldl
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

all: main

SRC = image_rotation.c hough_transform.c image_processing.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

image_processing : ${OBJ}
	·$(CC) -o image_processing $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END

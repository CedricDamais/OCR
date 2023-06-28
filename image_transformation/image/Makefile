# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -D__NO_INLINE__ -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -fsanitize=address -g 
LDFLAGS = -lm -ldl
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

all: image_processing

SRC = image_processing.c image_rotation.c hough_transform.c image_resize.c binarization.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

image_processing : ${OBJ}
		$(CC) -o image_processing $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END

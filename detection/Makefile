# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -D__NO_INLINE__ -Wall -Wextra -O0 -g -lm `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = -lm `pkg-config --libs sdl2 SDL2_image`

SRC = image_processing.c processing.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $< $(LDLIBS) $(LDFLAGS)

image_processing: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) $(LDFLAGS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} ${EXE}
	${RM} cases/*.png

# END

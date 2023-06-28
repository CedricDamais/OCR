CC = gcc
CPPFLAGS =
CFLAGS = -D__NO_INLINE__ -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS = -lm -ldl
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

SOURCE_DIR := .
SRC =  main.c grid_extract.c grid_numbers_extraction.c row_extract.c utils.c ../image_transformation/image/image_processing.c ../image_transformation/image/binarization.c ../detection/processing.c ../image_transformation/image/image_rotation.c ../image_transformation/image/image_resize.c ../image_transformation/image/hough_transform.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
EXE = ${SRC:.c=}

all: main

main : ${OBJ}
		$(CC) -o main $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)
clean:
	${RM} ${DEP}
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} main
# END
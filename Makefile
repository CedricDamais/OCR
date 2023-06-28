CC = gcc
CPPFLAGS =
CFLAGS = -D__NO_INLINE__ -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lm -g
LDFLAGS = -ldl
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

SOURCE_DIR := .
SRC =  ./image_transformation/image/binarization.c ./image_transformation/image/image_processing.c ./image_transformation/image/image_rotation.c ./image_transformation/image/image_resize.c ./image_transformation/image/hough_transform.c ./detection/processing.c ./Neural/neural2/MathTools.c ./Neural/neural2/Persist.c ./Neural/neural2/DataLoad.c ./Neural/neural2/Neural.c ./sudoku_solver/solver.c ./grid_creation/grid_creation.c main.c ./grid_extraction/utils.c ./grid_extraction/grid_extract.c ./grid_extraction/row_extract.c ./grid_extraction/grid_numbers_extraction.c
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
	${RM} sobel.png
	${RM} binarized.png
	${RM} sudoku_grid
	${RM} sudoku_grid.result
	${RM} z.png
	${RM} 01.png
	${RM} 02.png
	${RM} 03.png
	${RM} 04.png
	${RM} 05.png
	${RM} 06.png
	${RM} 07.png
	${RM} 08.png
	${RM} 09.png
	${RM} last_process.png
	${RM} c_image.jpeg


# END

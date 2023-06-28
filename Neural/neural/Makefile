# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -fsanitize=address -g
LDFLAGS = -lm
LDLIBS = -lm

SOURCE_DIR := .
SRC = Neural.c Train.c
OBJ = ${SRC:.c=.o} 
DEP = ${SRC:.c=.d}

all: alt main
alt:
		$(CC) -c -fsanitize=address -g $(SOURCE_DIR)/Matrix_tools/Matrix.c
		$(CC) -c -fsanitize=address -g $(SOURCE_DIR)/MathTools/MathTools.c
		$(CC) -c -fsanitize=address -g $(SOURCE_DIR)/Layers/Layer.c

main: ${OBJ} Layer.o MathTools.o Matrix.o
		
		$(CC) -o main $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ} Layer.o MathTools.o Matrix.o   # remove object files
	${RM} ${DEP} Layer.d MathTools.d Matrix.d  # remove dependency files
	${RM} main     # remove main program

-include ${DEP}

# END

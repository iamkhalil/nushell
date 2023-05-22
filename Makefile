CC= gcc
CFLAGS= -g -Wall -Wextra -Werror -pedantic
TARGET=nushell

SRC_DIR= ./src
SRC= $(wildcard $(SRC_DIR)/*.c)
OBJ= $(SRC:.c=.o)

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

memcheck: all
	@make -s all
	valgrind ./${TARGET}

help:
	@echo "List of available options:"
	@echo " * all - build the project"
	@echo " * memcheck - check memory leaks using Valgrind"
	@echo " * clean - delete build files"

clean:
	rm -f $(TARGET)

.PHONY: all clean memcheck help

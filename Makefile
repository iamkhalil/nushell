CC= gcc
CFLAGS= -g -Wall -Wextra -Werror -pedantic
TARGET=nushell

SRC_DIR= ./src
TEST_DIR= ./test
SRC= $(wildcard $(SRC_DIR)/*.c)
OBJ= $(SRC:.c=.o)

.ONESHELL:

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

memcheck: all
	@make -s all
	valgrind ./${TARGET}

help:
	@echo "List of available options:"
	@echo " * all - build the project"
	@echo " * test - run the test suites"
	@echo " * memcheck - check memory leaks using Valgrind"
	@echo " * clean - delete build files"

test: all
	@make -s all
	cd ${TEST_DIR}
	./run.sh

clean:
	rm -f $(TARGET)

.PHONY: all clean memcheck help test

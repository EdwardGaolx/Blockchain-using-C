CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -Wno-missing-braces -Wno-unused-function
LFLAGS  = -l
BUILD = extension_test

.SUFFIXES: .c .o

.PHONY: all clean

all: $(BUILD)

extension_test: extension_test.o sha256.o blockchain.o block.o proof_of_work.o utilities.o

blockchain.o: blockchain.h block.h utilities.h

block.o: block.h sha256.h proof_of_work.h

sha256.o: sha256.h

proof_of_work.o: proof_of_work.h sha256.h

extension_test.o: extension_test.c sha256.h blockchain.h utilities.h

utilities.o: utilities.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f lexer.c
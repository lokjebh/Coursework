###############################################
# Makefile                                    #
# Author: jhui94                              #
#                                             #
# To compile, run 'make'                      #
# To run a specific target, run 'make TARGET' #
# To remove generated files, run 'make clean' #
###############################################
CC       = gcc
CFLAGS   = -g -I include
LDFLAGS  = -L lib
BINARY   = mjava
OBJFILES = test1.mclass test2.mclass test3.mclass test4.mclass test5.mlass test6.mclass test8.mclass 

all      : $(BINARY)

mjava    : minijvm.o
	$(CC) $(LDFLAGS) -o $@ $< -ladt

minijvm.o: minijvm.c include/minijvm.h include/stack.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(BINARY) minijvm.o

test: test1 test2 test3 test4 test5 test6 test7 test8
	-$(make) $@

test1:
	-./$(BINARY) testfiles/test1

test2:
	-./$(BINARY) testfiles/test2

test3:
	-./$(BINARY) testfiles/test3

test4:
	-./$(BINARY) testfiles/test4

test5:
	-./$(BINARY) testfiles/test5

test6:
	-./$(BINARY) testfiles/test6

test7:
	-./$(BINARY) testfiles/test7

test8:
	-./$(BINARY) testfiles/test8


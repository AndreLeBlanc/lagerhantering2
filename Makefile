CC= gcc
OBJECTSMAIN= main.o bst.o helpers.o pallet.o palletui.o storage.o storageui.o alloc_check.o
#CFLAGS= -ggdb -std=c11 -c -Wall
#LDFLAGS= -ggdb
CFLAGS= -ggdb -std=c11 -c -pg -Wall -fprofile-arcs -ftest-coverage
LDFLAGS= -ggdb -pg -fprofile-arcs -ftest-coverage
TESTCF= -std=c11 -c -Wall -fprofile-arcs -ftest-coverage
TESTLF= -fprofile-arcs -ftest-coverage
#TESTCF= -std=c11 -c -pg -Wall -fprofile-arcs -ftest-coverage
#TESTLF= -pg -fprofile-arcs -ftest-coverage

notest: main

all: main tester				

clean:
	rm -f main $(OBJECTSMAIN)
	rm -f *.gcda *.gcno gmon.out
	rm -f bst_test storage_test

## main program

main: $(OBJECTSMAIN)
	$(CC) $(LDFLAGS) $^ -o $@

alloc_check.o: alloc_check.c alloc_check.h
	$(CC) $(CFLAGS) alloc_check.c

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) bst.c

helpers.o: helpers.c helpers.h storage.h
	$(CC) $(CFLAGS) helpers.c

main.o: main.c storage.h storageui.h
	$(CC) $(CFLAGS) main.c

pallet.o: pallet.c pallet.h bst.h helpers.h
	$(CC) $(CFLAGS) pallet.c

palletui.o: palletui.c palletui.h pallet.h helpers.h
	$(CC) $(CFLAGS) palletui.c

storage.o: storage.c storage.h bst.h helpers.h
	$(CC) $(CFLAGS) storage.c

storageui.o: storageui.c storageui.h storage.h palletui.h helpers.h
	$(CC) $(CFLAGS) storageui.c

## tests
test: bst_test storage_test
	./bst_test
	./storage_test

bst_test: bst_test.o bst.o alloc_check.o
	$(CC) $(TESTLF) $^ -lcunit -o $@

bst_test.o: bst_test.c bst.h alloc_check.h
	$(CC) $(TESTCF) bst_test.c

storage_test: storage_test.o storage.o bst.o helpers.o alloc_check.o
	$(CC) $(TESTLF) $^ -lcunit -o $@

storage_test.o: storage_test.c storage.h bst.h helpers.h alloc_check.h
	$(CC) $(TESTCF) storage_test.c

# Target/målfil: dependencies/beroenden
# <tab>kommando1 (fler komm. = fler rader)
#
# 1:a target körs när endast "make" används							
#'all: main' kör raden för målfilen 'main'
# 'make main' från bash gör samma sak
# $(<makronamn>) för makron
# $^ = alla dependencies
# $@ = target
# % = wildcard
#
# Gör targets för alla .o filer:
#%.o: %.c
#	$(CC) $(FLAGS) $^ -c


##
## Make file
##

CC=gcc
OBJ=*.o
SRC=header.c header.h
PROG=header

$(OBJ):	$(SRC)
	$(CC) `pkg-config --cflags glib-2.0` -c $(SRC) 

header: $(OBJ) lib
	$(CC) `pkg-config --cflags --libs glib-2.0` libheader.a header_main.c -o header_main

lib:	$(OBJ)
	ar cr libheader.a header.o

run: 	clean header
	./$(PROG)_main
	@echo "Printing file data:" && echo "" 
	hexdump file.pak

clean:
	-rm -f *.o file.pak header
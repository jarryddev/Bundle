##
## Make file
##

CC=gcc
OBJ=*.o
SRC=header.c header.h
PROG=header

$(OBJ):	$(SRC)
	$(CC) `pkg-config --cflags glib-2.0` -c $(SRC) 

header: $(OBJ)
	$(CC) `pkg-config --cflags --libs glib-2.0` $(PROG).o -o $(PROG) 

lib:	$(OBJ)
	ar rc libheader.a header.o

run: 	clean header
	./$(PROG)
	@echo "Printing file data:" && echo "" 
	hexdump file.pak

clean:
	-rm -f *.o file.pak header
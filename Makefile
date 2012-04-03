##
## Bundler Makefile
##
## 
##

OBJ=BundleHandler.o
SRC=BundleHandler.c
LIB=lib/
BIN=bin/

$(OBJ): $(SRC) header_lib
	gcc -c BundleHandler.c -Iheader/

run: $(OBJ)
	gcc -o BundleHandler BundleHandler.c header/libheader.a -lz 

header_lib: 
	cd header && make lib

test: header_lib
	gcc -o BundleHandler BundleHandler.c -lheader -Lheader/ -Iheader/ `pkg-config --cflags --libs glib-2.0` -lz		
	./BundleHandler test.jpg test.pak
	hexdump test.pak

clean:
	cd header && make clean
	rm BundleHandler.o BundleHandler *~
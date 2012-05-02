##
## Make file
##

CC=gcc
OBJ=*.o

BIN_DIR=../bin
HEADER_OBJ=header.o
HASH_OBJ=hash.o
HASH_SRC=hash.c hash.h
HEADER_SRC=header.c header.h
MMAP_SRC=BundleMemoryMapping.c BundleMemoryMapping.h
MMAP_OBJ=BundleMemoryMapping.o
BUNDLE_SRC=BundleHandler.c BundleHandler.h
PROG= BundleHandler
LIB_DIR=../lib
CC_FLAGS=-L../lib
LINK_FLAGS= -lz 


git_push:
	git push git@github.com:jarryddev/Bundle.git

$(HEADER_OBJ): 
	$(CC) $(CC_FLAGS) -c $(HEADER_SRC)

$(OBJ):	$(HEADER_OBJ) header_lib
	$(CC) -c $(BUNDLE_SRC) 

$(HASH_OBJ): header_lib
	$(CC) $(CC_FLAGS) -c $(HASH_SRC)

$(MMAP_OBJ): 
	$(CC) $(CC_FLAGS) -c $(MMAP_SRC)

install: $(OBJ)
	$(CC) $(OBJ) $(LINK_FLAGS) -o $(BIN_DIR)/bundle

lib:	$(OBJ) header_lib
	ar cr $(LIB_DIR)/libbundler.a bundler.o  

header: header_lib
	$(CC) $(LIB_DIR)/libheader.a header_main.c -o $(BIN_DIR)/header_main -lheader $(CC_FLAGS)

header_lib: $(HEADER_OBJ)
	ar cr $(LIB_DIR)/libheader.a header.o

mmap_lib: $(MMAP_OBJ)
	ar cr $(LIB_DIR)/libbundlemmap.a $(MMAP_OBJ)

hash_lib: $(HASH_OBJ)
	ar cr $(LIB_DIR)/libhash.a hash.o 

integrate: header_lib hash_lib mmap_lib
	$(CC) -o $(BIN_DIR)/integration -L$(LIB_DIR) -lhash -lheader -lbundlemmap integration.c

run: 	clean header
	./$(PROG)_main
	@echo "Printing file data:" && echo "" 
	hexdump file.pak

clean:
	-rm -f *.o *.a bundle $(LIB_DIR)/*.a *.gch *~
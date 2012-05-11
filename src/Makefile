# Copyright (c) <2012>, <Jarryd Hall, Taher Odeh>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met: 
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer. 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those
# of the authors and should not be interpreted as representing official policies, 
# either expressed or implied, of the FreeBSD Project.

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
GLOBAL_BIN_DIR=/usr/bin

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

bundle: $(OBJ)
	$(CC) $(OBJ) $(LINK_FLAGS) -o $(BIN_DIR)/bundle

bundle_install: 
	cp $(BIN_DIR)/bundle $(GLOBAL_BIN_DIR)/bundle
	make bundle_lib
	cp $(LIB_DIR)/libheader.a /usr/lib/libheader.a

bundle_lib:	$(OBJ) header_lib
	ar cr $(LIB_DIR)/libbundle.a bundle.o  

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
clean:
	-rm -f *.o *.a bundle $(LIB_DIR)/*.a *.gch *~
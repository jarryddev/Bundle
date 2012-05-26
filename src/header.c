/*
 
 Copyright (c) <2012>, <Jarryd Hall, Taher Odeh>
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met: 
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer. 
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution. 
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies, 
 either expressed or implied, of the FreeBSD Project.
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <sys/types.h>
#include <stdio.h>
#include "types.h"


/*void bin_to_int(char *str){
 int d=0;
 for (d=0; d< sizeof(int); d++){
 char *b= str << d;
 if ((*b & 1) == 1) fprintf(stderr, "1");
 else fprintf(stderr, "1");
 
 }
 }
 */
/*int main(void){
 
 char *filename = "file.pak";
 printf("endianness big?: %d\n", is_big_endian_machine());
 FILE *fh = fopen(filename, "w+b");
 
 header_init(fh,3);
 
 //  header_update_head(fh, 12);
 offset_p offp1 = malloc(HEADER_OFFSET_SIZE);
 offp1->hash = 0xFFFFFFFC;
 offp1->size = 0xBBBBBBBBBBBBBBBC;
 offp1->offset_start= 0xCCCCCCCCCCCCCCCC;
 offset_p offp2 = malloc(HEADER_OFFSET_SIZE);
 offp2->hash = 0xAAAAAAAC;
 offp2->size = 0xDDDDDDDDDDDDDDDC;
 offp2->offset_start= 0xEEEEEEEEEEEEEEEE;
 
 offset_p offp3 = malloc(HEADER_OFFSET_SIZE);
 offp2->hash = 0xAAAAAAfC;
 offp2->size = 0xDDDDDDDDDDDDDDfC;
 offp2->offset_start= 0xEEEEEEEEEfEEEEEE;
 
 
 header_write_offset(fh, offp1,0);
 header_write_offset(fh, offp2,1);
 header_write_offset(fh, offp3,2);
 
 
 free(offp1); free(offp2); free(offp3);
 
 
 unsigned int n_files = 0;
 n_files= header_get_head(fh);
 fprintf(stderr, "Number of files: %d\n", n_files);
 
 
 offset_p* offx;
 offx= header_get_offsets(fh);
 int i=0;
 for (i=0; i< n_files; i++){
 print_offset(offx[i]);
 }
 
 free(offx);
 
 header_close(fh);
 //  free(offp);
 }
 */


int is_big_endian_machine(void)
{
    unsigned tst = 1;
    int stat = (*(unsigned char*)&tst) != 1;
    return stat;
}


FILE *header_file_open(char *pkg_file){
    FILE *fh;
    // open file with binary/read mode
    if ( (fh = fopen(pkg_file, "rb+")) == NULL){
        perror("fopen");
        return NULL; // todo: ...
    }
    
    return fh;
}

/*
 * returns a pointer to a new struct of the offset
 * existing according to the hash key passed.
 */
offset_p header_read_offset(FILE *fh, hash_size hash){
    unsigned int i,num_files = header_get_head(fh);
    
    fseek(fh, sizeof(unsigned int), SEEK_SET);
    char *buff = NULL;
    offset_p offp = malloc( HEADER_OFFSET_SIZE );
    
    for (i=0;i<num_files;i++){
        buff = malloc( HEADER_OFFSET_SIZE );
        
        fread(buff, HEADER_OFFSET_SIZE, 1, fh);
        memcpy(&offp->hash        , buff                            , sizeof(hash_size));
        memcpy(&offp->offset_start, buff+sizeof(hash_size)              , sizeof(long int));
        memcpy(&offp->size        , buff+sizeof(hash_size)+sizeof(long int), sizeof(size_t));
        memcpy(&offp->compressed   , buff+sizeof(hash_size)+sizeof(long int)+sizeof(size_t), sizeof(char));
        free(buff);
        buff= NULL;
        if (offp->hash == hash) return offp;
    }
    
    return NULL;
}

offset_p get_vm_offst(FILE *fh, hash_size hash, long int vm_address){
    offset_p offp = header_read_offset(fh, hash);
    if (offp == NULL) return NULL;
    
    offp->offset_start+= vm_address;
    return offp;
}

header_offset to_offset(hash_size hash, size_t filesize){
    header_offset of;
    of.hash= hash;
    of.size= filesize;
    return of;
}

void header_write_offset(FILE *fh, offset_p offp, unsigned int index){
    char *buff = malloc(HEADER_OFFSET_SIZE);
    bzero(buff, HEADER_OFFSET_SIZE);
    
    printf("wrtingf now\n");
    print_offset(offp);

    // fill up buffer
    memcpy(buff, (void *)&offp->hash, sizeof(hash_size));
    memcpy(buff + sizeof(hash_size), (void *)&offp->offset_start, sizeof(long int));
    memcpy(buff + sizeof(hash_size) + sizeof(long int), (void*)&offp->size, sizeof(size_t));
    memcpy(buff + sizeof(hash_size) + sizeof(long int) + sizeof(size_t), (void *)&offp->compressed, sizeof(char));

    // jump to end of header  
    fseek(fh, sizeof(unsigned int) + (index*HEADER_OFFSET_SIZE), SEEK_SET); /* todo: err check */
    
    fwrite(buff, HEADER_OFFSET_SIZE, 1, fh); // write offset
    
    // free buffer
    
    free(buff);
    buff= NULL;
}


void small_to_big_endian_int(char **bin, unsigned int size){
    int i;
    char *temp = malloc( sizeof(char) * size);
    for (i=size-1; i >= 0; i--){
        temp[size-i] = (*bin)[i];
    }
    memcpy(bin, &temp, sizeof(char) * size);
    
    free(temp);
    temp= NULL;
}

unsigned int header_init(FILE *fh, unsigned int n_files){
    size_t header_size;
    char *buff;
    
    // set to file handler to ancor
    fseek(fh, 0, SEEK_SET);
    
    // write sum of files to beginning of file
    fwrite(&n_files, sizeof(unsigned int), 1, fh);
    
    //write rubbish until end of header.
    header_size=n_files * HEADER_OFFSET_SIZE + sizeof(unsigned int);
    
    if ((buff = malloc(header_size))==NULL){
        perror("malloc");
        exit(1);
    }
    
    bzero(buff, header_size);
    
    fseek(fh, sizeof(unsigned int), SEEK_SET); // skip header count
    
    fwrite(&buff, header_size, 1, fh);
    return header_size;
}

void print_offset(offset_p off){
    printf("OFFSET: ---- \n size: %p, offset: %p, hash: %p, compressed: %d\n", (size_t)off->size, (long int)off->offset_start, (hash_size)off->hash, (int)off->compressed);
}

unsigned int header_get_head(FILE *fh){
    unsigned int *c = malloc(sizeof(int)), r;
    
    fseek(fh, 0L, SEEK_SET);
    
    if (c == NULL){
        perror("malloc");
        return -1;
    }
    
    int len= fread(c, sizeof(int), 1, fh);
    
    if (len<1) {
        printf("head not initialized!\n");
        return -1;
    }
    r= *c;
    free(c);
    c=NULL;
    return r;
}

void header_close(FILE* fh){
    close(fh);
}

offset_p* header_get_offsets(FILE *fh){
    unsigned int n_files=  header_get_head(fh),i,len;
    offset_p *offsets;
    char *buff;
    size_t s= HEADER_OFFSET_SIZE;
    
    fseek(fh, sizeof(unsigned int), SEEK_SET);
    
    if ((buff=malloc( s*n_files)) == NULL){
        perror("malloc");
        return NULL;
    }
    
    if ((len = fread(buff, sizeof(header_offset),n_files, fh)) == -1){
        perror("fread");
        return NULL;
    }
    
    /*
     if ((offsets=calloc(n_files, sizeof(header_offset))) == NULL){
     perror("calloc");
     return NULL;
     }  
     */
    
    if ((offsets = (offset_p*)malloc(n_files * sizeof(offset_p))) == NULL){
        perror("malloc");
        return NULL;
    }
    
    for (i=0; i< n_files; i++){
        
        offsets[i] = malloc( sizeof(header_offset) );
        
        memcpy(&(*offsets[i]).hash, buff, sizeof(hash_size));
        buff+= sizeof(hash_size);
        memcpy(&(*offsets[i]).offset_start, buff, sizeof(long int));
        buff+= sizeof(long int);
        memcpy(&(*offsets[i]).size, buff, sizeof(size_t));
        buff+=sizeof(size_t);
        memcpy(&(*offsets[i]).compressed, buff, sizeof(char));
        buff+=sizeof(char);
    }
    
    buff-= n_files*(sizeof(size_t)+sizeof(long int)+sizeof(hash_size)+sizeof(char));
    free(buff);
    buff=NULL;
    
    return offsets;
}






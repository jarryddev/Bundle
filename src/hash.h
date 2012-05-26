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

#ifndef _HASH_INCLUDED

#define _HASH_INCLUDED
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t hash_size;

struct hashnode_s {
  hash_size key;
  void *data;
  struct hashnode_s *next;
};

typedef struct hashtbl {
  hash_size size;
  struct hashnode_s **nodes;
  hash_size (*hashfunc)(const char *);
} HASHTBL;


HASHTBL* bundle_hash_init(const char *);
HASHTBL* hashtbl_create(hash_size size, hash_size (*hashfunc)(const char *));
void hashtbl_destroy(HASHTBL *hashtbl);
int hashtbl_insert(HASHTBL *hashtbl, hash_size key, void *data);
int hashtbl_remove(HASHTBL *hashtbl, hash_size key);
void *hashtbl_get(HASHTBL *hashtbl, hash_size key);
int hashtbl_resize(HASHTBL *hashtbl, hash_size size);
hash_size def_hashfunc(const char *key);
#endif

/**
 *
 * Hash table (khash) of filenames=>offsets
 *
 **/

/* INTERFACE FUNCTIONS  */

/**
 * Returns the offset pointer corresponding to filename as
 * the key, or NULL of key does not exist
 */
//offset_p get_offset(char *);

/**
 * Initialize a hashtable, passing a pak filename to extract the header offsets from
 * and create a hashmap of [hash of filename]=>[offset].
 *
 */
//int hash_init(char *);


/**
 * Output the elements in the hash table.
 */
//void hash_read();

//void hash_destroy();

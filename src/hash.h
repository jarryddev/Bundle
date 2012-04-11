#ifndef _HEADER_INCLUDED
#include "header.h"
#endif

/**
*
* Hash table (khash) of filenames=>offsets
*
**/

// INTERFACE FUNCTIONS 


/**
 * Returns the offset pointer corresponding to filename as
 * the key, or NULL of key does not exist
 */
offset_p get_offset(char *);

/**
 * Initialize a hashtable, passing a pak filename to extract the header offsets from
 * and create a hashmap of [hash of filename]=>[offset].
 *
 */
int hash_init(char *);


/**
 * Output the elements in the hash table.
 */
void hash_read();

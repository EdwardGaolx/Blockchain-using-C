#include <stdint.h>

#define HASH_BYTE_SIZE 64

/* returns the hash generated from data using the SHA256 algorithm */
extern char* generateHash(char *msg);

extern char* generateHashForBlock(char *prevHash, char *data);

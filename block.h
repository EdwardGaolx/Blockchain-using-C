#ifndef BLOCK_H
#define BLOCK_H

#include "sha256.h"
#include "proof_of_work.h"
#include <stdio.h>
#include <stdlib.h>
#define EXIT_FAILURE 1
#define MALLOC_ERROR "Memory allocation error"
#define NULL_ERROR "Null pointer"
#define HASH_SIZE 64
#define NULL_CHECK(ptr, msg) \
    if (!ptr)                \
    {                        \
        perror(msg);         \
        exit(EXIT_FAILURE);  \
    }

typedef struct Block {
  struct Block *next;
  struct Block *prev;
  char *hash;
  char *prev_hash;
  char *data;
  int proof;
  int prev_proof;
} Block_t;

/* creates new heap-allocated block with data and proof */
extern Block_t *createBlock(char *data, int proof);

/* updates the block's prev_hash and hash according to its previous block */
extern void updateHash(Block_t *block);

/* updates the block's prev_proof and proof according to its previous block */
extern void updateProof(Block_t *block);

/* frees the memory allocated to block */
extern void freeBlock(Block_t *block);

#endif

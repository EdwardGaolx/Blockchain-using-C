#include "block.h"
#include "stdbool.h"

typedef struct Blockchain {
  Block_t *head;
  Block_t *tail;
  int size;
} Blockchain_t;

/* creates a heap-allocated blockchain with head, tail, and size initialized */
extern Blockchain_t *createChain(void);

/* 
  generates new proof of work and creates a new block with the proof and data, 
  appends the block to chain, and updates the chain's size; returns true if 
  successful
*/
extern bool addBlock(Blockchain_t *chain, char *data);

/* prints the full chain */
extern void printFullChain(Blockchain_t *chain);

/* returns the size of the chain */
extern int getSize(Blockchain_t *chain);

/* 
  modifies the (block_no)th block's data and updating its hash; returns false if
  block_no is an invalid index 
*/
extern bool modifyBlock(Blockchain_t *chain, int block_no, char *data);

/*
  traverses the chain and returns false if any block_n's hash or proof does not
  match block_n+1's prev_has or prev_proof
*/
extern bool checkConsistency(Blockchain_t *chain);

/*
  traverses the chain and update each block's prev_hash, prev_proof and proof 
  according to the previous block's hash and proof
*/
extern void recalculateChain(Blockchain_t *chain);

/* frees the space allocated to chain */
extern void freeChain(Blockchain_t *chain);
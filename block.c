#include "block.h"

Block_t *createBlock(char *data, int proof)
{
    NULL_CHECK(data, NULL_ERROR);

    Block_t *block = malloc(sizeof(Block_t));
    NULL_CHECK(block, MALLOC_ERROR);

    block->data = data;
    block->proof = proof;
    return block;
}

void updateHash(Block_t *block)
{
    NULL_CHECK(block, NULL_ERROR);

    block->hash = generateHashForBlock(block->prev->hash, block->data);
    block->prev_hash = block->prev->hash;
}

void updateProof(Block_t *block)
{
    NULL_CHECK(block, NULL_ERROR);

    block->proof = generateProof(block->prev->proof, block->prev->hash, NORMALDIFF);
    block->prev_proof = block->prev->proof;
}

void freeBlock(Block_t *block)
{
    NULL_CHECK(block, NULL_ERROR);
    free(block);
}
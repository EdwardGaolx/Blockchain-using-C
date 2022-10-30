#include "blockchain.h"
#include "utilities.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Blockchain_t *createChain(void)
{
    Blockchain_t *chain = malloc(sizeof(Blockchain_t));
    NULL_CHECK(chain, MALLOC_ERROR);
    return chain;
}

bool addBlock(Blockchain_t *chain, char *data)
{
    NULL_CHECK(chain, NULL_ERROR);
    NULL_CHECK(data, NULL_ERROR);

    Block_t *newblock = createBlock(data, 0);
    newblock->prev = chain->tail;

    if (newblock->prev != NULL)
    {
        updateHash(newblock);
        updateProof(newblock);
        chain->tail->next = newblock;
    }
    else
    {
        chain->head = newblock;
        newblock->prev = NULL;
        newblock->prev_hash = generateHash(randomString(64));
        newblock->hash = generateHashForBlock(newblock->prev_hash, data);
        newblock->prev_proof = rand();
        newblock->proof = generateProof(newblock->prev_proof, newblock->prev_hash, NORMALDIFF);
    }

    chain->tail = newblock;
    chain->size++;
    return true;
}

void printFullChain(Blockchain_t *chain)
{
    printf("======PRINTING FULL CHAIN======\n");
    NULL_CHECK(chain, NULL_ERROR);
    if (!chain->size)
    {
        return;
    }

    Block_t *curr = chain->tail;
    int currBlockNum = chain->size - 1;
    while (curr != NULL)
    {
        printf(" %c", '\n');
        printf("block: %d", currBlockNum);
        printf(" %c", '\n');
        printf("Data: %s", curr->data);
        printf(" %c", '\n');
        printf("Hash: %s", curr->hash);
        printf(" %c", '\n');
        printf("Previous hash: %s", curr->prev_hash);
        printf(" %c", '\n');
        printf("Proof: %d", curr->proof);
        printf(" %c", '\n');
        printf("Previous proof: %d\n", curr->prev_proof);
        curr = curr->prev;
        currBlockNum--;
    }
}

int getSize(Blockchain_t *chain)
{
    return chain->size;
}

bool modifyBlock(Blockchain_t *chain, int block_no, char *data)
{
    NULL_CHECK(chain, NULL_ERROR);
    NULL_CHECK(data, NULL_ERROR);
    if (chain->size < block_no)
    {
        return false;
    }

    Block_t *curr = chain->head;
    for (int i = 0; i < block_no; i++)
    {
        curr = curr->next;
    }
    curr->data = data;
    return true;
}

bool checkConsistency(Blockchain_t *chain)
{
    NULL_CHECK(chain, NULL_ERROR);

    printf("======CHAIN CONSISTENCY CHECK======\n");

    Block_t *curr = chain->tail;
    int blockNum = chain->size - 1;
    while ((blockNum > 1) && (curr->prev != NULL))
    {

        char *prevData = (curr->prev)->data;
        char *prevHash = (curr->prev)->prev_hash;
        //char *currData = curr->data;
        char *currPrevHash = curr->prev_hash;

        char *calculatedHash = generateHashForBlock(prevHash, prevData);

        if (strcmp(calculatedHash, currPrevHash) != 0)
        {
            printf("CHECK FAILS! Block %d hash inconsistent\n", blockNum - 1);
            return false;
        }

        int prevProof = curr->prev->prev_proof;

        int currPrevProof = curr->prev_proof;

        int calculatedProof = generateProof(prevProof, prevHash, NORMALDIFF);

        if (currPrevProof != calculatedProof)
        {
            printf("CHECK FAILS! Block %d proof inconsistent\n", blockNum - 1);
            return false;
        }
        
        free(calculatedHash);
        curr = curr->prev;
        blockNum--;
    }
    printf("CHECK SUCCEEDS! All blocks healthy\n");
    return true;
}

void recalculateChain(Blockchain_t *chain)
{
    NULL_CHECK(chain, NULL_ERROR);

    Block_t *curr = chain->head;
    for (int i = 0; i < chain->size; i++)
    {
        if (i == 0)
        {
            curr->prev_hash = generateHash(randomString(64));
            curr->hash = generateHash(curr->data);
            curr->prev_proof = rand();
            curr->proof = generateProof(curr->prev_proof, curr->prev_hash, NORMALDIFF);
        }
        else
        {
            updateHash(curr);
            updateProof(curr);
        }
        curr = curr->next;
    }
}

void freeChain(Blockchain_t *chain)
{
    NULL_CHECK(chain, NULL_ERROR);

    Block_t *curr = chain->head;
    while (curr != NULL)
    {
        Block_t *next = curr->next;
        freeBlock(curr);
        curr = next;
    }
    free(chain);
}
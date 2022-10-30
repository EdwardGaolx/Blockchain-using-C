#include <stdio.h>
#include "proof_of_work.h"


int generateProof(int prevProof, char *prevHash, int difficulty) {

    int proof = 0;
    while (!isValidProof(proof, prevProof, prevHash, difficulty)) {
        proof ++;
    }

    return proof;
}

bool isValidProof(int proof, int prevProof, char *prevHash, int difficulty) {
    char buff[2 * PROOF_SIZE + 1];

    sprintf(buff, "%d",proof);

    char *last_proof_string = (char *)malloc(PROOF_SIZE*sizeof(int));
    sprintf(last_proof_string, "%d",prevProof);
    strncat(buff, last_proof_string, PROOF_SIZE);
    free(last_proof_string);

    char *guess_hash = generateHashForBlock(prevHash, buff);

    char zeros[difficulty + 1];
    for (int i = 0; i < difficulty; i ++) {
        zeros[i] = LEADING_ZERO;
    }

    zeros[difficulty] = '\0';

    return strncmp(guess_hash, zeros, strlen(zeros)) == 0;
}

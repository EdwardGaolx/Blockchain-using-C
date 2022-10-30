#include <time.h>
#include "blockchain.c"
#include "proof_of_work.h"

#define PREVPROOF ("10")

bool test(void) {
    char *prevhash = generateHash(randomString(64));

    char *hash = generateHashForBlock(prevhash, PREVPROOF);

    int difficulty = 1;

    double time = 0;

    bool result;

    while (difficulty < 7) {
        clock_t begin = clock();

        int proof = generateProof(10, hash, difficulty);

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

        bool isLonger = time_spent > time;

        printf("Execution time for level %d is %f CLOCKS_PER_SEC, the proof for 10 is %d \n", difficulty, time_spent, proof);
        
        time = time_spent;
        difficulty ++;
        result = result && isLonger;
    }

    return result;
}

int main(void) {
    printf("Proof of work test begins\n");
    bool result = test();
    printf("Does it pass? %s", result?"true":"false");
}
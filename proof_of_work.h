#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.h"

#define PROOF_SIZE (sizeof(int) * 8 + 1)
#define LEADING_ZERO ('0')
#define NORMALDIFF (4)

/* generate new proof of work based on previous proof and previous hash */
extern int generateProof(int prevProof, char *prevHash, int difficulty);

/* checks if a proof is valid given previous proof and previous hash */
extern bool isValidProof(int proof, int prevProof, char *prevHash, int difficulty);

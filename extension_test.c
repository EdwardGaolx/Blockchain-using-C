#include "sha256.h"
#include "blockchain.h"
#include "proof_of_work.h"
#include "utilities.h"
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SHA256_NUM_TESTS 5
#define PROOF_NUM_DIFFICULTY 6
#define PREVPROOF ("10")

bool blockchain_test(void)
{
  bool test_result = true;
  Blockchain_t *chain = createChain();

  addBlock(chain, "first");
  addBlock(chain, "second");
  addBlock(chain, "third");
  addBlock(chain, "fourth");
  addBlock(chain, "fifth");

  printFullChain(chain);
  test_result = checkConsistency(chain);
  printf("ACTION: modifies block 3\n");
  modifyBlock(chain, 3, "hello world");
  printFullChain(chain);
  test_result = checkConsistency(chain);
  printf("ACTION: recalculates chain\n");
  recalculateChain(chain);
  printFullChain(chain);
  test_result = checkConsistency(chain);

  freeChain(chain);
  return test_result;
}

bool sha256_test(void)
{
  char *data[SHA256_NUM_TESTS] = {"hello world", "testing", "12345678", "",
                                  "This is a sentence with more than 64 characters to test the sha256 algorithm"};

  char *hash[SHA256_NUM_TESTS] = {"b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9",
                                  "cf80cd8aed482d5d1527d7dc72fceff84e6326592848447d2dc0b0e87dfc9a90",
                                  "ef797c8118f02dfb649607dd5d3f8c7623048c9c063d532cc95c5ed7a898a64f",
                                  "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", 
                                  "6972fb68b1f6ebc255202fff79af92deb17fd00adec0744381def63ff868a893"};

  char *calculated_hash = calloc(HASH_BYTE_SIZE, sizeof(char));
  bool test_passed = true;
  for (int i = 0; i < SHA256_NUM_TESTS; i++)
  {
    calculated_hash = generateHash(data[i]);
    printf("test %d: ", i + 1);
    if (strcmp(hash[i], calculated_hash) == 0)
    {
      printf("SUCCEEDS!\n");
    }
    else
    {
      printf("FAILS!\n");
      test_passed = false;
    }
  }
  return test_passed;
}

bool proof_of_work_test(void)
{
  char *prevhash = generateHash(randomString(64));

  char *hash = generateHashForBlock(prevhash, PREVPROOF);

  int difficulty = 1;

  double time = 0;

  bool result = true;

  while (difficulty <= PROOF_NUM_DIFFICULTY)
  {
    clock_t begin = clock();

    int proof = generateProof(10, hash, difficulty);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    bool isLonger = time_spent > time;

    printf("Execution time for level %d is %f CLOCKS_PER_SEC, the proof for 10 is %d \n", difficulty, time_spent, proof);

    time = time_spent;
    difficulty++;
    result = result && isLonger;
  }

  return result;
}

int main()
{
  bool test_result;
  printf("\n---------STARTING BLOCKCHAIN TEST---------\n");
  test_result = blockchain_test();
  printf("TEST %s\n", test_result ? "SUCCESSFUL" : "UNSUCCESSFUL");
  printf("\n---------STARTING SHA256 TEST---------\n");
  test_result = sha256_test();
  printf("TEST %s\n", test_result ? "SUCCESSFUL" : "UNSUCCESSFUL");
  printf("\n---------STARTING PROOF OF WORK TEST---------\n");
  test_result = proof_of_work_test();
  printf("TEST %s\n", test_result ? "SUCCESSFUL" : "UNSUCCESSFUL");
}
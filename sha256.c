#include "sha256.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BITS_SIZE 32
#define K_SIZE 64
#define BLOCK_SIZE 64
#define BYTES_TO_APPEND 8
#define BYTE_SIZE 8
#define ROTL(n, d) (((n) << (d)) | ((n) >> (BITS_SIZE - (d))))
#define ROTR(n, d) (((n) >> (d)) | ((n) << (BITS_SIZE - (d))))
#define SHIFTL(n, d) ((n) << (d))
#define SHIFTR(n, d) ((n) >> (d))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define GET_8_BITS(x) ((x) & ((1 << 8) - 1))

#define D0(w) (ROTR((w), 7) ^ ROTR((w), 18) ^ SHIFTR((w), 3))
#define D1(w) (ROTR((w), 17) ^ ROTR((w), 19) ^ SHIFTR((w), 10))
#define S0(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))
#define S1(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))

typedef struct Context {
  uint32_t hash_val[8];
  uint32_t words[K_SIZE];
  char data_block[BLOCK_SIZE];
  uint32_t size;
  uint32_t total_size;
} Context_t;

static const uint32_t sha256_k[K_SIZE] = //cube roots of the first 64 primes
  {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
  0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
  0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
  0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
  0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
  0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

uint32_t get_bigendian(char *msg, int bytes) {
    uint32_t result = 0;
    for (int i = 0; i < bytes; i++) {
        result = 256 * result + (msg[i] & 0xff);
    }
    return result;
}

void hash_init(Context_t *ctx) {
  ctx->size = 0;
  ctx->total_size = 0;
  ctx->hash_val[0] = 0x6a09e667;
  ctx->hash_val[1] = 0xbb67ae85;
  ctx->hash_val[2] = 0x3c6ef372;
  ctx->hash_val[3] = 0xa54ff53a;
  ctx->hash_val[4] = 0x510e527f;
  ctx->hash_val[5] = 0x9b05688c;
  ctx->hash_val[6] = 0x1f83d9ab;
  ctx->hash_val[7] = 0x5be0cd19;
}

void hash_process(Context_t *ctx) {
  uint32_t a, b, c, d, e, f, g, h, temp1, temp2;
  uint32_t words[K_SIZE];

  //initialize message schedule array
  char *msg_ptr = ctx->data_block;

  for (int i = 0; i < 16; i++) {
    words[i] = get_bigendian(msg_ptr, 4);
    msg_ptr += 4;
  }
  for (int i = 16; i < K_SIZE; i++) {
    words[i] = words[i - 16] + D0(words[i - 15]) + words[i - 7] + D1(words[i - 2]);
  }  

  //initialize working registers
  a = ctx->hash_val[0];
  b = ctx->hash_val[1];
  c = ctx->hash_val[2];
  d = ctx->hash_val[3];
  e = ctx->hash_val[4];
  f = ctx->hash_val[5];
  g = ctx->hash_val[6];
  h = ctx->hash_val[7];

  //compress 
  for (int i = 0; i < K_SIZE; i++) {
    temp1 = h + S1(e) + CH(e, f, g) + sha256_k[i] + words[i];
    temp2 = S0(a) + MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + temp1;
    d = c;
    c = b;
    b = a;
    a = temp1 + temp2;
  }

  //add compressed chunks back to hash values
  ctx->hash_val[0] += a;
  ctx->hash_val[1] += b;
  ctx->hash_val[2] += c;
  ctx->hash_val[3] += d;
  ctx->hash_val[4] += e;
  ctx->hash_val[5] += f;
  ctx->hash_val[6] += g;
  ctx->hash_val[7] += h;
}

void hash_update(Context_t *ctx, char *msg, int msg_len) {
  ctx->total_size += msg_len;

  char *msgptr = msg;

  while(msg_len > 0) {
    uint32_t updated_len = msg_len;
    /* 
      if message length is greater than remaining size in block, modifies the 
      length of messages to be added to block
    */
    if (msg_len > BLOCK_SIZE - ctx->size) {
      updated_len = BLOCK_SIZE - ctx->size;
    }
    /* updates block */
    memcpy(&ctx->data_block[ctx->size], msgptr, updated_len);
    ctx->size += updated_len;
    msg_len -= updated_len;
    msgptr += updated_len;

    /* if block is full, process block */
    if (ctx->size == BLOCK_SIZE) {
      hash_process(ctx);
      ctx->size = 0;
    }
  }
}

void hash_final(Context_t *ctx) {
  char *msgptr = &ctx->data_block[ctx->size];

  /* add single bit 1 to message */
  *msgptr++ = (char)0x80; // 0x80 = 0b10000000
  ctx->size++;

  /* if remaining block size is less than 64 bits (8 byte) */
  if (BLOCK_SIZE - ctx->size < BYTES_TO_APPEND) {
    memset(msgptr, 0x00, BLOCK_SIZE - ctx->size);
    hash_process(ctx);
    ctx->size = 0;
    msgptr = ctx->data_block;
  }

  /* pad 0s until 64 bits remain */
  uint32_t pad_size = BLOCK_SIZE - ctx->size - BYTES_TO_APPEND;
  memset(msgptr, 0x00, pad_size);
  msgptr += pad_size;

  /* append 64-bit int representing message length to end of message */
  uint32_t total_size_bits = ctx->total_size * 8;
  for (int i = 7; i >= 0; i--) {
    msgptr[i] = GET_8_BITS(total_size_bits);
    total_size_bits >>= BYTE_SIZE;
  }

  hash_process(ctx);
}

char *generateHash(char *msg) {
  Context_t *ctx = malloc(sizeof(Context_t));
  uint32_t len = strlen(msg);
  hash_init(ctx);
  hash_update(ctx, msg, len);
  hash_final(ctx);

  /* append the final hash */
  char *hash_result = calloc(HASH_BYTE_SIZE, sizeof(char));
  for (int i = 0; i < 8; i++) {
    sprintf(hash_result + (i * BYTE_SIZE), "%08x", ctx->hash_val[i]);
  }
  free(ctx);
  return hash_result;
}

char *generateHashForBlock(char *prev_hash, char *data) {
  char *tempStr = calloc(strlen(prev_hash) + strlen(data), sizeof(char));
  memcpy(tempStr, prev_hash, strlen(prev_hash));
  memcpy(tempStr + strlen(prev_hash), data, strlen(data));
  char* result = generateHash(tempStr);
  free(tempStr);
  return result;
}
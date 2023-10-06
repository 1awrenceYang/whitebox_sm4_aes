/*
 *
 * Chinese Academy of Sciences
 * State Key Laboratory of Information Security
 * Institute of Information Engineering
 *
 * Copyright (C) 2016 Chinese Academy of Sciences
 *
 * LuoPeng, luopeng@iie.ac.cn
 * Updated in Oct 2016
 * Updated in Jan 2017, update muliple function on GF(2^8).
 *
 */
#include <stdint.h>
#include <stdio.h>

#include "gcm.h"

 /*
  * round constants
  */
static uint8_t RC[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

/*
 * Sbox
 */
static uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

/*
 * Inverse Sboxs
 */
static uint8_t INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

/**
 * https://en.wikipedia.org/wiki/Finite_field_arithmetic
 * Multiply two numbers in the GF(2^8) finite field defined
 * by the polynomial x^8 + x^4 + x^3 + x + 1 = 0
 * We do use mul2(int8_t a) but not mul(uint8_t a, uint8_t b)
 * just in order to get a higher speed.
 */
static inline uint8_t mul2(uint8_t a) {
    return (a & 0x80) ? ((a << 1) ^ 0x1b) : (a << 1);
}

/**
 * @purpose:    ShiftRows
 * @descrption:
 *  Row0: s0  s4  s8  s12   <<< 0 byte
 *  Row1: s1  s5  s9  s13   <<< 1 byte
 *  Row2: s2  s6  s10 s14   <<< 2 bytes
 *  Row3: s3  s7  s11 s15   <<< 3 bytes
 */
static void shift_rows(uint8_t* state) {
    uint8_t temp;
    // row1
    temp = *(state + 1);
    *(state + 1) = *(state + 5);
    *(state + 5) = *(state + 9);
    *(state + 9) = *(state + 13);
    *(state + 13) = temp;
    // row2
    temp = *(state + 2);
    *(state + 2) = *(state + 10);
    *(state + 10) = temp;
    temp = *(state + 6);
    *(state + 6) = *(state + 14);
    *(state + 14) = temp;
    // row3
    temp = *(state + 15);
    *(state + 15) = *(state + 11);
    *(state + 11) = *(state + 7);
    *(state + 7) = *(state + 3);
    *(state + 3) = temp;
}

/**
 * @purpose:    Inverse ShiftRows
 * @description
 *  Row0: s0  s4  s8  s12   >>> 0 byte
 *  Row1: s1  s5  s9  s13   >>> 1 byte
 *  Row2: s2  s6  s10 s14   >>> 2 bytes
 *  Row3: s3  s7  s11 s15   >>> 3 bytes
 */
static void inv_shift_rows(uint8_t* state) {
    uint8_t temp;
    // row1
    temp = *(state + 13);
    *(state + 13) = *(state + 9);
    *(state + 9) = *(state + 5);
    *(state + 5) = *(state + 1);
    *(state + 1) = temp;
    // row2
    temp = *(state + 14);
    *(state + 14) = *(state + 6);
    *(state + 6) = temp;
    temp = *(state + 10);
    *(state + 10) = *(state + 2);
    *(state + 2) = temp;
    // row1
    temp = *(state + 3);
    *(state + 3) = *(state + 7);
    *(state + 7) = *(state + 11);
    *(state + 11) = *(state + 15);
    *(state + 15) = temp;
}

void aes_key_schedule_128(const uint8_t* key, uint8_t* roundkeys) {

    uint8_t temp[4];
    uint8_t* last4bytes; // point to the last 4 bytes of one round
    uint8_t* lastround;
    uint8_t i;

    for (i = 0; i < 16; ++i) {
        *roundkeys++ = *key++;
    }

    last4bytes = roundkeys - 4;
    for (i = 0; i < AES_ROUNDS; ++i) {
        // k0-k3 for next round
        temp[3] = SBOX[*last4bytes++];
        temp[0] = SBOX[*last4bytes++];
        temp[1] = SBOX[*last4bytes++];
        temp[2] = SBOX[*last4bytes++];
        temp[0] ^= RC[i];
        lastround = roundkeys - 16;
        *roundkeys++ = temp[0] ^ *lastround++;
        *roundkeys++ = temp[1] ^ *lastround++;
        *roundkeys++ = temp[2] ^ *lastround++;
        *roundkeys++ = temp[3] ^ *lastround++;
        // k4-k7 for next round        
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        // k8-k11 for next round
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        // k12-k15 for next round
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
    }
}

void aes_encrypt_128(const uint8_t* roundkeys, const uint8_t* plaintext, uint8_t* ciphertext) {

    uint8_t tmp[16], t;
    uint8_t i, j;

    // first AddRoundKey
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        *(ciphertext + i) = *(plaintext + i) ^ *roundkeys++;
    }

    // 9 rounds
    for (j = 1; j < AES_ROUNDS; ++j) {

        // SubBytes
        for (i = 0; i < AES_BLOCK_SIZE; ++i) {
            *(tmp + i) = SBOX[*(ciphertext + i)];
        }
        shift_rows(tmp);
        /*
         * MixColumns
         * [02 03 01 01]   [s0  s4  s8  s12]
         * [01 02 03 01] . [s1  s5  s9  s13]
         * [01 01 02 03]   [s2  s6  s10 s14]
         * [03 01 01 02]   [s3  s7  s11 s15]
         */
        for (i = 0; i < AES_BLOCK_SIZE; i += 4) {
            t = tmp[i] ^ tmp[i + 1] ^ tmp[i + 2] ^ tmp[i + 3];
            ciphertext[i] = mul2(tmp[i] ^ tmp[i + 1]) ^ tmp[i] ^ t;
            ciphertext[i + 1] = mul2(tmp[i + 1] ^ tmp[i + 2]) ^ tmp[i + 1] ^ t;
            ciphertext[i + 2] = mul2(tmp[i + 2] ^ tmp[i + 3]) ^ tmp[i + 2] ^ t;
            ciphertext[i + 3] = mul2(tmp[i + 3] ^ tmp[i]) ^ tmp[i + 3] ^ t;
        }

        // AddRoundKey
        for (i = 0; i < AES_BLOCK_SIZE; ++i) {
            *(ciphertext + i) ^= *roundkeys++;
        }

    }

    // last round
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        *(ciphertext + i) = SBOX[*(ciphertext + i)];
    }
    shift_rows(ciphertext);
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        *(ciphertext + i) ^= *roundkeys++;
    }

}

void aes_decrypt_128(const uint8_t* roundkeys, const uint8_t* ciphertext, uint8_t* plaintext) {

    uint8_t tmp[16];
    uint8_t t, u, v;
    uint8_t i, j;

    roundkeys += 160;

    // first round
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        *(plaintext + i) = *(plaintext + i) ^ *(roundkeys + i);
    }
    roundkeys -= 16;
    inv_shift_rows(plaintext);
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        *(plaintext + i) = INV_SBOX[*(plaintext + i)];
    }

    for (j = 1; j < AES_ROUNDS; ++j) {

        // Inverse AddRoundKey
        for (i = 0; i < AES_BLOCK_SIZE; ++i) {
            *(tmp + i) = *(plaintext + i) ^ *(roundkeys + i);
        }

        /*
         * Inverse MixColumns
         * [0e 0b 0d 09]   [s0  s4  s8  s12]
         * [09 0e 0b 0d] . [s1  s5  s9  s13]
         * [0d 09 0e 0b]   [s2  s6  s10 s14]
         * [0b 0d 09 0e]   [s3  s7  s11 s15]
         */
        for (i = 0; i < AES_BLOCK_SIZE; i += 4) {
            t = tmp[i] ^ tmp[i + 1] ^ tmp[i + 2] ^ tmp[i + 3];
            plaintext[i] = t ^ tmp[i] ^ mul2(tmp[i] ^ tmp[i + 1]);
            plaintext[i + 1] = t ^ tmp[i + 1] ^ mul2(tmp[i + 1] ^ tmp[i + 2]);
            plaintext[i + 2] = t ^ tmp[i + 2] ^ mul2(tmp[i + 2] ^ tmp[i + 3]);
            plaintext[i + 3] = t ^ tmp[i + 3] ^ mul2(tmp[i + 3] ^ tmp[i]);
            u = mul2(mul2(tmp[i] ^ tmp[i + 2]));
            v = mul2(mul2(tmp[i + 1] ^ tmp[i + 3]));
            t = mul2(u ^ v);
            plaintext[i] ^= t ^ u;
            plaintext[i + 1] ^= t ^ v;
            plaintext[i + 2] ^= t ^ u;
            plaintext[i + 3] ^= t ^ v;
        }

        // Inverse ShiftRows
        inv_shift_rows(plaintext);

        // Inverse SubBytes
        for (i = 0; i < AES_BLOCK_SIZE; ++i) {
            *(plaintext + i) = INV_SBOX[*(plaintext + i)];
        }

        roundkeys -= 16;

    }

    // last AddRoundKey
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        *(plaintext + i) ^= *(roundkeys + i);
    }

}



#define DEBUG

void* gcm_init() {
	return malloc(sizeof(gcm_context));
}

void* whitebox_gcm_init() {
	return malloc(sizeof(whitebox_gcm_context));
}

operation_result gcm_setkey(void* ctx,
	const unsigned char* key,
	unsigned int keybits) {
	if (NULL == ctx || NULL == key || 128 != keybits) { return OPERATION_FAIL; }
	gcm_context* temp_ctx = (gcm_context*)ctx;
	temp_ctx->block_encrypt = (block_encrypt_p)aes_encrypt_128;
	temp_ctx->rk = (uint8_t*)malloc(sizeof(uint8_t) * AES_ROUND_KEY_SIZE);
	if (NULL == temp_ctx->rk) { return OPERATION_FAIL; }
	aes_key_schedule_128((const uint8_t*)key, temp_ctx->rk);
	return OPERATION_SUC;
}

operation_result gcm_setkey_whitebox(void* ctx,
	const unsigned char* key,
	unsigned int keybits, whitebox_enc_p enc_algo) {
	if (NULL == ctx || NULL == key || 128 != keybits) { return OPERATION_FAIL; }
	whitebox_gcm_context* temp_ctx = (whitebox_gcm_context*)ctx;
	temp_ctx->key = (char*)key;
	temp_ctx->block_encrypt = (whitebox_enc_p)enc_algo;
	return OPERATION_SUC;
}

void gcm_free(void* ctx) {
	if (ctx) {
		gcm_context* temp_ctx = (gcm_context*)ctx;
		if (temp_ctx->rk) {
			free(temp_ctx->rk);
		}
		free(ctx);
	}
}

void gcm_free_whitebox(void* ctx) {
	if (ctx) {
		gcm_context* temp_ctx = (gcm_context*)ctx;
		free(ctx);
	}
}



/**
 * compute T1, T2, ... , and T15
 * suppose 0^n is a string with n bit zeros, s1||s2 is a jointed string of s1 and s2
 *
 * T1 = T0 . P^8
 * 	where P^8 = 0^8 || 1 || 0^119
 * T2 = T1 . P^8 = T0 . P^16
 * 	where P^16 = 0^16 || 1 || 0^111
 * T3 = T2 . P^8 = T0 . P^24
 * ...
 * T15 = T14 . P^8 = T0 . P^120
 * 	where P^120 = 0^120 || 1 || 0^7
 *
 */
static void otherT(uint8_t T[][256][16]) {
	int i = 0, j = 0, k = 0;
	uint64_t vh, vl;
	uint64_t zh, zl;
	for (i = 0; i < 256; i++) {
		vh = ((uint64_t)T[0][i][0] << 56) ^ ((uint64_t)T[0][i][1] << 48) ^ ((uint64_t)T[0][i][2] << 40) ^ ((uint64_t)T[0][i][3] << 32) ^
			((uint64_t)T[0][i][4] << 24) ^ ((uint64_t)T[0][i][5] << 16) ^ ((uint64_t)T[0][i][6] << 8) ^ ((uint64_t)T[0][i][7]);
		vl = ((uint64_t)T[0][i][8] << 56) ^ ((uint64_t)T[0][i][9] << 48) ^ ((uint64_t)T[0][i][10] << 40) ^ ((uint64_t)T[0][i][11] << 32) ^
			((uint64_t)T[0][i][12] << 24) ^ ((uint64_t)T[0][i][13] << 16) ^ ((uint64_t)T[0][i][14] << 8) ^ ((uint64_t)T[0][i][15]);
		zh = zl = 0;
		for (j = 0; j <= 120; j++) {
			if ((j > 0) && (0 == j % 8)) {
				zh ^= vh;
				zl ^= vl;
				for (k = 1; k <= GCM_BLOCK_SIZE / 2; k++) {
					T[j / 8][i][GCM_BLOCK_SIZE / 2 - k] = (uint8_t)zh;
					zh = zh >> 8;
					T[j / 8][i][GCM_BLOCK_SIZE - k] = (uint8_t)zl;
					zl = zl >> 8;
				}
				zh = zl = 0;
			}
			if (vl & 0x1) {
				vl = vl >> 1;
				if (vh & 0x1) { vl ^= 0x8000000000000000; }
				vh = vh >> 1;
				vh ^= GCM_FIELD_CONST;
			}
			else {
				vl = vl >> 1;
				if (vh & 0x1) { vl ^= 0x8000000000000000; }
				vh = vh >> 1;
			}
		}
	}
}

/**
 * @purpose
 * compute table T0 = X0 . H
 * only the first byte of X0 is nonzero, other bytes are all 0
 * @T
 * the final tables: 16 tables in total, each has 256 elements, the value of which is 16 bytes
 * @H
 * 128-bit, H = E(K, 0^128)
 * the leftmost(most significant) bit of H[0] is bit-0 of H(in GCM)
 * the rightmost(least significant) bit of H[15] is bit-127 of H(in GCM)
 */
static void computeTable(uint8_t T[][256][16], uint8_t H[]) {

	// zh is the higher 64-bit, zl is the lower 64-bit
	uint64_t zh = 0, zl = 0;
	// vh is the higher 64-bit, vl is the lower 64-bit
	uint64_t vh = ((uint64_t)H[0] << 56) ^ ((uint64_t)H[1] << 48) ^ ((uint64_t)H[2] << 40) ^ ((uint64_t)H[3] << 32) ^
		((uint64_t)H[4] << 24) ^ ((uint64_t)H[5] << 16) ^ ((uint64_t)H[6] << 8) ^ ((uint64_t)H[7]);
	uint64_t vl = ((uint64_t)H[8] << 56) ^ ((uint64_t)H[9] << 48) ^ ((uint64_t)H[10] << 40) ^ ((uint64_t)H[11] << 32) ^
		((uint64_t)H[12] << 24) ^ ((uint64_t)H[13] << 16) ^ ((uint64_t)H[14] << 8) ^ ((uint64_t)H[15]);
	uint8_t temph;

	uint64_t tempvh = vh;
	uint64_t tempvl = vl;
	int i = 0, j = 0;
	for (i = 0; i < 256; i++) {
		temph = (uint8_t)i;
		vh = tempvh;
		vl = tempvl;
		zh = zl = 0;

		for (j = 0; j < 8; j++) {
			if (0x80 & temph) {
				zh ^= vh;
				zl ^= vl;
			}
			if (vl & 0x1) {
				vl = vl >> 1;
				if (vh & 0x1) { vl ^= 0x8000000000000000; }
				vh = vh >> 1;
				vh ^= GCM_FIELD_CONST;
			}
			else {
				vl = vl >> 1;
				if (vh & 0x1) { vl ^= 0x8000000000000000; }
				vh = vh >> 1;
			}
			temph = temph << 1;
		}
		// get result
		for (j = 1; j <= GCM_BLOCK_SIZE / 2; j++) {
			T[0][i][GCM_BLOCK_SIZE / 2 - j] = (uint8_t)zh;
			zh = zh >> 8;
			T[0][i][GCM_BLOCK_SIZE - j] = (uint8_t)zl;
			zl = zl >> 8;
		}
	}
	otherT(T);
}

/**
 * return the value of (output.H) by looking up tables
 */
static void multi(uint8_t T[][256][16], uint8_t* output) {
	uint8_t i, j;
	uint8_t temp[16];
	for (i = 0; i < 16; i++) {
		temp[i] = output[i];
		output[i] = 0;
	}
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			output[j] ^= T[i][*(temp + i)][j];
		}
	}
}

/**
 * return the value of vector after increasement
 */
static void incr(uint8_t* iv) {
	iv += 12;
	uint32_t temp = ((uint32_t)iv[0] << 24) + ((uint32_t)iv[1] << 16) + ((uint32_t)iv[2] << 8) + ((uint32_t)iv[3]) + 1;
	iv[3] = (uint8_t)(temp); // the priority of () is higher than >>, ^_^
	iv[2] = (uint8_t)(temp >> 8);
	iv[1] = (uint8_t)(temp >> 16);
	iv[0] = (uint8_t)(temp >> 24);
}

#if defined(DEBUG)
static int countY = 0;

static void printf_output(uint8_t* p, size_t length) {
	size_t i = 0, j = 0;
	if (length > GCM_BLOCK_SIZE) {
		// first block
		for (i = 0; i < GCM_BLOCK_SIZE; i++) {
			printf("%2x ", p[i]);
		}
		printf("\n");
		// middle blocks
		for (i = 1; i < length / GCM_BLOCK_SIZE; i++) {
			printf("                ");
			for (j = 0; j < GCM_BLOCK_SIZE; j++) {
				printf("%2x ", p[i * GCM_BLOCK_SIZE + j]);
			}
			printf("\n");
		}
		// last block
		printf("                ");
		i = length / GCM_BLOCK_SIZE * GCM_BLOCK_SIZE;
		for (; i < length; i++) {
			printf("%2x ", p[i]);
		}
		printf("\n");
	}
	else {
		for (i = 0; i < length; i++) {
			printf("%2x ", p[i]);
		}
		printf("\n");
	}
}
#endif

/*
 * a: additional authenticated data
 * c: the cipher text or initial vector
 */
static void ghash(uint8_t T[][256][16],
	const uint8_t* add,
	size_t add_len,
	const uint8_t* cipher,
	size_t length,
	uint8_t* output) {
	/* x0 = 0 */
	*(uint64_t*)output = 0;
	*((uint64_t*)output + 1) = 0;

	/* compute with add */
	int i = 0;
	for (i = 0; i < add_len / GCM_BLOCK_SIZE; i++) {
		*(uint64_t*)output ^= *(uint64_t*)add;
		*((uint64_t*)output + 1) ^= *((uint64_t*)add + 1);
		add += GCM_BLOCK_SIZE;
		multi(T, output);
	}

	if (add_len % GCM_BLOCK_SIZE) {
		// the remaining add
		for (i = 0; i < add_len % GCM_BLOCK_SIZE; i++) {
			*(output + i) ^= *(add + i);
		}
		multi(T, output);
	}

	/* compute with cipher text */
	for (i = 0; i < length / GCM_BLOCK_SIZE; i++) {
		*(uint64_t*)output ^= *(uint64_t*)cipher;
		*((uint64_t*)output + 1) ^= *((uint64_t*)cipher + 1);
		cipher += GCM_BLOCK_SIZE;
		multi(T, output);
	}
	if (length % GCM_BLOCK_SIZE) {
		// the remaining cipher
		for (i = 0; i < length % GCM_BLOCK_SIZE; i++) {
			*(output + i) ^= *(cipher + i);
		}
		multi(T, output);
	}

	/* eor (len(A)||len(C)) */
	uint64_t temp_len = (uint64_t)(add_len * 8); // len(A) = (uint64_t)(add_len*8)
	for (i = 1; i <= GCM_BLOCK_SIZE / 2; i++) {
		output[GCM_BLOCK_SIZE / 2 - i] ^= (uint8_t)temp_len;
		temp_len = temp_len >> 8;
	}
	temp_len = (uint64_t)(length * 8); // len(C) = (uint64_t)(length*8)
	for (i = 1; i <= GCM_BLOCK_SIZE / 2; i++) {
		output[GCM_BLOCK_SIZE - i] ^= (uint8_t)temp_len;
		temp_len = temp_len >> 8;
	}
	multi(T, output);
}

#define xor_state(output, input, buff, size) \
    for (t = 0; t < size; ++t) {             \
        output[t] = input[t] ^ buff[t];      \
    }

#define copy_state(output, input, size) \
    for (t = 0; t < size; ++t) {        \
        output[t] = input[t];           \
    }

/**
 * authenticated encryption
 */
operation_result gcm_crypt_and_tag(void* context,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output,
	unsigned char* tag,
	size_t tag_len) 
{

	gcm_context* ctx = (gcm_context*)context;
	if (!ctx || !(ctx->rk)) { return OPERATION_FAIL; }
	if (tag_len <= 0 || tag_len > GCM_BLOCK_SIZE) { return OPERATION_FAIL; }

	int i, t;
	uint8_t y0[GCM_BLOCK_SIZE] = { 0 }; // store the counter
	uint8_t ency0[GCM_BLOCK_SIZE]; // the cihper text of first counter

	// set H
	(ctx->block_encrypt)(ctx->rk, y0, ency0);
	for (i = 0; i < GCM_BLOCK_SIZE; ++i) { ctx->H[i] = ency0[i]; }

#if defined(DEBUG)
	printf("\n----AUTH-ENCRYPTION----\n");
	printf("H:              ");
	printf_output(ctx->H, GCM_BLOCK_SIZE);
	printf("COMPUTE TABLES\n");
	countY = 0;
#endif
	computeTable(ctx->T, ctx->H);

	// compute y0 (initilization vector)
	if (GCM_DEFAULT_IV_LEN == iv_len) {
		copy_state(y0, iv, GCM_DEFAULT_IV_LEN);
		y0[15] = 1;
	}
	else {
		ghash(ctx->T, NULL, 0, iv, iv_len, y0);
	}

#if defined(DEBUG)
	printf("Y%d:             ", countY);
	printf_output(y0, GCM_BLOCK_SIZE);
#endif

	// compute ency0 = ENC(K, y0)
	(ctx->block_encrypt)(ctx->rk, y0, ency0);

#if defined(DEBUG)
	printf("E(K, Y%d):       ", countY++);
	printf_output(ency0, GCM_BLOCK_SIZE);
#endif

	// encyrption
	uint8_t* output_temp = output; // store the start pointer of cipher text
	for (i = 0; i < length / GCM_BLOCK_SIZE; ++i) {
		incr(y0);
		(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		xor_state(output, input, ctx->buff, GCM_BLOCK_SIZE);
		output += GCM_BLOCK_SIZE;
		input += GCM_BLOCK_SIZE;
	}
	// the remaining plain text
	if (length % GCM_BLOCK_SIZE) {
		incr(y0);
		// the last block size man be smaller than GCM_BLOCK_SIZE, can NOT be written directly.
			  // (ctx->block_encrypt)((const uint8_t *)(ctx->rk), (const uint8_t *)y0, output);
		(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		xor_state(output, input, ctx->buff, length % GCM_BLOCK_SIZE);
	}

#if defined(DEBUG)
	printf("CIPHER:         ");
	printf_output(output_temp, length);
#endif

	// compute tag
	ghash(ctx->T, add, add_len, output_temp, length, ctx->buff);
#if defined(DEBUG)
	printf("GHASH(H, A, C): ");
	printf_output(ctx->buff, GCM_BLOCK_SIZE);
#endif

	for (i = 0; i < tag_len; ++i) {
		tag[i] = ctx->buff[i] ^ ency0[i];
	}
#if defined(DEBUG)
	printf("TAG:            ");
	printf_output(tag, tag_len);
#endif

	return OPERATION_SUC;
}


/*
 * authenticated decryption
 */
operation_result gcm_auth_decrypt(void* context,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* tag,
	size_t tag_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output) {

	gcm_context* ctx = (gcm_context*)context;
	if (!ctx || !(ctx->rk)) { return OPERATION_FAIL; }
	if (tag_len <= 0 || tag_len > GCM_BLOCK_SIZE) { return OPERATION_FAIL; }

	uint8_t y0[GCM_BLOCK_SIZE] = { 0 }; // store the counter
	uint8_t ency0[GCM_BLOCK_SIZE]; // the cihper text of first counter

	// set H
	(ctx->block_encrypt)(ctx->rk, y0, ency0);
	int i, t;
	for (i = 0; i < GCM_BLOCK_SIZE; ++i) { ctx->H[i] = ency0[i]; }

#if defined(DEBUG)
	printf("\n----AUTH-DECRYPTION----\n");
	printf("H:              ");
	printf_output(ctx->H, GCM_BLOCK_SIZE);
	printf("COMPUTE TABLES\n");
	countY = 0;
#endif
	computeTable(ctx->T, ctx->H);

	// compute tag
	ghash(ctx->T, add, add_len, input, length, ctx->buff);
#if defined(DEBUG)
	printf("GHASH(H, A, C): ");
	printf_output(ctx->buff, GCM_BLOCK_SIZE);
#endif

	// compute y0 (initilization vector)
	if (GCM_DEFAULT_IV_LEN == iv_len) {
		copy_state(y0, iv, GCM_DEFAULT_IV_LEN);
		y0[15] = 1;
	}
	else {
		ghash(ctx->T, NULL, 0, iv, iv_len, y0);
	}

	// compute ency0 = ENC(K, y0)
	(ctx->block_encrypt)(ctx->rk, y0, ency0);

	// authentication
	for (i = 0; i < tag_len; ++i) {
		if (tag[i] != (ency0[i] ^ ctx->buff[i])) { break; }
	}
	if (i != tag_len) {
		return OPERATION_FAIL;
	}

	// decyrption
	uint8_t* output_temp = output;
	for (i = 0; i < length / GCM_BLOCK_SIZE; ++i) {
		incr(y0);
		(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		xor_state(output, input, ctx->buff, GCM_BLOCK_SIZE);
		output += GCM_BLOCK_SIZE;
		input += GCM_BLOCK_SIZE;
	}
	// the remaining plain text
	if (length % GCM_BLOCK_SIZE) {
		incr(y0);
		(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		xor_state(output, input, ctx->buff, length % GCM_BLOCK_SIZE);
	}

#if defined(DEBUG)
	printf("PLAIN:          ");
	printf_output(output_temp, length);
#endif

	return OPERATION_SUC;
}

operation_result gcm_auth_decrypt_whitebox(void* context,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* tag,
	size_t tag_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output) {

	whitebox_gcm_context* ctx = (whitebox_gcm_context*)context;
	//if (!ctx || !(ctx->rk)) { return OPERATION_FAIL; }
	if (tag_len <= 0 || tag_len > GCM_BLOCK_SIZE) { return OPERATION_FAIL; }

	uint8_t y0[GCM_BLOCK_SIZE] = { 0 }; // store the counter
	uint8_t ency0[GCM_BLOCK_SIZE]; // the cihper text of first counter

	// set H
	//(ctx->block_encrypt)(ctx->rk, y0, ency0);
	(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ency0);

	int i, t;
	for (i = 0; i < GCM_BLOCK_SIZE; ++i) { ctx->H[i] = ency0[i]; }

#if defined(DEBUG)
	printf("\n----AUTH-DECRYPTION----\n");
	printf("H:              ");
	printf_output(ctx->H, GCM_BLOCK_SIZE);
	printf("COMPUTE TABLES\n");
	countY = 0;
#endif
	computeTable(ctx->T, ctx->H);

	// compute tag
	ghash(ctx->T, add, add_len, input, length, ctx->buff);
#if defined(DEBUG)
	printf("GHASH(H, A, C): ");
	printf_output(ctx->buff, GCM_BLOCK_SIZE);
#endif

	// compute y0 (initilization vector)
	if (GCM_DEFAULT_IV_LEN == iv_len) {
		copy_state(y0, iv, GCM_DEFAULT_IV_LEN);
		y0[15] = 1;
	}
	else {
		ghash(ctx->T, NULL, 0, iv, iv_len, y0);
	}

	// compute ency0 = ENC(K, y0)
	//(ctx->block_encrypt)(ctx->rk, y0, ency0);
	(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ency0);

	// authentication
	for (i = 0; i < tag_len; ++i) {
		if (tag[i] != (ency0[i] ^ ctx->buff[i])) { break; }
	}
	if (i != tag_len) {
		return OPERATION_FAIL;
	}

	// decyrption
	uint8_t* output_temp = output;
	for (i = 0; i < length / GCM_BLOCK_SIZE; ++i) {
		incr(y0);
		//(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ctx->buff);
		xor_state(output, input, ctx->buff, GCM_BLOCK_SIZE);
		output += GCM_BLOCK_SIZE;
		input += GCM_BLOCK_SIZE;
	}
	// the remaining plain text
	if (length % GCM_BLOCK_SIZE) {
		incr(y0);
		//(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ctx->buff);
		xor_state(output, input, ctx->buff, length % GCM_BLOCK_SIZE);
	}

#if defined(DEBUG)
	printf("PLAIN:          ");
	printf_output(output_temp, length);
#endif

	return OPERATION_SUC;
}


operation_result encryption(const unsigned char* key,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output,
	unsigned char* tag,
	size_t tag_len)
{

	void* context = gcm_init();
	if (!context) {
		printf("malloc failed.\n");
		return OPERATION_FAIL;
	}
	

	operation_result flag = gcm_setkey(context, key, 128);
	if (OPERATION_FAIL == flag) { return OPERATION_FAIL; }

	gcm_crypt_and_tag(context,
		iv, iv_len,
		add, add_len,
		input, length,
		output,
		tag, tag_len);

	gcm_free(context);

	return OPERATION_SUC;

}

operation_result decryption(const unsigned char* key,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* tag,
	size_t tag_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output) {

	void* context = gcm_init();
	if (!context) {
		printf("malloc failed.\n");
		return OPERATION_FAIL;
	}

	operation_result flag = gcm_setkey(context, key, 128);
	if (OPERATION_FAIL == flag) { return OPERATION_FAIL; }

	gcm_auth_decrypt(context,
		iv, iv_len,
		add, add_len,
		tag, tag_len,
		input, length,
		output);

	gcm_free(context);

	return OPERATION_SUC;

}


operation_result gcm_crypt_and_tag_whitebox(void* context,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output,
	unsigned char* tag,
	size_t tag_len)
{

	whitebox_gcm_context* ctx = (whitebox_gcm_context*)context;
	//if (!ctx || !(ctx->rk)) { return OPERATION_FAIL; }
	if (tag_len <= 0 || tag_len > GCM_BLOCK_SIZE) { return OPERATION_FAIL; }

	int i, t;
	uint8_t y0[GCM_BLOCK_SIZE] = { 0 }; // store the counter
	uint8_t ency0[GCM_BLOCK_SIZE]; // the cihper text of first counter

	// set H
	//(ctx->block_encrypt)(ctx->rk, y0, ency0);
	(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ency0);
	for (i = 0; i < GCM_BLOCK_SIZE; ++i) { ctx->H[i] = ency0[i]; }

#if defined(DEBUG)
	printf("\n----AUTH-ENCRYPTION----\n");
	printf("H:              ");
	printf_output(ctx->H, GCM_BLOCK_SIZE);
	printf("COMPUTE TABLES\n");
	countY = 0;
#endif
	computeTable(ctx->T, ctx->H);

	// compute y0 (initilization vector)
	if (GCM_DEFAULT_IV_LEN == iv_len) {
		copy_state(y0, iv, GCM_DEFAULT_IV_LEN);
		y0[15] = 1;
	}
	else {
		ghash(ctx->T, NULL, 0, iv, iv_len, y0);
	}

#if defined(DEBUG)
	printf("Y%d:             ", countY);
	printf_output(y0, GCM_BLOCK_SIZE);
#endif

	// compute ency0 = ENC(K, y0)
	//(ctx->block_encrypt)(ctx->rk, y0, ency0);
	(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ency0);

#if defined(DEBUG)
	printf("E(K, Y%d):       ", countY++);
	printf_output(ency0, GCM_BLOCK_SIZE);
#endif

	// encyrption
	uint8_t* output_temp = output; // store the start pointer of cipher text
	for (i = 0; i < length / GCM_BLOCK_SIZE; ++i) {
		incr(y0);
		//(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ctx->buff);
		xor_state(output, input, ctx->buff, GCM_BLOCK_SIZE);
		output += GCM_BLOCK_SIZE;
		input += GCM_BLOCK_SIZE;
	}
	// the remaining plain text
	if (length % GCM_BLOCK_SIZE) {
		incr(y0);
		// the last block size man be smaller than GCM_BLOCK_SIZE, can NOT be written directly.
			  // (ctx->block_encrypt)((const uint8_t *)(ctx->rk), (const uint8_t *)y0, output);
		//(ctx->block_encrypt)(ctx->rk, y0, ctx->buff);
		(ctx->block_encrypt)((char*)y0, ctx->key, (char*)ctx->buff);
		xor_state(output, input, ctx->buff, length % GCM_BLOCK_SIZE);
	}

#if defined(DEBUG)
	printf("CIPHER:         ");
	printf_output(output_temp, length);
#endif

	// compute tag
	ghash(ctx->T, add, add_len, output_temp, length, ctx->buff);
#if defined(DEBUG)
	printf("GHASH(H, A, C): ");
	printf_output(ctx->buff, GCM_BLOCK_SIZE);
#endif

	for (i = 0; i < tag_len; ++i) {
		tag[i] = ctx->buff[i] ^ ency0[i];
	}
#if defined(DEBUG)
	printf("TAG:            ");
	printf_output(tag, tag_len);
#endif

	return OPERATION_SUC;
}

operation_result encryption_gcm_whitebox(const unsigned char* key,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output,
	unsigned char* tag,
	size_t tag_len,
	whitebox_enc_p enc_algo)
{

	void* context = gcm_init();
	if (!context) {
		printf("malloc failed.\n");
		return OPERATION_FAIL;
	}


	//operation_result flag = gcm_setkey(context, key, 128);
	operation_result flag = gcm_setkey_whitebox(context, key, 128, enc_algo);
	if (OPERATION_FAIL == flag) { return OPERATION_FAIL; }

	gcm_crypt_and_tag_whitebox(context,
		iv, iv_len,
		add, add_len,
		input, length,
		output,
		tag, tag_len);

	gcm_free_whitebox(context);

	return OPERATION_SUC;

}

operation_result decryption_gcm_whitebox(const unsigned char* key,
	const unsigned char* iv,
	size_t iv_len,
	const unsigned char* add,
	size_t add_len,
	const unsigned char* tag,
	size_t tag_len,
	const unsigned char* input,
	size_t length,
	unsigned char* output,
	whitebox_enc_p enc_algo) 
{

	void* context = gcm_init();
	if (!context) {
		printf("malloc failed.\n");
		return OPERATION_FAIL;
	}

	//operation_result flag = gcm_setkey(context, key, 128);
	operation_result flag = gcm_setkey_whitebox(context, key, 128, enc_algo);
	if (OPERATION_FAIL == flag) { return OPERATION_FAIL; }

	gcm_auth_decrypt_whitebox(context,
		iv, iv_len,
		add, add_len,
		tag, tag_len,
		input, length,
		output);

	gcm_free_whitebox(context);

	return OPERATION_SUC;

}
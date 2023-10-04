#ifndef LOOK_UP_TABLE_SM4_H
#define LOOK_UP_TABLE_SM4_H
#include"masked_aes.h"
#include"masked_sm4.h"
#include"utils.h"
#include<map>
const long B1[8][8] =
{
    {1,0,1,0,0,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,0,1,0,1,0,0,0},
    {0,0,0,1,0,1,0,0},
    {0,0,0,0,1,0,1,0},
    {0,0,0,0,0,1,0,1},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,1}
};
const long B2[8][8] =
{
    {0,0,1,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0},
    {0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0}
};
const long B3[8][8] =
{
    {1,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0},
    {0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0},
    {0,0,0,0,0,1,0,0},
    {1,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,1}
};

void NewSbox(vec_GF2& input, vec_GF2& rk, vec_GF2& output);

std::map<char, uint64_t> BuildOneSboxTable(int num, int tij);

uint32_t ShiftAfterLookUpTable(uint64_t* four_input, int* tij);

uint32_t EncodeLookUpTableResult(uint32_t input, mat_GF2& inv_A, vec_GF2& b);

uint32_t RecoverLookUpTableResult(uint32_t input, mat_GF2& A, vec_GF2& b);

uint32_t Encode32bit(uint32_t input, mat_GF2& inv_A, vec_GF2& b);

uint32_t Decode32bit(uint32_t input, mat_GF2& A, vec_GF2& b);

uint32_t DiaAffineTransfer(uint32_t input, mat_GF2& A1, mat_GF2& A2, mat_GF2& A3, mat_GF2& A4, vec_GF2 b);

uint32_t DiaAffineTransfer_inv(uint32_t input, mat_GF2& A1, mat_GF2& A2, mat_GF2& A3, mat_GF2& A4, vec_GF2 b);

uint32_t LastEncode(uint32_t input, mat_GF2& A, vec_GF2& b);

void LookUpTableEncryptSM4(char* plaintext, char* key, char* ciphertext);

void LookUpTableDecryptSM4(char* plaintext, char* key, char* ciphertext);

#endif // !LOOK_UP_TABLE_SM4_H

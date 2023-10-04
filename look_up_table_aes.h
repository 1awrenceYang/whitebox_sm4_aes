#ifndef LOOK_UP_TABLE_ASE_H
#define LOOK_UP_TABLE_ASE_H
#include"look_up_table_sm4.h"
#include"masked_aes.h"
#include"masked_sm4.h"
#include"utils.h"



std::map<char, uint32_t> AESBuildTable_AS(int rij, char round_key, mat_GF2& input_decode_A, vec_GF2& input_decode_b, mat_GF2* output_encode_A, vec_GF2& output_encode_b);

char encode8(char input, mat_GF2& A, vec_GF2& b);

char decode8(char input, mat_GF2& A, vec_GF2& b);

uint32_t encode32(uint32_t input, mat_GF2& A, vec_GF2& b);

uint32_t decode32(uint32_t input, mat_GF2& A, vec_GF2& b);

uint32_t encode32dia(uint32_t input, mat_GF2* A, vec_GF2& b);

uint32_t decode32dia(uint32_t input, mat_GF2* A, vec_GF2& b);

void ExtendedShiftRow(uint32_t* shift_row_input, uint32_t* shift_row_output);

std::map<uint32_t, uint32_t> AESBuildTable_MC_mult2(int rij, mat_GF2* last_output_encode_A, vec_GF2& last_output_encode_b, mat_GF2* output_encode_A, vec_GF2& output_encode_b);

std::map<uint32_t, uint32_t> AESBuildTable_MC_mult3(int rij, mat_GF2* last_output_encode_A, vec_GF2& last_output_encode_b, mat_GF2* output_encode_A, vec_GF2& output_encode_b);

void GenerateEncode32Dia(mat_GF2* A, mat_GF2* inv_A, vec_GF2 &b);

void GenerateEncode8(mat_GF2& A, mat_GF2& inv_A, vec_GF2& b);

char RecoverSequence(uint32_t input, int rij);

void LookUpTableEncryptAES(char* plaintext, char* key, char* ciphertext);

void LookUpTableDecryptAES(char* plaintext, char* key, char* ciphertext);

#endif 

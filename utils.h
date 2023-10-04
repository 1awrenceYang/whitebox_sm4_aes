#ifndef UTILS_H
#define UTILS_H
#include"masked_sm4.h"
#include<NTL/ZZ.h>
#include<NTL/GF2.h>
#include<NTL/mat_GF2.h>
#include "gmp.h"
#include"gmpxx.h"
#include<random>
using namespace std;
using namespace NTL;

void Convert_to_char4(char* input, char4* output);

void Convert_to_char(char4* input, char* output);

void Convert_to_char4_32(char* input, char4* output);

void Convert_to_char_32(char4* input, char* output);

void print_char4(char4* info, unsigned int length);

void print_char(char* input, unsigned int bytes_length);

void LeftShift32(char4* input, char4* output, unsigned int length);

unsigned int  left_shit_int(unsigned int  val, unsigned int n);

unsigned int TFunctionVerification(char* input);

unsigned int fourCharToInt(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

unsigned int F_FunctionVerification(char *state, char *round_key);

unsigned int TFunctionKE_Verification(char* input);

unsigned int F_Function_KE_Verification(char* state, char* round_key);

void int_to_char(unsigned int val, char* output);

void ShiftRowVerification(char* input, char* output);

void ShiftRowVerification_inv(char* input, char* output);

void AddRoundKeyVerification(char* state, char* round_key, char* output);

void ShiftCol_KE_Verification(char* state, char* output);

void BytesSub_KE_Verification(char* state, char* output);

void XOR_RC_Verification(char* state, int round, char* output);

void AES_KE_T_Function_Verification(char* state, int round, char* output);

mat_GF2 Get_Inverse_Mat(mat_GF2 mat);

void PrintMatrix(mat_GF2 mat);

void PrintVector(vec_GF2 vec);

mat_GF2 GenerateGF2RandomMatrix(unsigned int dimension);

vec_GF2 GenerateGF2RandomVector(unsigned int Length);

void Generate_2Random_Inverse_Matrix(unsigned int dimension, mat_GF2& mat1, mat_GF2& mat2);

mat_GF2 Generate_Random_Inverse_Matrix(unsigned int dimension);

void Generate_2Random_Inverse_Diagonal_Matrix(mat_GF2& mat1, mat_GF2& mat2);

char vec_GF2_to_char(vec_GF2& input);

void char_to_vec_GF2(char input, vec_GF2& output);

uint64_t vec_GF2_to_u64(vec_GF2& input);

uint32_t vec_GF2_to_u32(vec_GF2& input);

vec_GF2 u64_to_vec_GF2(uint64_t input);

vec_GF2 u32_to_vec_GF2(uint32_t input);
#endif // !UTILS_H

#include"utils.h"
#include"masked_aes.h"
#include<string>
#include<random>
#include<map>
void Convert_to_char4(char* input, char4* output)
{
	char4* temp = (char4*)malloc(128 * sizeof(char4));
	for (int i = 0; i < 16; i++)
	{
		char bytes = input[i];
		for (int j = 0; j < 8; j++)
		{
			char bit = (bytes >> (7 - j)) & 0x01;
			temp[8 * i + j].ra = (char)(rand() % 2);
			temp[8 * i + j].rb = (char)(rand() % 2);
			temp[8 * i + j].rc = (char)(rand() % 2);
			temp[8 * i + j].x = ((temp[8 * i + j].ra) & (temp[8 * i + j].rb) & (temp[8 * i + j].rc)) ^ (bit);
		}
	}

	memcpy(output, temp, 128 * sizeof(char4));
	free(temp);
	temp = NULL;
}
void Convert_to_char(char4* input, char* output)
{
	char* temp = (char*)malloc(16 * sizeof(char));
	for (int i = 0; i < 128; i=i+8)
	{
		char temp1 = 0x00;
		temp1 = temp1 | (((input[i].ra) & (input[i].rb) & (input[i].rc) ^ (input[i].x)) << 7);
		temp1 = temp1 | (((input[i + 1].ra) & (input[i + 1].rb) & (input[i + 1].rc) ^ (input[i + 1].x)) << 6);
		temp1 = temp1 | (((input[i + 2].ra) & (input[i + 2].rb) & (input[i + 2].rc) ^ (input[i + 2].x)) << 5);
		temp1 = temp1 | (((input[i + 3].ra) & (input[i + 3].rb) & (input[i + 3].rc) ^ (input[i + 3].x)) << 4);

		temp1 = temp1 | (((input[i + 4].ra) & (input[i + 4].rb) & (input[i + 4].rc) ^ (input[i + 4].x)) << 3);
		temp1 = temp1 | (((input[i + 5].ra) & (input[i + 5].rb) & (input[i + 5].rc) ^ (input[i + 5].x)) << 2);
		temp1 = temp1 | (((input[i + 6].ra) & (input[i + 6].rb) & (input[i + 6].rc) ^ (input[i + 6].x)) << 1);
		temp1 = temp1 | (((input[i + 7].ra) & (input[i + 7].rb) & (input[i + 7].rc) ^ (input[i + 7].x)) << 0);

		/*printf("%d", input[i + 7].ra);
		printf("%d", input[i + 7].rb);
		printf("%d", input[i + 7].rc);
		printf("%d", input[i + 7].x);*/

		
		temp[i / 8] = temp1;
	}

	memcpy(output, temp, 16);
	free(temp);
	temp = NULL;
}
void print_char(char* text, unsigned int length)//输入字节长度
{
	for (int i = 0; i < length; i++)
	{
		printf("%02X", (unsigned char)text[i]);
		if ((i + 1) % 4 == 0)
			printf(" ");
		if ((i + 1) % 32 == 0)
			printf("\n");
	}
	printf("\n");
}

void print_char4(char4* info, unsigned int length)
{
	char* temp = (char*)malloc(length * sizeof(char));
	if(length==4)
		Convert_to_char_32(info, temp);
	else
		Convert_to_char(info, temp);
	print_char(temp, length);
	free(temp);
	temp = NULL;
}


unsigned int fourCharToInt(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
	unsigned int ret_val = 0;
	ret_val = a;
	ret_val <<= 8;
	ret_val |= b;
	ret_val <<= 8;
	ret_val |= c;
	ret_val <<= 8;
	ret_val |= d;

	return ret_val;
}


void Convert_to_char4_32(char* input, char4* output)
{
	char4* temp = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 4; i++)
	{
		char bytes = input[i];
		for (int j = 0; j < 8; j++)
		{
			char bit = (bytes >> (7 - j)) & 0x01;
			temp[8 * i + j].ra = (char)(rand() % 2);
			temp[8 * i + j].rb = (char)(rand() % 2);
			temp[8 * i + j].rc = (char)(rand() % 2);
			temp[8 * i + j].x = ((temp[8 * i + j].ra) & (temp[8 * i + j].rb) & (temp[8 * i + j].rc)) ^ (bit);
		}
	}

	memcpy(output, temp, 32 * sizeof(char4));
	free(temp);
	temp = NULL;
}

void Convert_to_char_32(char4* input, char* output)
{
	char* temp = (char*)malloc(4 * sizeof(char));
	for (int i = 0; i < 32; i = i + 8)
	{
		char temp1 = 0x00;
		temp1 = temp1 | (((input[i].ra) & (input[i].rb) & (input[i].rc) ^ (input[i].x)) << 7);
		temp1 = temp1 | (((input[i + 1].ra) & (input[i + 1].rb) & (input[i + 1].rc) ^ (input[i + 1].x)) << 6);
		temp1 = temp1 | (((input[i + 2].ra) & (input[i + 2].rb) & (input[i + 2].rc) ^ (input[i + 2].x)) << 5);
		temp1 = temp1 | (((input[i + 3].ra) & (input[i + 3].rb) & (input[i + 3].rc) ^ (input[i + 3].x)) << 4);

		temp1 = temp1 | (((input[i + 4].ra) & (input[i + 4].rb) & (input[i + 4].rc) ^ (input[i + 4].x)) << 3);
		temp1 = temp1 | (((input[i + 5].ra) & (input[i + 5].rb) & (input[i + 5].rc) ^ (input[i + 5].x)) << 2);
		temp1 = temp1 | (((input[i + 6].ra) & (input[i + 6].rb) & (input[i + 6].rc) ^ (input[i + 6].x)) << 1);
		temp1 = temp1 | (((input[i + 7].ra) & (input[i + 7].rb) & (input[i + 7].rc) ^ (input[i + 7].x)) << 0);

		/*printf("%d", input[i + 7].ra);
		printf("%d", input[i + 7].rb);
		printf("%d", input[i + 7].rc);
		printf("%d", input[i + 7].x);*/


		temp[i / 8] = temp1;
	}

	memcpy(output, temp, 4);
	free(temp);
	temp = NULL;
}

void LeftShift32(char4* input, char4*output, unsigned int length)
{
	char4* temp = (char4*)malloc(32 * sizeof(char4));

	for (int i = 0; i < 32-length; i++)
	{
		temp[i].ra = input[i + length].ra;
		temp[i].rb = input[i + length].rb;
		temp[i].rc = input[i + length].rc;
		temp[i].x = input[i + length].x;
	}

	for (int i = 0; i < length; i++)
	{
		temp[i + (32 - length)].ra = input[i].ra;
		temp[i + (32 - length)].rb = input[i].rb;
		temp[i + (32 - length)].rc = input[i].rc;
		temp[i + (32 - length)].x = input[i].x;
	}

	//print_char4(temp, 4);
	memcpy(output, temp, 32 * sizeof(char4));
	free(temp);
	temp = NULL;
}

unsigned int  left_shit_int(unsigned int  val, unsigned int n)
{
	return (val >> (32 - n) | (val << n));
}
unsigned int TFunctionVerification(char* input)
{
	char* SboxResult = (char*)malloc(4 * sizeof(char));
	for (int i = 0; i < 4; i++)
		SboxResult[i] = LookUpTable(input[i]);

	unsigned int SboxResultInt = fourCharToInt(SboxResult[0], SboxResult[1], SboxResult[2], SboxResult[3]);

	unsigned int Shift2 = left_shit_int(SboxResultInt, 2);
	unsigned int Shift10 = left_shit_int(SboxResultInt, 10);
	unsigned int Shift18 = left_shit_int(SboxResultInt, 18);
	unsigned int Shift24 = left_shit_int(SboxResultInt, 24);

	free(SboxResult);
	SboxResult = NULL;

	return SboxResultInt ^ Shift2 ^ Shift10 ^ Shift18 ^ Shift24;
}

unsigned int F_FunctionVerification(char* state, char* round_key)
{
	char* x1 = (char*)malloc(4 * sizeof(char));
	char* x2 = (char*)malloc(4 * sizeof(char));
	char* x3 = (char*)malloc(4 * sizeof(char));
	char* x4 = (char*)malloc(4 * sizeof(char));
	char* temp = (char*)malloc(4 * sizeof(char));

	for (int i = 0; i < 4; i++)
	{
		x1[i] = state[i];
		x2[i] = state[i + 4];
		x3[i] = state[i + 8];
		x4[i] = state[i + 12];
	}
	memset(temp, 0, 4 * sizeof(char));

	for (int i = 0; i < 4; i++)
		temp[i] = temp[i] ^ x2[i] ^ x3[i] ^ x4[i] ^ round_key[i];

	unsigned int temp_int = TFunctionVerification(temp);
	unsigned int x1_int = fourCharToInt(x1[0], x1[1], x1[2], x1[3]);

	free(x1);
	free(x2);
	free(x3);
	free(x4);
	free(temp);
	x1 = NULL;
	x2 = NULL;
	x3 = NULL;
	x4 = NULL;

	return temp_int ^ x1_int;
}
unsigned int TFunctionKE_Verification(char* input)
{
	char* SboxResult = (char*)malloc(4 * sizeof(char));
	for (int i = 0; i < 4; i++)
		SboxResult[i] = LookUpTable(input[i]);
	
	unsigned int SboxResultInt = fourCharToInt(SboxResult[0], SboxResult[1], SboxResult[2], SboxResult[3]);
	unsigned int Shift13 = left_shit_int(SboxResultInt, 13);
	unsigned int Shift23 = left_shit_int(SboxResultInt, 23);

	free(SboxResult);
	SboxResult = NULL;

	return SboxResultInt ^ Shift13 ^ Shift23;
}

unsigned int F_Function_KE_Verification(char* state, char* round_key)
{
	char* x1 = (char*)malloc(4 * sizeof(char));
	char* x2 = (char*)malloc(4 * sizeof(char));
	char* x3 = (char*)malloc(4 * sizeof(char));
	char* x4 = (char*)malloc(4 * sizeof(char));
	char* temp = (char*)malloc(4 * sizeof(char));

	for (int i = 0; i < 4; i++)
	{
		x1[i] = state[i];
		x2[i] = state[i + 4];
		x3[i] = state[i + 8];
		x4[i] = state[i + 12];
	}
	memset(temp, 0, 4 * sizeof(char));

	for (int i = 0; i < 4; i++)
		temp[i] = temp[i] ^ x2[i] ^ x3[i] ^ x4[i] ^ round_key[i];

	unsigned int temp_int = TFunctionKE_Verification(temp);
	unsigned int x1_int = fourCharToInt(x1[0], x1[1], x1[2], x1[3]);

	free(x1);
	free(x2);
	free(x3);
	free(x4);
	free(temp);
	x1 = NULL;
	x2 = NULL;
	x3 = NULL;
	x4 = NULL;

	return temp_int ^ x1_int;
}

void int_to_char(unsigned int val, char* output)
{
	unsigned int temp_int = val;
	char* temp = (char*)malloc(4 * sizeof(char));
	char* ptr = (char*)&temp_int;
	temp[3] = *ptr;
	temp[2] = *(ptr + 1);
	temp[1] = *(ptr + 2);
	temp[0] = *(ptr + 3);
	memcpy(output, temp, 4 * sizeof(char));
	free(temp);
	temp = NULL;
}

void ShiftRowVerification(char* input, char* output)
{
	char* temp_state = (char*)malloc(16 * sizeof(char));

	temp_state[0] = input[0];
	temp_state[4] = input[4];
	temp_state[8] = input[8];
	temp_state[12] = input[12];

	temp_state[1] = input[5];
	temp_state[5] = input[9];
	temp_state[9] = input[13];
	temp_state[13] = input[1];

	temp_state[2] = input[10];
	temp_state[6] = input[14];
	temp_state[10] = input[2];
	temp_state[14] = input[6];

	temp_state[3] = input[15];
	temp_state[7] = input[3];
	temp_state[11] = input[7];
	temp_state[15] = input[11];

	memcpy(output, temp_state, 16 * sizeof(char));
	free(temp_state);
	temp_state = NULL;

}

void ShiftRowVerification_inv(char* input, char* output)
{
	char* temp_state = (char*)malloc(16 * sizeof(char));

	temp_state[0] = input[0];
	temp_state[4] = input[4];
	temp_state[8] = input[8];
	temp_state[12] = input[12];

	temp_state[1] = input[13];
	temp_state[5] = input[1];
	temp_state[9] = input[5];
	temp_state[13] = input[9];

	temp_state[2] = input[10];
	temp_state[6] = input[14];
	temp_state[10] = input[2];
	temp_state[14] = input[6];

	temp_state[3] = input[7];
	temp_state[7] = input[11];
	temp_state[11] = input[15];
	temp_state[15] = input[3];

	memcpy(output, temp_state, 16 * sizeof(char));
	free(temp_state);
	temp_state = NULL;
}

void AddRoundKeyVerification(char* state, char* round_key, char* output)
{
	char* temp_output = (char*)malloc(16 * sizeof(char));
	for (int i = 0; i < 16; i++)
		temp_output[i] = state[i] ^ round_key[i];
	memcpy(output, temp_output, 16 * sizeof(char));
	free(temp_output);
	temp_output = NULL;
}

void ShiftCol_KE_Verification(char* state, char* output)
{
	char* temp_output = (char*)malloc(4 * sizeof(char));
	temp_output[0] = state[1];
	temp_output[1] = state[2];
	temp_output[2] = state[3];
	temp_output[3] = state[0];
	memcpy(output, temp_output, 4 * sizeof(char));
	free(temp_output);
	temp_output = NULL;
}

void BytesSub_KE_Verification(char* state, char* output)
{
	char* temp = (char*)malloc(4 * sizeof(char));
	for (int i = 0; i < 4; i++)
		temp[i] = LookUpTable_AES(state[i]);
	memcpy(output, temp, 4 * sizeof(char));
	free(temp);
	temp = NULL;
}
void XOR_RC_Verification(char* state, int round, char* output)
{
	char* RCi = (char*)malloc(4 * sizeof(char));
	char* temp_output = (char*)malloc(4 * sizeof(char));
	int_to_char(Rcon[round], RCi);
	for (int i = 0; i < 4; i++)
		temp_output[i] = RCi[i] ^ state[i];
	memcpy(output, temp_output, 4 * sizeof(char));
	free(RCi);
}

void AES_KE_T_Function_Verification(char* state, int round, char* output)
{
	char* ShiftRes = (char*)malloc(4 * sizeof(char));
	char* SboxRes = (char*)malloc(4 * sizeof(char));
	char* RconRes = (char*)malloc(4 * sizeof(char));

	ShiftCol_KE_Verification(state, ShiftRes);
	BytesSub_KE_Verification(ShiftRes, SboxRes);
	XOR_RC_Verification(SboxRes, round, RconRes);

	memcpy(output, RconRes, 4 * sizeof(char));

	free(ShiftRes);
	free(SboxRes);
	free(RconRes);
	ShiftRes = NULL;
	SboxRes = NULL;
	RconRes = NULL;
}
mat_GF2 Get_Inverse_Mat(mat_GF2 mat)
{
	GF2 d = GF2(0);
	mat_GF2 inv_mat;
	inv(d, inv_mat, mat);
	return inv_mat;
}

void PrintMatrix(mat_GF2 mat)
{
	unsigned int RowNum = mat.NumRows();
	unsigned int ColNum = mat.NumCols();
	if (ColNum == 1)
	{
		for (int i = 0; i < RowNum; i++)
			cout << mat[i][0] << " ";
		printf("\n");
		return;
	}
	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
			cout << mat[i][j] << " ";
		cout << "\n";
	}
	printf("\n");
}

void PrintVector(vec_GF2 vec)
{
	unsigned int length = vec.length();
	for (int i = 0; i < length; i++)
		cout << vec[i] << " ";
	printf("\n");
}

mat_GF2 GenerateGF2RandomMatrix(unsigned int dimension)
{
	mat_GF2 mat(INIT_SIZE, dimension, dimension);
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
			mat[i][j] = random_GF2();
	}
	return mat;
}

vec_GF2 GenerateGF2RandomVector(unsigned int Length)
{
	vec_GF2 vec(INIT_SIZE, Length);
	for (int i = 0; i < Length; i++)
		vec[i] = random_GF2();
	return vec;
}

mat_GF2 Generate_Random_Inverse_Matrix(unsigned int dimension)
{
	mat_GF2 temp1(INIT_SIZE, dimension, dimension);
	mat_GF2 temp1_inv(INIT_SIZE, dimension, dimension);
	GF2 d = GF2();
	while (true)
	{
		temp1 = GenerateGF2RandomMatrix(dimension);
		inv(d, temp1_inv, temp1);
		if (!IsZero(d)) break;
	}
	return temp1;
	temp1_inv.kill();
}

void Generate_2Random_Inverse_Matrix(unsigned int dimension, mat_GF2& mat1, mat_GF2& mat2)
{
	mat_GF2 temp1(INIT_SIZE, dimension, dimension);
	mat_GF2 temp1_inv(INIT_SIZE, dimension, dimension);
	GF2 d = GF2();
	while (true)
	{
		temp1 = GenerateGF2RandomMatrix(dimension);
		inv(d, temp1_inv, temp1);
		if (!IsZero(d)) break;
	}
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			mat1[i][j] = temp1[i][j];
			mat2[i][j] = temp1_inv[i][j];
		}
	}
	return;
}

char vec_GF2_to_char(vec_GF2& input)//8bit input to output char 
{
	long temp = 0;
	char temp_char = 0x00;
	for (int i = 0; i < 8; i++)
	{
		temp= rep(input[i]);
		temp_char = temp_char | ((temp & 0x01) << (7 - i));
	}
	return temp_char;
}

void char_to_vec_GF2(char input, vec_GF2& output)//input char to 8bit output
{
	vec_GF2 temp(INIT_SIZE, 8);
	for (int i = 0; i < 8; i++)
		temp[i] = (long)((input >> (7 - i)) & 0x01);

	output.move(temp);
	temp.kill();
}

uint64_t vec_GF2_to_u64(vec_GF2& input)
{
	uint64_t temp_u64 = 0;
	uint64_t shift = 0;
	int digit = 0;
	long temp = 0;
	for (int i = 0; i < 64; i++)
	{
		temp = rep(input[i]);
		digit = 63-i;
		shift = (uint64_t)(temp) << digit;
		temp_u64 = temp_u64 | shift;;
	}
	return temp_u64;
}

vec_GF2 u64_to_vec_GF2(uint64_t input)
{
	vec_GF2 temp(INIT_SIZE, 64);
	for (int i = 0; i < 64; i++)
		temp[i] = (long)((input >> (63 - i)) & 0x01);
	return temp;
}

uint32_t vec_GF2_to_u32(vec_GF2& input)
{
	uint32_t temp_u32 = 0;
	uint32_t shift = 0;
	int digit = 0;
	long temp = 0;
	for (int i = 0; i < 64; i++)
	{
		temp = rep(input[i]);
		digit = 63 - i;
		shift = (uint32_t)(temp) << digit;
		temp_u32 = temp_u32 | shift;;
	}
	return temp_u32;
}

vec_GF2 u32_to_vec_GF2(uint32_t input)
{
	vec_GF2 temp(INIT_SIZE, 32);
	for (int i = 0; i < 32; i++)
		temp[i] = (long)((input >> (31 - i)) & 0x01);
	return temp;
}

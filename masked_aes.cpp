#include"masked_aes.h"
char LookUpTable_AES(char input)
{
	unsigned int index = (unsigned int)input & 0x000000ff;
	char temp = Sbox_AES[index];
	return temp;
}
char LookUpTable_AES_inv(char input)
{
	unsigned int index = (unsigned int)input & 0x000000ff;
	return Sbox_AES_inv[index];
}

void BytesSub(char4* input, char4* output)
{
	char* temp_state = (char*)malloc(16 * sizeof(char));
	char* temp_res = (char*)malloc(16 * sizeof(char));
	char4* temp_res_char4 = (char4*)malloc(128 * sizeof(char4));
	Convert_to_char(input, temp_state);
	for (int i = 0; i < 16; i++)
		temp_res[i] = LookUpTable_AES(temp_state[i]);
	Convert_to_char4(temp_res, temp_res_char4);

	memcpy(output, temp_res_char4, 128 * sizeof(char4));

	free(temp_state);
	free(temp_res);
	free(temp_res_char4);
	temp_state = NULL;
	temp_res = NULL;
	temp_res_char4 = NULL;
}

void BytesSub_inv(char4* input, char4* output)
{
	char* temp_state = (char*)malloc(16 * sizeof(char));
	char* temp_res = (char*)malloc(16 * sizeof(char));
	char4* temp_res_char4 = (char4*)malloc(128 * sizeof(char4));
	Convert_to_char(input, temp_state);
	for (int i = 0; i < 16; i++)
		temp_res[i] = LookUpTable_AES_inv(temp_state[i]);
	Convert_to_char4(temp_res, temp_res_char4);

	memcpy(output, temp_res_char4, 128 * sizeof(char4));

	free(temp_state);
	free(temp_res);
	free(temp_res_char4);
	temp_state = NULL;
	temp_res = NULL;
	temp_res_char4 = NULL;
}

void ShiftRow(char4* state, char4* output_state)
{
	char4* temp_state = (char4*)malloc(128 * sizeof(char4));

	for (int i = 0; i < 8; i++)
	{
		//S0 remains unshifted
		temp_state[i].ra = state[i].ra;
		temp_state[i].rb = state[i].rb;
		temp_state[i].rc = state[i].rc;
		temp_state[i].x = state[i].x;
		//S4 ramins unshifted
		temp_state[8 * 4 + i].ra = state[8 * 4 + i].ra;
		temp_state[8 * 4 + i].rb = state[8 * 4 + i].rb;
		temp_state[8 * 4 + i].rc = state[8 * 4 + i].rc;
		temp_state[8 * 4 + i].x = state[8 * 4 + i].x;

		//S8 ramins unshifted
		temp_state[8 * 8 + i].ra = state[8 * 8 + i].ra;
		temp_state[8 * 8 + i].rb = state[8 * 8 + i].rb;
		temp_state[8 * 8 + i].rc = state[8 * 8 + i].rc;
		temp_state[8 * 8 + i].x = state[8 * 8 + i].x;
		//S12 ramins unshifted
		temp_state[8 * 12 + i].ra = state[8 * 12 + i].ra;
		temp_state[8 * 12 + i].rb = state[8 * 12 + i].rb;
		temp_state[8 * 12 + i].rc = state[8 * 12 + i].rc;
		temp_state[8 * 12 + i].x = state[8 * 12 + i].x;


		//S1=S5
		temp_state[8 * 1 + i].ra = state[8 * 5 + i].ra;
		temp_state[8 * 1 + i].rb = state[8 * 5 + i].rb;
		temp_state[8 * 1 + i].rc = state[8 * 5 + i].rc;
		temp_state[8 * 1 + i].x = state[8 * 5 + i].x;
		//S5=S9
		temp_state[8 * 5 + i].ra = state[8 * 9 + i].ra;
		temp_state[8 * 5 + i].rb = state[8 * 9 + i].rb;
		temp_state[8 * 5 + i].rc = state[8 * 9 + i].rc;
		temp_state[8 * 5 + i].x = state[8 * 9 + i].x;
		//S9=S13
		temp_state[8 * 9 + i].ra = state[8 * 13 + i].ra;
		temp_state[8 * 9 + i].rb = state[8 * 13 + i].rb;
		temp_state[8 * 9 + i].rc = state[8 * 13 + i].rc;
		temp_state[8 * 9 + i].x = state[8 * 13 + i].x;
		//S13=S1
		temp_state[8 * 13 + i].ra = state[8 * 1 + i].ra;
		temp_state[8 * 13 + i].rb = state[8 * 1 + i].rb;
		temp_state[8 * 13 + i].rc = state[8 * 1 + i].rc;
		temp_state[8 * 13 + i].x = state[8 * 1 + i].x;


		//S2=S10
		temp_state[8 * 2+ i].ra = state[8 * 10 + i].ra;
		temp_state[8 * 2 + i].rb = state[8 * 10 + i].rb;
		temp_state[8 * 2 + i].rc = state[8 * 10 + i].rc;
		temp_state[8 * 2 + i].x = state[8 * 10 + i].x;
		//S6=S14
		temp_state[8 * 6 + i].ra = state[8 * 14 + i].ra;
		temp_state[8 * 6 + i].rb = state[8 * 14 + i].rb;
		temp_state[8 * 6 + i].rc = state[8 * 14 + i].rc;
		temp_state[8 * 6 + i].x = state[8 * 14 + i].x;
		//S10=S2
		temp_state[8 *10 + i].ra = state[8 * 2 + i].ra;
		temp_state[8 * 10 + i].rb = state[8 * 2 + i].rb;
		temp_state[8 * 10 + i].rc = state[8 * 2 + i].rc;
		temp_state[8 * 10 + i].x = state[8 * 2 + i].x;
		//S14=S6
		temp_state[8 * 14 + i].ra = state[8 * 6 + i].ra;
		temp_state[8 * 14 + i].rb = state[8 * 6 + i].rb;
		temp_state[8 * 14 + i].rc = state[8 * 6 + i].rc;
		temp_state[8 * 14 + i].x = state[8 * 6 + i].x;


		//S3=S15
		temp_state[8 * 3 + i].ra = state[8 * 15 + i].ra;
		temp_state[8 * 3 + i].rb = state[8 * 15 + i].rb;
		temp_state[8 * 3 + i].rc = state[8 * 15 + i].rc;
		temp_state[8 * 3 + i].x = state[8 * 15 + i].x;
		//S7=S3
		temp_state[8 * 7 + i].ra = state[8 * 3 + i].ra;
		temp_state[8 * 7 + i].rb = state[8 * 3 + i].rb;
		temp_state[8 * 7 + i].rc = state[8 * 3 + i].rc;
		temp_state[8 * 7 + i].x = state[8 * 3 + i].x;
		//S11=S7
		temp_state[8 * 11 + i].ra = state[8 * 7 + i].ra;
		temp_state[8 * 11 + i].rb = state[8 * 7 + i].rb;
		temp_state[8 * 11 + i].rc = state[8 * 7+ i].rc;
		temp_state[8 * 11 + i].x = state[8 * 7 + i].x;
		//S15=S11
		temp_state[8 * 15 + i].ra = state[8 * 11 + i].ra;
		temp_state[8 * 15 + i].rb = state[8 * 11 + i].rb;
		temp_state[8 * 15 + i].rc = state[8 * 11 + i].rc;
		temp_state[8 * 15 + i].x = state[8 * 11 + i].x;
	}
	memcpy(output_state, temp_state, 128 * sizeof(char4));
	free(temp_state);
	temp_state = NULL;
}

void ShiftRow_inv(char4* state, char4* output_state)
{
	char4* temp_state = (char4*)malloc(128 * sizeof(char4));

	for (int i = 0; i < 8; i++)
	{
		//S0 remains unshifted
		temp_state[i].ra = state[i].ra;
		temp_state[i].rb = state[i].rb;
		temp_state[i].rc = state[i].rc;
		temp_state[i].x = state[i].x;
		//S4 ramins unshifted
		temp_state[8 * 4 + i].ra = state[8 * 4 + i].ra;
		temp_state[8 * 4 + i].rb = state[8 * 4 + i].rb;
		temp_state[8 * 4 + i].rc = state[8 * 4 + i].rc;
		temp_state[8 * 4 + i].x = state[8 * 4 + i].x;

		//S8 ramins unshifted
		temp_state[8 * 8 + i].ra = state[8 * 8 + i].ra;
		temp_state[8 * 8 + i].rb = state[8 * 8 + i].rb;
		temp_state[8 * 8 + i].rc = state[8 * 8 + i].rc;
		temp_state[8 * 8 + i].x = state[8 * 8 + i].x;
		//S12 ramins unshifted
		temp_state[8 * 12 + i].ra = state[8 * 12 + i].ra;
		temp_state[8 * 12 + i].rb = state[8 * 12 + i].rb;
		temp_state[8 * 12 + i].rc = state[8 * 12 + i].rc;
		temp_state[8 * 12 + i].x = state[8 * 12 + i].x;


		//S1=S5
		temp_state[8 * 1 + i].ra = state[8 * 13 + i].ra;
		temp_state[8 * 1 + i].rb = state[8 * 13 + i].rb;
		temp_state[8 * 1 + i].rc = state[8 * 13 + i].rc;
		temp_state[8 * 1 + i].x = state[8 * 13 + i].x;
		//S5=S9
		temp_state[8 * 5 + i].ra = state[8 * 1 + i].ra;
		temp_state[8 * 5 + i].rb = state[8 * 1 + i].rb;
		temp_state[8 * 5 + i].rc = state[8 * 1 + i].rc;
		temp_state[8 * 5 + i].x = state[8 * 1 + i].x;
		//S9=S13
		temp_state[8 * 9 + i].ra = state[8 * 5 + i].ra;
		temp_state[8 * 9 + i].rb = state[8 * 5 + i].rb;
		temp_state[8 * 9 + i].rc = state[8 * 5 + i].rc;
		temp_state[8 * 9 + i].x = state[8 * 5 + i].x;
		//S13=S1
		temp_state[8 * 13 + i].ra = state[8 * 9 + i].ra;
		temp_state[8 * 13 + i].rb = state[8 * 9 + i].rb;
		temp_state[8 * 13 + i].rc = state[8 * 9 + i].rc;
		temp_state[8 * 13 + i].x = state[8 * 9 + i].x;


		//S2=S10
		temp_state[8 * 2 + i].ra = state[8 * 10 + i].ra;
		temp_state[8 * 2 + i].rb = state[8 * 10 + i].rb;
		temp_state[8 * 2 + i].rc = state[8 * 10 + i].rc;
		temp_state[8 * 2 + i].x = state[8 * 10 + i].x;
		//S6=S14
		temp_state[8 * 6 + i].ra = state[8 * 14 + i].ra;
		temp_state[8 * 6 + i].rb = state[8 * 14 + i].rb;
		temp_state[8 * 6 + i].rc = state[8 * 14 + i].rc;
		temp_state[8 * 6 + i].x = state[8 * 14 + i].x;
		//S10=S2
		temp_state[8 * 10 + i].ra = state[8 * 2 + i].ra;
		temp_state[8 * 10 + i].rb = state[8 * 2 + i].rb;
		temp_state[8 * 10 + i].rc = state[8 * 2 + i].rc;
		temp_state[8 * 10 + i].x = state[8 * 2 + i].x;
		//S14=S6
		temp_state[8 * 14 + i].ra = state[8 * 6 + i].ra;
		temp_state[8 * 14 + i].rb = state[8 * 6 + i].rb;
		temp_state[8 * 14 + i].rc = state[8 * 6 + i].rc;
		temp_state[8 * 14 + i].x = state[8 * 6 + i].x;


		//S3=S15
		temp_state[8 * 3 + i].ra = state[8 * 7 + i].ra;
		temp_state[8 * 3 + i].rb = state[8 * 7 + i].rb;
		temp_state[8 * 3 + i].rc = state[8 * 7 + i].rc;
		temp_state[8 * 3 + i].x = state[8 * 7 + i].x;
		//S7=S3
		temp_state[8 * 7 + i].ra = state[8 * 11 + i].ra;
		temp_state[8 * 7 + i].rb = state[8 * 11 + i].rb;
		temp_state[8 * 7 + i].rc = state[8 * 11 + i].rc;
		temp_state[8 * 7 + i].x = state[8 * 11 + i].x;
		//S11=S7
		temp_state[8 * 11 + i].ra = state[8 * 15 + i].ra;
		temp_state[8 * 11 + i].rb = state[8 * 15 + i].rb;
		temp_state[8 * 11 + i].rc = state[8 * 15 + i].rc;
		temp_state[8 * 11 + i].x = state[8 * 15 + i].x;
		//S15=S11
		temp_state[8 * 15 + i].ra = state[8 * 3 + i].ra;
		temp_state[8 * 15 + i].rb = state[8 * 3 + i].rb;
		temp_state[8 * 15 + i].rc = state[8 * 3 + i].rc;
		temp_state[8 * 15 + i].x = state[8 * 3 + i].x;
	}
	memcpy(output_state, temp_state, 128 * sizeof(char4));
	free(temp_state);
	temp_state = NULL;
}


void AddRoundKey(char4* state, char4* round_key, char4* output)
{
	char4* temp_state = (char4*)malloc(128 * sizeof(char4));
	char4* temp_round_key = (char4*)malloc(128 * sizeof(char4));
	char4* temp_output = (char4*)malloc(128 * sizeof(char4));

	memcpy(temp_state, state, 128 * sizeof(char4));
	memcpy(temp_round_key, round_key, 128 * sizeof(char4));
	
	Eval_XOR(temp_state, temp_round_key, temp_output);
	memcpy(output, temp_output, 128 * sizeof(char4));

	free(temp_state);
	free(temp_round_key);
	free(temp_output);
	temp_state = NULL;
	temp_round_key = NULL;
	temp_output = NULL;
}

unsigned char Mult2(unsigned char num)
{
	unsigned char temp = num << 1;
	if ((num >> 7) & 0x01)
		temp = temp ^ 27;
	return temp;
}

unsigned char Mult3(unsigned char num)
{
	return Mult2(num) ^ num;
}

void MixColumns(unsigned char* state, unsigned char* output)
{

	int i;
	for (i = 0; i < 4; i++)
		output[4 * i] = Mult2(state[4 * i]) ^ Mult3(state[4 * i + 1]) ^ state[4 * i + 2] ^ state[4 * i + 3];
	for (i = 0; i < 4; i++)
		output[4 * i + 1] = state[4 * i] ^ Mult2(state[4 * i + 1]) ^ Mult3(state[4 * i + 2]) ^ state[4 * i + 3];
	for (i = 0; i < 4; i++)
		output[4 * i + 2] = state[4 * i] ^ state[4 * i + 1] ^ Mult2(state[4 * i + 2]) ^ Mult3(state[4 * i + 3]);
	for (i = 0; i < 4; i++)
		output[4 * i + 3] = Mult3(state[4 * i]) ^ state[4 * i + 1] ^ state[4 * i + 2] ^ Mult2(state[4 * i + 3]);
}

void MixColumnsRe(unsigned char* state, unsigned char *output)
{

	unsigned char a, b, c, d;
	for (int i = 0; i < 4; i++)
	{
		a = state[4 * i];
		b = state[4 * i + 1];
		c = state[4 * i + 2];
		d = state[4 * i + 3];
		output[4 * i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
		output[4 * i + 1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
		output[4 * i + 2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
		output[4 * i + 3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
	}
}

void MaskedMixColumns(char4* state, char4* output)
{
	char* temp_state = (char*)malloc(16 * sizeof(char));
	char* temp_output = (char*)malloc(16 * sizeof(char));
	char4* temp_output_char4 = (char4*)malloc(128 * sizeof(char4));

	Convert_to_char(state, temp_state);
	MixColumns((unsigned char*)temp_state, (unsigned char*)temp_output);
	Convert_to_char4(temp_output, temp_output_char4);
	memcpy(output, temp_output_char4, 128 * sizeof(char4));

	free(temp_state);
	free(temp_output);
	free(temp_output_char4);
	temp_state = NULL;
	temp_output = NULL;
	temp_output_char4 = NULL;
}

void MaskedMixColumnsRe(char4* state, char4* output)
{
	char* temp_state = (char*)malloc(16 * sizeof(char));
	char* temp_output = (char*)malloc(16 * sizeof(char));
	char4* temp_output_char4 = (char4*)malloc(128 * sizeof(char4));

	Convert_to_char(state, temp_state);
	MixColumnsRe((unsigned char*)temp_state, (unsigned char*)temp_output);
	Convert_to_char4(temp_output, temp_output_char4);
	memcpy(output, temp_output_char4, 128 * sizeof(char4));

	free(temp_state);
	free(temp_output);
	free(temp_output_char4);
	temp_state = NULL;
	temp_output = NULL;
	temp_output_char4 = NULL;
}

void ShiftCol_KE(char4* state, char4* output)
{
	char4* temp_output = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 24; i++)
	{
		temp_output[i].ra = state[i + 8].ra;
		temp_output[i].rb = state[i + 8].rb;
		temp_output[i].rc = state[i + 8].rc;
		temp_output[i].x = state[i + 8].x;
	}
	for (int i = 0; i < 8; i++)
	{
		temp_output[i + 24].ra = state[i].ra;
		temp_output[i + 24].rb = state[i].rb;
		temp_output[i + 24].rc = state[i].rc;
		temp_output[i + 24].x = state[i].x;

	}
	memcpy(output, temp_output, 32 * sizeof(char4));
	free(temp_output);
	temp_output = NULL;
}

void BytesSub_KE(char4* state, char4* output)
{
	char* temp_state = (char*)malloc(4 * sizeof(char));
	char* temp_res = (char*)malloc(4 * sizeof(char));
	char4* temp_output = (char4*)malloc(32 * sizeof(char4));
	Convert_to_char_32(state, temp_state);
	for (int i = 0; i < 4; i++)
		temp_res[i] = LookUpTable_AES(temp_state[i]);
	Convert_to_char4_32(temp_res, temp_output);
	memcpy(output, temp_output, 32 * sizeof(char4));

	free(temp_state);
	free(temp_res);
	free(temp_output);
	temp_state = NULL;
	temp_res = NULL;
	temp_output = NULL;
}

void XOR_RC(char4* state, int round, char4* output)
{
	char* RCi = (char*)malloc(4 * sizeof(char));
	char4* RCi_masked = (char4*)malloc(32 * sizeof(char4));
	char4* temp_output = (char4*)malloc(32 * sizeof(char4));
	int_to_char(Rcon[round], RCi);
	Convert_to_char4_32(RCi, RCi_masked);
	Eval_XOR_32(state, RCi_masked, temp_output);

	memcpy(output, temp_output, 32 * sizeof(char4));
	free(RCi);
	free(RCi_masked);
	free(temp_output);
	RCi = NULL;
	RCi_masked = NULL;
	temp_output = NULL;
}

void AES_KE_T_Function(char4* state,int round,  char4* output)
{
	char4* ShiftRes = (char4*)malloc(32 * sizeof(char4));
	char4* SboxRes = (char4*)malloc(32 * sizeof(char4));
	char4* RconRes = (char4*)malloc(32 * sizeof(char4));
	
	ShiftCol_KE(state, ShiftRes);
	BytesSub_KE(ShiftRes, SboxRes);
	XOR_RC(SboxRes, round, RconRes);

	memcpy(output, RconRes, 32 * sizeof(char4));

	free(ShiftRes);
	free(SboxRes);
	free(RconRes);
	ShiftRes = NULL;
	SboxRes = NULL;
	RconRes = NULL;

}

void AESKeyExpansion(char* key, char4** round_key)
{
	char4* MaskedKey = (char4*)malloc(128 * sizeof(char4));
	char4** temp_round_key = (char4**)malloc(44 * sizeof(char4*));
	char4* W4i_1 = (char4*)malloc(32 * sizeof(char4));
	char4* W4i_2 = (char4*)malloc(32 * sizeof(char4));
	char4* W4i_3 = (char4*)malloc(32 * sizeof(char4));
	char4* W4i_4 = (char4*)malloc(32 * sizeof(char4));

	char4* W4i_plus = (char4*)malloc(32 * sizeof(char4));
	char4* W4i_plus_1 = (char4*)malloc(32 * sizeof(char4));
	char4* W4i_plus_2 = (char4*)malloc(32 * sizeof(char4));
	char4* W4i_plus_3 = (char4*)malloc(32 * sizeof(char4));
	char4* TFunctionRes = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 44; i++)
		temp_round_key[i] = (char4*)malloc(32 * sizeof(char4));
	
	Convert_to_char4(key, MaskedKey);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			temp_round_key[i][j].ra = MaskedKey[i * 32 + j].ra;
			temp_round_key[i][j].rb = MaskedKey[i * 32 + j].rb;
			temp_round_key[i][j].rc = MaskedKey[i * 32 + j].rc;
			temp_round_key[i][j].x = MaskedKey[i * 32 + j].x;
		}
	}
	

	for (int i = 1; i <= 10; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			W4i_4[j].ra = temp_round_key[4 * i - 4][j].ra;
			W4i_4[j].rb = temp_round_key[4 * i - 4][j].rb;
			W4i_4[j].rc = temp_round_key[4 * i - 4][j].rc;
			W4i_4[j].x = temp_round_key[4 * i - 4][j].x;

			W4i_3[j].ra = temp_round_key[4 * i - 3][j].ra;
			W4i_3[j].rb = temp_round_key[4 * i - 3][j].rb;
			W4i_3[j].rc = temp_round_key[4 * i - 3][j].rc;
			W4i_3[j].x = temp_round_key[4 * i - 3][j].x;

			W4i_2[j].ra = temp_round_key[4 * i - 2][j].ra;
			W4i_2[j].rb = temp_round_key[4 * i - 2][j].rb;
			W4i_2[j].rc = temp_round_key[4 * i - 2][j].rc;
			W4i_2[j].x = temp_round_key[4 * i - 2][j].x;

			W4i_1[j].ra = temp_round_key[4 * i - 1][j].ra;
			W4i_1[j].rb = temp_round_key[4 * i - 1][j].rb;
			W4i_1[j].rc = temp_round_key[4 * i - 1][j].rc;
			W4i_1[j].x = temp_round_key[4 * i - 1][j].x;
		}

		AES_KE_T_Function(W4i_1, i - 1, TFunctionRes);


		Eval_XOR_32(W4i_4, TFunctionRes, W4i_plus); //W[4i-4] ^ T_result = W[4i]
		Eval_XOR_32(W4i_plus, W4i_3, W4i_plus_1);     //W[4i-3] ^ W[4i] = W[4i+1]
		Eval_XOR_32(W4i_plus_1, W4i_2, W4i_plus_2); //W[4i-2] ^ W[4i+1] = W[4i+2]
		Eval_XOR_32(W4i_plus_2, W4i_1, W4i_plus_3); //W[4i-1] ^ W[4i+2] = W[4i+3]

		/*print_char4(W4i_plus, 4);
		print_char4(W4i_plus_1, 4);
		print_char4(W4i_plus_2, 4);
		print_char4(W4i_plus_3, 4);

		printf("\n");*/

		for (int j = 0; j < 32; j++)
		{
			temp_round_key[4 * i][j].ra = W4i_plus[j].ra;
			temp_round_key[4 * i][j].rb = W4i_plus[j].rb;
			temp_round_key[4 * i][j].rc = W4i_plus[j].rc;
			temp_round_key[4 * i][j].x = W4i_plus[j].x;

			temp_round_key[4 * i + 1][j].ra = W4i_plus_1[j].ra;
			temp_round_key[4 * i + 1][j].rb = W4i_plus_1[j].rb;
			temp_round_key[4 * i + 1][j].rc = W4i_plus_1[j].rc;
			temp_round_key[4 * i + 1][j].x = W4i_plus_1[j].x;

			temp_round_key[4 * i + 2][j].ra = W4i_plus_2[j].ra;
			temp_round_key[4 * i + 2][j].rb = W4i_plus_2[j].rb;
			temp_round_key[4 * i + 2][j].rc = W4i_plus_2[j].rc;
			temp_round_key[4 * i + 2][j].x = W4i_plus_2[j].x;

			temp_round_key[4 * i + 3][j].ra = W4i_plus_3[j].ra;
			temp_round_key[4 * i + 3][j].rb = W4i_plus_3[j].rb;
			temp_round_key[4 * i + 3][j].rc = W4i_plus_3[j].rc;
			temp_round_key[4 * i + 3][j].x = W4i_plus_3[j].x;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			round_key[i][j].ra = temp_round_key[4 * (i + 1) + 0][j].ra;
			round_key[i][j].rb = temp_round_key[4 * (i + 1) + 0][j].rb;
			round_key[i][j].rc = temp_round_key[4 * (i + 1) + 0][j].rc;
			round_key[i][j].x = temp_round_key[4 * (i + 1) + 0][j].x;

			round_key[i][j + 32].ra = temp_round_key[4 * (i + 1) + 1][j].ra;
			round_key[i][j + 32].rb = temp_round_key[4 * (i + 1) + 1][j].rb;
			round_key[i][j + 32].rc = temp_round_key[4 * (i + 1) + 1][j].rc;
			round_key[i][j + 32].x = temp_round_key[4 * (i + 1) + 1][j].x;

			round_key[i][j + 64].ra = temp_round_key[4 * (i + 1) + 2][j].ra;
			round_key[i][j + 64].rb = temp_round_key[4 * (i + 1) + 2][j].rb;
			round_key[i][j + 64].rc = temp_round_key[4 * (i + 1) + 2][j].rc;
			round_key[i][j + 64].x = temp_round_key[4 * (i + 1) + 2][j].x;

			round_key[i][j + 96].ra = temp_round_key[4 * (i + 1) + 3][j].ra;
			round_key[i][j + 96].rb = temp_round_key[4 * (i + 1) + 3][j].rb;
			round_key[i][j + 96].rc = temp_round_key[4 * (i + 1) + 3][j].rc;
			round_key[i][j + 96].x = temp_round_key[4 * (i + 1) + 3][j].x;
		}
	}


	free(MaskedKey);
	for (int i = 0; i < 44; i++)
	{
		free(temp_round_key[i]);
		temp_round_key[i] = NULL;
	}
	free(temp_round_key);
	temp_round_key = NULL;
	free(W4i_1);
	free(W4i_2);
	free(W4i_3);
	free(W4i_4);

	W4i_1 = NULL;
	W4i_2 = NULL;
	W4i_3 = NULL;
	W4i_4 = NULL;

	free(W4i_plus);
	free(W4i_plus_1);
	free(W4i_plus_2);
	free(W4i_plus_3);
	free(TFunctionRes);
	W4i_plus = NULL;
	W4i_plus_1 = NULL;
	W4i_plus_2 = NULL;
	W4i_plus_3 = NULL;
	TFunctionRes = NULL;
}

void encryptAES(char* plaintext, char* key, char* ciphertext)
{
	char4* masked_state = (char4*)malloc(128 * sizeof(char4));
	char4* masked_key = (char4*)malloc(128 * sizeof(char4));
	char4* iteration_input = (char4*)malloc(128 * sizeof(char4));
	char4* iteration_output = (char4*)malloc(128 * sizeof(char4));
	char4* ShiftRowRes = (char4*)malloc(128 * sizeof(char4));
	char4* MixColRes = (char4*)malloc(128 * sizeof(char4));
	char4* SubBytesRes = (char4*)malloc(128 * sizeof(char4));
	char4* one_round_key = (char4*)malloc(128 * sizeof(char4));
	char4** round_key = (char4**)malloc(10 * sizeof(char4*));
	char* temp_ciphertext = (char*)malloc(16 * sizeof(char));
	for (int i = 0; i < 10; i++)
		round_key[i] = (char4*)malloc(128 * sizeof(char4));
	AESKeyExpansion(key, round_key);
	/*for (int i = 0; i < 10; i++)
		print_char4(round_key[i], 16);*/
	Convert_to_char4(plaintext, masked_state);
	Convert_to_char4(key, masked_key);

	AddRoundKey(masked_state, masked_key, iteration_input);
	for (int i = 0; i < 9; i++)
	{
		BytesSub(iteration_input, SubBytesRes);
		ShiftRow(SubBytesRes, ShiftRowRes);
		MaskedMixColumns(ShiftRowRes, MixColRes);
		print_char4(MixColRes, 16);
		for (int j = 0; j < 128; j++)
		{
			one_round_key[j].ra = round_key[i][j].ra;
			one_round_key[j].rb = round_key[i][j].rb;
			one_round_key[j].rc = round_key[i][j].rc;
			one_round_key[j].x = round_key[i][j].x;
		}
		AddRoundKey(MixColRes, one_round_key, iteration_output);
		memcpy(iteration_input, iteration_output, 128 * sizeof(char4));
	}
	BytesSub(iteration_input, SubBytesRes);
	ShiftRow(SubBytesRes, ShiftRowRes);
	for (int j = 0; j < 128; j++)
	{
		one_round_key[j].ra = round_key[9][j].ra;
		one_round_key[j].rb = round_key[9][j].rb;
		one_round_key[j].rc = round_key[9][j].rc;
		one_round_key[j].x = round_key[9][j].x;
	}
	AddRoundKey(ShiftRowRes, one_round_key, iteration_output);
	Convert_to_char(iteration_output, temp_ciphertext);
	memcpy(ciphertext, temp_ciphertext, 16 * sizeof(char));

	free(masked_state);
	free(masked_key);
	free(iteration_input);
	free(iteration_output);
	free(ShiftRowRes);
	free(MixColRes);
	free(SubBytesRes);
	free(one_round_key);
	free(temp_ciphertext);
	for (int i = 0; i < 10; i++)
	{
		free(round_key[i]);
		round_key[i] = NULL;
	}
	free(round_key);
	round_key = NULL;

	masked_state = NULL;
	masked_key = NULL;
	iteration_input = NULL;
	iteration_output = NULL;
	ShiftRowRes = NULL;
	MixColRes = NULL;
	SubBytesRes = NULL;
	one_round_key = NULL;
	temp_ciphertext = NULL;
}

void decryptAES(char* ciphertext, char* key, char* plaintext)
{
	char4* masked_state = (char4*)malloc(128 * sizeof(char4));
	char4* masked_key = (char4*)malloc(128 * sizeof(char4));
	char4* one_round_key = (char4*)malloc(128 * sizeof(char4));
	char4** round_key = (char4**)malloc(10 * sizeof(char4*));
	char4** round_key_reverse = (char4**)malloc(10 * sizeof(char4*));
	char4* iteration_input = (char4*)malloc(128 * sizeof(char4));
	char4* iteration_output = (char4*)malloc(128 * sizeof(char4));
	char4* AddRoundKeyRes = (char4*)malloc(128 * sizeof(char4));
	char4* ShiftRowRes = (char4*)malloc(128 * sizeof(char4));
	char4* SubBytesRes = (char4*)malloc(128 * sizeof(char4));
	char* temp_plaintext = (char*)malloc(16 * sizeof(char));
	for (int i = 0; i < 10; i++)
	{
		round_key[i] = (char4*)malloc(128 * sizeof(char4));
		round_key_reverse[i] = (char4*)malloc(128 * sizeof(char4));
	}

	Convert_to_char4(ciphertext, masked_state);
	Convert_to_char4(key, masked_key);

	AESKeyExpansion(key, round_key);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			round_key_reverse[i][j].ra = round_key[9 - i][j].ra;
			round_key_reverse[i][j].rb = round_key[9 - i][j].rb;
			round_key_reverse[i][j].rc = round_key[9 - i][j].rc;
			round_key_reverse[i][j].x = round_key[9 - i][j].x;
		}
	}

	for (int i = 0; i < 128; i++)
	{
		one_round_key[i].ra = round_key_reverse[0][i].ra;
		one_round_key[i].rb = round_key_reverse[0][i].rb;
		one_round_key[i].rc = round_key_reverse[0][i].rc;
		one_round_key[i].x = round_key_reverse[0][i].x;
	}

	AddRoundKey(masked_state, one_round_key, iteration_input);

	for (int i = 1; i < 10; i++)
	{
		ShiftRow_inv(iteration_input, ShiftRowRes);
		BytesSub_inv(ShiftRowRes, SubBytesRes);
		for (int j = 0; j < 128; j++)
		{
			one_round_key[j].ra = round_key_reverse[i][j].ra;
			one_round_key[j].rb = round_key_reverse[i][j].rb;
			one_round_key[j].rc = round_key_reverse[i][j].rc;
			one_round_key[j].x = round_key_reverse[i][j].x;
		}
		AddRoundKey(SubBytesRes, one_round_key, AddRoundKeyRes);
		MaskedMixColumnsRe(AddRoundKeyRes, iteration_output);
		memcpy(iteration_input, iteration_output, 128 * sizeof(char4));
	}

	ShiftRow_inv(iteration_output, ShiftRowRes);
	BytesSub_inv(ShiftRowRes, SubBytesRes);
	AddRoundKey(SubBytesRes, masked_key, iteration_output);

	Convert_to_char(iteration_output, temp_plaintext);
	memcpy(plaintext, temp_plaintext, 16 * sizeof(char));

	free(masked_state);
	free(masked_key);
	free(iteration_input);
	free(iteration_output);
	free(ShiftRowRes);
	free(SubBytesRes);
	free(one_round_key);
	free(temp_plaintext);
	free(AddRoundKeyRes);
	for (int i = 0; i < 10; i++)
	{
		free(round_key[i]);
		free(round_key_reverse[i]);
		round_key[i] = NULL;
		round_key_reverse[i] = NULL;
	}
	free(round_key);
	free(round_key_reverse);
	round_key = NULL;
	round_key_reverse = NULL;
	masked_state = NULL;
	masked_key = NULL;
	iteration_input = NULL;
	iteration_output = NULL;
	ShiftRowRes = NULL;
	SubBytesRes = NULL;
	one_round_key = NULL;
	temp_plaintext = NULL;
	AddRoundKeyRes = NULL;
}

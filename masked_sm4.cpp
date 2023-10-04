#include"masked_sm4.h"
#include"utils.h"
#include<string>
#include<random>
void Refresh(char4* input, char4* refresh_result)
{
	char4* random_bits = (char4*)malloc(128 * sizeof(char4));
	char4* temp = (char4*)malloc(128 * sizeof(char4));

	for (int i = 0; i < 128; i++)
	{
		random_bits[i].ra = input[i].ra;
		random_bits[i].rb = input[i].rb;
		random_bits[i].rc = input[i].rc;
		random_bits[i].x = 0x00;

	}

	for (int i = 0; i < 128; i++)//accoring to the sequence of masked input, ra=a, rb=b, rc=c, x=d
	{
		temp[i].ra = random_bits[i].ra ^ input[i].ra;
		temp[i].rb = random_bits[i].rb ^ input[i].rb;
		temp[i].rc = random_bits[i].rc ^ input[i].rc;

		temp[i].x = (temp[i].ra) & (temp[i].rb) & (temp[i].rc) ^ (input[i].ra & input[i].rb & input[i].rc) ^ (input[i].x);
	}

	memcpy(refresh_result, temp, 128 * sizeof(char4));
	free(temp);
	free(random_bits);
	temp = NULL;
	random_bits = NULL;
}

void Eval_XOR(char4* input1, char4* input2, char4* xor_result)
{
	char4* temp_abcd = (char4*)malloc(128 * sizeof(char4));
	char4* temp_efgh = (char4*)malloc(128 * sizeof(char4));
	char4* temp_res = (char4*)malloc(128 * sizeof(char4));



	Refresh(input1, temp_abcd);
	Refresh(input2, temp_efgh);

	
	

	for (int i = 0; i < 128; i++)
	{
		temp_res[i].ra = temp_abcd[i].ra ^ temp_efgh[i].ra; //X = a^e
		temp_res[i].rb = temp_abcd[i].rb ^ temp_efgh[i].rb;
		temp_res[i].rc = temp_abcd[i].rc ^ temp_efgh[i].rc;
		temp_res[i].x = (temp_abcd[i].x) ^ (temp_efgh[i].x) ^ (temp_abcd[i].ra & temp_abcd[i].rb & temp_abcd[i].rc) ^ (temp_efgh[i].ra & temp_efgh[i].rb & temp_efgh[i].rc) ^ (temp_res[i].ra & temp_res[i].rb & temp_res[i].rc);
	}

	memcpy(xor_result, temp_res, 128 * sizeof(char4));

	free(temp_abcd);
	free(temp_efgh);
	free(temp_res);
	temp_abcd = NULL;
	temp_efgh = NULL;
	temp_res = NULL;
}
void Eval_AND(char4* input1, char4* input2, char4* and_result)
{
	char4* temp_abcd = (char4*)malloc(128 * sizeof(char4));
	char4* temp_efgh = (char4*)malloc(128 * sizeof(char4));
	char4* temp_res = (char4*)malloc(128 * sizeof(char4));

	Refresh(input1, temp_abcd);
	Refresh(input2, temp_efgh);


	for (int i = 0; i < 128; i++)
	{
		temp_res[i].ra = (temp_abcd[i].ra & temp_efgh[i].ra)^input2[i].ra; //X = ae ^ re
		temp_res[i].rb = (temp_abcd[i].rb & temp_efgh[i].rb) ^ input1[i].rb;
		temp_res[i].rc = (temp_abcd[i].rc & temp_efgh[i].rc) ^ input2[i].ra ^ input1[i].rb;
		temp_res[i].x = (temp_abcd[i].x ^ (temp_abcd[i].ra & temp_abcd[i].rb & temp_abcd[i].rc)) & (temp_efgh[i].x ^ (temp_efgh[i].ra & temp_efgh[i].rb & temp_efgh[i].rc)) ^ (temp_res[i].ra & temp_res[i].rb & temp_res[i].rc);
		
	}

	memcpy(and_result, temp_res, 128 * sizeof(char4));

	free(temp_abcd);
	free(temp_efgh);
	free(temp_res);
	temp_abcd = NULL;
	temp_efgh = NULL;
	temp_res = NULL;
}
void Refresh32(char4* input, char4* refresh_result)
{
	char4* random_bits = (char4*)malloc(32 * sizeof(char4));
	char4* temp = (char4*)malloc(32 * sizeof(char4));

	for (int i = 0; i < 32; i++)
	{
		random_bits[i].ra = input[i].ra;
		random_bits[i].rb = input[i].rb;
		random_bits[i].rc = input[i].rc;
		random_bits[i].x = 0x00;

	}

	for (int i = 0; i < 32; i++)//accoring to the sequence of masked input, ra=a, rb=b, rc=c, x=d
	{
		temp[i].ra = random_bits[i].ra ^ input[i].ra;
		temp[i].rb = random_bits[i].rb ^ input[i].rb;
		temp[i].rc = random_bits[i].rc ^ input[i].rc;

		temp[i].x = (temp[i].ra) & (temp[i].rb) & (temp[i].rc) ^ (input[i].ra & input[i].rb & input[i].rc) ^ (input[i].x);
	}

	memcpy(refresh_result, temp, 32 * sizeof(char4));
	free(temp);
	free(random_bits);
	temp = NULL;
	random_bits = NULL;
}
void Eval_XOR_32(char4* input1, char4* input2, char4* xor_result)
{
	char4* temp_abcd = (char4*)malloc(32 * sizeof(char4));
	char4* temp_efgh = (char4*)malloc(32 * sizeof(char4));
	char4* temp_res = (char4*)malloc(32 * sizeof(char4));



	Refresh32(input1, temp_abcd);
	Refresh32(input2, temp_efgh);




	for (int i = 0; i < 32; i++)
	{
		temp_res[i].ra = temp_abcd[i].ra ^ temp_efgh[i].ra; //X = a^e
		temp_res[i].rb = temp_abcd[i].rb ^ temp_efgh[i].rb;
		temp_res[i].rc = temp_abcd[i].rc ^ temp_efgh[i].rc;
		temp_res[i].x = (temp_abcd[i].x) ^ (temp_efgh[i].x) ^ (temp_abcd[i].ra & temp_abcd[i].rb & temp_abcd[i].rc) ^ (temp_efgh[i].ra & temp_efgh[i].rb & temp_efgh[i].rc) ^ (temp_res[i].ra & temp_res[i].rb & temp_res[i].rc);
	}

	memcpy(xor_result, temp_res, 32 * sizeof(char4));

	free(temp_abcd);
	free(temp_efgh);
	free(temp_res);
	temp_abcd = NULL;
	temp_efgh = NULL;
	temp_res = NULL;
}

void Eval_AND_32(char4* input1, char4* input2, char4* and_result)
{
	char4* temp_abcd = (char4*)malloc(32 * sizeof(char4));
	char4* temp_efgh = (char4*)malloc(32 * sizeof(char4));
	char4* temp_res = (char4*)malloc(32 * sizeof(char4));

	Refresh32(input1, temp_abcd);
	Refresh32(input2, temp_efgh);


	for (int i = 0; i < 32; i++)
	{
		temp_res[i].ra = (temp_abcd[i].ra & temp_efgh[i].ra) ^ input2[i].ra; //X = ae ^ re
		temp_res[i].rb = (temp_abcd[i].rb & temp_efgh[i].rb) ^ input1[i].rb;
		temp_res[i].rc = (temp_abcd[i].rc & temp_efgh[i].rc) ^ input2[i].ra ^ input1[i].rb;
		temp_res[i].x = (temp_abcd[i].x ^ (temp_abcd[i].ra & temp_abcd[i].rb & temp_abcd[i].rc)) & (temp_efgh[i].x ^ (temp_efgh[i].ra & temp_efgh[i].rb & temp_efgh[i].rc)) ^ (temp_res[i].ra & temp_res[i].rb & temp_res[i].rc);

	}

	memcpy(and_result, temp_res, 32 * sizeof(char4));

	free(temp_abcd);
	free(temp_efgh);
	free(temp_res);
	temp_abcd = NULL;
	temp_efgh = NULL;
	temp_res = NULL;
}

char LookUpTable(char input)
{
	unsigned int index = (unsigned int)input &0x000000ff;
	return SboxTable[index];
}

void Sbox(char4* SboxInput, char4* SboxOutput)
{
	char* temp1 = (char*)malloc(4 * sizeof(char));
	char* temp2 = (char*)malloc(4 * sizeof(char));
	Convert_to_char_32(SboxInput, temp1);
	for (int i = 0; i < 4; i++)
		temp2[i] = LookUpTable(temp1[i]);
	Convert_to_char4_32(temp2, SboxOutput);

	free(temp1);
	free(temp2);

	temp1 = NULL;
	temp2 = NULL;
}

void T_Function(char4* input, char4* output)
{
	char4* SboxResult = (char4*)malloc(32 * sizeof(char4));
	char4* Shift2 = (char4*)malloc(32 * sizeof(char4));
	char4* Shift10 = (char4*)malloc(32 * sizeof(char4));
	char4* Shift18 = (char4*)malloc(32 * sizeof(char4));
	char4* Shift24 = (char4*)malloc(32 * sizeof(char4));
	char4* temp = (char4*)malloc(32 * sizeof(char4));

	Sbox(input, SboxResult);

	
	LeftShift32(SboxResult, Shift2, 2);
	LeftShift32(SboxResult, Shift10, 10);
	LeftShift32(SboxResult, Shift18, 18);
	LeftShift32(SboxResult, Shift24, 24);


	Eval_XOR_32(SboxResult, Shift2, temp);//B^(B<<2)
	Eval_XOR_32(temp, Shift10, temp);
	Eval_XOR_32(temp, Shift18, temp);
	Eval_XOR_32(temp, Shift24, temp);

	memcpy(output, temp, 32 * sizeof(char4));

	free(SboxResult);
	free(Shift2);
	free(Shift10);
	free(Shift18);
	free(Shift24);
	free(temp);
	SboxResult = NULL;
	Shift2 = NULL;
	Shift10 = NULL;
	Shift18 = NULL;
	Shift24 = NULL;
	temp = NULL;

}

void F_Function(char4* state, char4* round_key, char4* output)
{
	char4* temp = (char4*)malloc(32 * sizeof(char4));
	char4* x1 = (char4*)malloc(32 * sizeof(char4));
	char4* x2 = (char4*)malloc(32 * sizeof(char4));
	char4* x3 = (char4*)malloc(32 * sizeof(char4));
	char4* x4 = (char4*)malloc(32 * sizeof(char4));

	for (int i = 0; i < 32; i++)
	{
		x1[i].ra = state[i].ra;
		x1[i].rb = state[i].rb;
		x1[i].rc = state[i].rc;
		x1[i].x = state[i].x;

		x2[i].ra = state[i + 32].ra;
		x2[i].rb = state[i + 32].rb;
		x2[i].rc = state[i + 32].rc;
		x2[i].x = state[i + 32].x;

		x3[i].ra = state[i + 64].ra;
		x3[i].rb = state[i + 64].rb;
		x3[i].rc = state[i + 64].rc;
		x3[i].x = state[i + 64].x;

		x4[i].ra = state[i + 96].ra;
		x4[i].rb = state[i + 96].rb;
		x4[i].rc = state[i + 96].rc;
		x4[i].x = state[i + 96].x;
	}
	Eval_XOR_32(x2, x3, temp);
	Eval_XOR_32(temp, x4, temp);
	Eval_XOR_32(temp, round_key, temp);
	//print_char4(temp, 4);
	T_Function(temp, temp);
	//print_char4(temp, 4);
	Eval_XOR_32(temp, x1, temp);
	memcpy(output, temp, 32 * sizeof(char4));


	free(temp);
	free(x1);
	free(x2);
	free(x3);
	free(x4);
	temp = NULL;
	x1 = NULL;
	x2 = NULL;
	x3 = NULL;
	x4 = NULL;
}

void T_Funtion_KE(char4* input, char4* output)
{
	char4* SboxResult = (char4*)malloc(32 * sizeof(char4));
	char4* Shift13 = (char4*)malloc(32 * sizeof(char4));
	char4* Shift23 = (char4*)malloc(32 * sizeof(char4));
	char4* temp = (char4*)malloc(32 * sizeof(char4));

	Sbox(input, SboxResult);
	LeftShift32(SboxResult, Shift13, 13);
	LeftShift32(SboxResult, Shift23, 23);

	
	Eval_XOR_32(SboxResult, Shift13, temp);
	Eval_XOR_32(temp, Shift23, temp);

	memcpy(output, temp, 32 * sizeof(char4));

	free(SboxResult);
	free(Shift13);
	free(Shift23);
	free(temp);
	SboxResult = NULL;
	Shift13 = NULL;
	Shift23 = NULL;
	temp = NULL;
}

void F_Function_KE(char4* state, char4* round_key, char4* output)
{
	char4* temp = (char4*)malloc(32 * sizeof(char4));
	char4* x1 = (char4*)malloc(32 * sizeof(char4));
	char4* x2 = (char4*)malloc(32 * sizeof(char4));
	char4* x3 = (char4*)malloc(32 * sizeof(char4));
	char4* x4 = (char4*)malloc(32 * sizeof(char4));

	for (int i = 0; i < 32; i++)
	{
		x1[i].ra = state[i].ra;
		x1[i].rb = state[i].rb;
		x1[i].rc = state[i].rc;
		x1[i].x = state[i].x;

		x2[i].ra = state[i + 32].ra;
		x2[i].rb = state[i + 32].rb;
		x2[i].rc = state[i + 32].rc;
		x2[i].x = state[i + 32].x;

		x3[i].ra = state[i + 64].ra;
		x3[i].rb = state[i + 64].rb;
		x3[i].rc = state[i + 64].rc;
		x3[i].x = state[i + 64].x;

		x4[i].ra = state[i + 96].ra;
		x4[i].rb = state[i + 96].rb;
		x4[i].rc = state[i + 96].rc;
		x4[i].x = state[i + 96].x;
	}
	Eval_XOR_32(x2, x3, temp);
	Eval_XOR_32(temp, x4, temp);
	Eval_XOR_32(temp, round_key, temp);
	T_Funtion_KE(temp, temp);
	Eval_XOR_32(temp, x1, temp);
	memcpy(output, temp, 32 * sizeof(char4));


	free(temp);
	free(x1);
	free(x2);
	free(x3);
	free(x4);
	temp = NULL;
	x1 = NULL;
	x2 = NULL;
	x3 = NULL;
	x4 = NULL;
}


void KeyExpansion(char* key, char4** round_key)
{
	char* K = (char*)malloc(16 * sizeof(char));
	char* CK_char = (char*)malloc(4 * sizeof(char));
	char4** round_key_temp = (char4**)malloc(36 * sizeof(char4*));
	char4* temp_state = (char4*)malloc(128 * sizeof(char4));
	char4* temp_CK = (char4*)malloc(32 * sizeof(char4));
	char4* temp_rk = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 36; i++)
		round_key_temp[i] = (char4*)malloc(32 * sizeof(char4));

	for (int i = 0; i < 4; i++)
	{
		K[i] = key[i] ^ FK0[i];
		K[i + 4] = key[i + 4] ^ FK1[i];
		K[i + 8] = key[i + 8] ^ FK2[i];
		K[i + 12] = key[i + 12] ^ FK3[i];
	}
	Convert_to_char4_32(K, round_key_temp[0]);
	Convert_to_char4_32(K + 4, round_key_temp[1]);
	Convert_to_char4_32(K + 8, round_key_temp[2]);
	Convert_to_char4_32(K + 12, round_key_temp[3]);

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			temp_state[j].ra = round_key_temp[i][j].ra;
			temp_state[j].rb = round_key_temp[i][j].rb;
			temp_state[j].rc = round_key_temp[i][j].rc;
			temp_state[j].x = round_key_temp[i][j].x;

			temp_state[j + 32].ra = round_key_temp[i + 1][j].ra;
			temp_state[j + 32].rb = round_key_temp[i + 1][j].rb;
			temp_state[j + 32].rc = round_key_temp[i + 1][j].rc;
			temp_state[j + 32].x = round_key_temp[i + 1][j].x;

			temp_state[j + 64].ra = round_key_temp[i + 2][j].ra;
			temp_state[j + 64].rb = round_key_temp[i + 2][j].rb;
			temp_state[j + 64].rc = round_key_temp[i + 2][j].rc;
			temp_state[j + 64].x = round_key_temp[i + 2][j].x;

			temp_state[j + 96].ra = round_key_temp[i + 3][j].ra;
			temp_state[j + 96].rb = round_key_temp[i + 3][j].rb;
			temp_state[j + 96].rc = round_key_temp[i + 3][j].rc;
			temp_state[j + 96].x = round_key_temp[i + 3][j].x;
		}

		int_to_char(CK[i], CK_char);
		Convert_to_char4_32(CK_char, temp_CK);
		F_Function_KE(temp_state, temp_CK, temp_rk);

		for (int j = 0; j < 32; j++)
		{
			round_key_temp[i + 4][j].ra = temp_rk[j].ra;
			round_key_temp[i + 4][j].rb = temp_rk[j].rb;
			round_key_temp[i + 4][j].rc = temp_rk[j].rc;
			round_key_temp[i + 4][j].x = temp_rk[j].x;
		}
		
	}

	for (int i = 0; i < 32; i++)
		memcpy(round_key[i], round_key_temp[i + 4], 32 * sizeof(char4));

	free(K);
	free(CK_char);
	for (int i = 0; i < 36; i++)
	{
		free(round_key_temp[i]);
		round_key_temp[i] = NULL;
	}
	free(round_key_temp);
	free(temp_state);
	free(temp_CK);
	free(temp_rk);
	
	K = NULL;
	CK_char = NULL;
	round_key_temp = NULL;
	temp_state = NULL;
	temp_CK = NULL;
	temp_rk = NULL;
}

void encryptSM4(char* plaintext, char* key, char* ciphertext)
{
	char* FirstFour = (char*)malloc(16 * sizeof(char));
	char4** round_key = (char4**)malloc(32 * sizeof(char*));
	char4** temp_ciphertext = (char4**)malloc(36 * sizeof(char4*));
	char4* temp_state = (char4*)malloc(128 * sizeof(char4));
	char4* temp_xi = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 32; i++)
		round_key[i] = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 36; i++)
		temp_ciphertext[i] = (char4*)malloc(32 * sizeof(char4));

	KeyExpansion(key, round_key);
	
	for (int i = 0; i < 4; i++)
	{
		FirstFour[i] = plaintext[i];
		FirstFour[i + 4] = plaintext[i + 4];
		FirstFour[i + 8] = plaintext[i + 8];
		FirstFour[i + 12] = plaintext[i + 12];
	}

	Convert_to_char4_32(FirstFour, temp_ciphertext[0]);
	Convert_to_char4_32(FirstFour + 4, temp_ciphertext[1]);
	Convert_to_char4_32(FirstFour + 8, temp_ciphertext[2]);
	Convert_to_char4_32(FirstFour + 12, temp_ciphertext[3]);


	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			temp_state[j].ra = temp_ciphertext[i][j].ra;
			temp_state[j].rb = temp_ciphertext[i][j].rb;
			temp_state[j].rc = temp_ciphertext[i][j].rc;
			temp_state[j].x = temp_ciphertext[i][j].x;

			temp_state[j + 32].ra = temp_ciphertext[i + 1][j].ra;
			temp_state[j + 32].rb = temp_ciphertext[i + 1][j].rb;
			temp_state[j + 32].rc = temp_ciphertext[i + 1][j].rc;
			temp_state[j + 32].x = temp_ciphertext[i + 1][j].x;

			temp_state[j + 64].ra = temp_ciphertext[i + 2][j].ra;
			temp_state[j + 64].rb = temp_ciphertext[i + 2][j].rb;
			temp_state[j + 64].rc = temp_ciphertext[i + 2][j].rc;
			temp_state[j + 64].x = temp_ciphertext[i + 2][j].x;

			temp_state[j + 96].ra = temp_ciphertext[i + 3][j].ra;
			temp_state[j + 96].rb = temp_ciphertext[i + 3][j].rb;
			temp_state[j + 96].rc = temp_ciphertext[i + 3][j].rc;
			temp_state[j + 96].x = temp_ciphertext[i + 3][j].x;
		}

		
		F_Function(temp_state, round_key[i], temp_xi);

		//print_char4(temp_xi, 4);

		for (int j = 0; j < 32; j++)
		{
			temp_ciphertext[i + 4][j].ra = temp_xi[j].ra;
			temp_ciphertext[i + 4][j].rb = temp_xi[j].rb;
			temp_ciphertext[i + 4][j].rc = temp_xi[j].rc;
			temp_ciphertext[i + 4][j].x = temp_xi[j].x;
		}
		
	}

	memcpy(temp_state, temp_ciphertext[35], 32 * sizeof(char4));
	memcpy(temp_state + 32, temp_ciphertext[34], 32 * sizeof(char4));
	memcpy(temp_state + 64, temp_ciphertext[33], 32 * sizeof(char4));
	memcpy(temp_state + 96, temp_ciphertext[32], 32 * sizeof(char4));
	Convert_to_char(temp_state, FirstFour);
	memcpy(ciphertext, FirstFour, 16 * sizeof(char));

	free(FirstFour);
	free(temp_state);
	free(temp_xi);
	FirstFour = NULL;
	temp_state = NULL;
	temp_xi = NULL;

	for (int i = 0; i < 36; i++)
	{
		free(temp_ciphertext[i]);
		temp_ciphertext[i] = NULL;
	}
	for (int i = 0; i < 32; i++)
	{
		free(round_key[i]);
		round_key[i] = NULL;
	}
	free(temp_ciphertext);
	free(round_key);

	temp_ciphertext = NULL;
	round_key = NULL;
}

void decryptSM4(char* plaintext, char* key, char* ciphertext)
{
	char* FirstFour = (char*)malloc(16 * sizeof(char));
	char4** round_key = (char4**)malloc(32 * sizeof(char*));
	char4** round_key_reverse = (char4**)malloc(32 * sizeof(char*));
	char4** temp_ciphertext = (char4**)malloc(36 * sizeof(char4*));
	char4* temp_state = (char4*)malloc(128 * sizeof(char4));
	char4* temp_xi = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 32; i++)
		round_key[i] = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 36; i++)
		temp_ciphertext[i] = (char4*)malloc(32 * sizeof(char4));
	for (int i = 0; i < 32; i++)
		round_key_reverse[i] = (char4*)malloc(32 * sizeof(char4));

	KeyExpansion(key, round_key);

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			round_key_reverse[i][j].ra = round_key[31 - i][j].ra;
			round_key_reverse[i][j].rb = round_key[31 - i][j].rb;
			round_key_reverse[i][j].rc = round_key[31 - i][j].rc;
			round_key_reverse[i][j].x = round_key[31 - i][j].x;
		}
	}

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			round_key[i][j].ra = round_key_reverse[i][j].ra;
			round_key[i][j].rb = round_key_reverse[i][j].rb;
			round_key[i][j].rc = round_key_reverse[i][j].rc;
			round_key[i][j].x = round_key_reverse[i][j].x;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		FirstFour[i] = plaintext[i];
		FirstFour[i + 4] = plaintext[i + 4];
		FirstFour[i + 8] = plaintext[i + 8];
		FirstFour[i + 12] = plaintext[i + 12];
	}

	Convert_to_char4_32(FirstFour, temp_ciphertext[0]);
	Convert_to_char4_32(FirstFour + 4, temp_ciphertext[1]);
	Convert_to_char4_32(FirstFour + 8, temp_ciphertext[2]);
	Convert_to_char4_32(FirstFour + 12, temp_ciphertext[3]);


	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			temp_state[j].ra = temp_ciphertext[i][j].ra;
			temp_state[j].rb = temp_ciphertext[i][j].rb;
			temp_state[j].rc = temp_ciphertext[i][j].rc;
			temp_state[j].x = temp_ciphertext[i][j].x;

			temp_state[j + 32].ra = temp_ciphertext[i + 1][j].ra;
			temp_state[j + 32].rb = temp_ciphertext[i + 1][j].rb;
			temp_state[j + 32].rc = temp_ciphertext[i + 1][j].rc;
			temp_state[j + 32].x = temp_ciphertext[i + 1][j].x;

			temp_state[j + 64].ra = temp_ciphertext[i + 2][j].ra;
			temp_state[j + 64].rb = temp_ciphertext[i + 2][j].rb;
			temp_state[j + 64].rc = temp_ciphertext[i + 2][j].rc;
			temp_state[j + 64].x = temp_ciphertext[i + 2][j].x;

			temp_state[j + 96].ra = temp_ciphertext[i + 3][j].ra;
			temp_state[j + 96].rb = temp_ciphertext[i + 3][j].rb;
			temp_state[j + 96].rc = temp_ciphertext[i + 3][j].rc;
			temp_state[j + 96].x = temp_ciphertext[i + 3][j].x;
		}


		F_Function(temp_state, round_key[i], temp_xi);

		for (int j = 0; j < 32; j++)
		{
			temp_ciphertext[i + 4][j].ra = temp_xi[j].ra;
			temp_ciphertext[i + 4][j].rb = temp_xi[j].rb;
			temp_ciphertext[i + 4][j].rc = temp_xi[j].rc;
			temp_ciphertext[i + 4][j].x = temp_xi[j].x;
		}
	}

	memcpy(temp_state, temp_ciphertext[35], 32 * sizeof(char4));
	memcpy(temp_state + 32, temp_ciphertext[34], 32 * sizeof(char4));
	memcpy(temp_state + 64, temp_ciphertext[33], 32 * sizeof(char4));
	memcpy(temp_state + 96, temp_ciphertext[32], 32 * sizeof(char4));
	Convert_to_char(temp_state, FirstFour);
	memcpy(ciphertext, FirstFour, 16 * sizeof(char));

	free(FirstFour);
	free(temp_state);
	free(temp_xi);

	FirstFour = NULL;
	temp_state = NULL;
	temp_xi = NULL;

	for (int i = 0; i < 36; i++)
	{
		free(temp_ciphertext[i]);
		temp_ciphertext[i] = NULL;
	}
	for (int i = 0; i < 32; i++)
	{
		free(round_key[i]);
		round_key[i] = NULL;
	}
	for (int i = 0; i < 32; i++)
	{
		free(round_key_reverse[i]);
		round_key_reverse[i] = NULL;
	}
	free(temp_ciphertext);
	free(round_key);
	free(round_key_reverse);

	temp_ciphertext = NULL;
	round_key = NULL;
}
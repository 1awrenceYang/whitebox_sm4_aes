#include"look_up_table_sm4.h"

void NewSbox(vec_GF2& input, vec_GF2&rk, vec_GF2& output)//输入之前要经过解编码，默认进来的是解除编码后的结果
{
	vec_GF2 aij = GenerateGF2RandomVector(8);
	vec_GF2 useful_info = input + rk;
	vec_GF2 confuse_info = input + aij;
	vec_GF2 temp_output1(INIT_SIZE, 8);
	vec_GF2 temp_output2(INIT_SIZE, 8);

	char useful_info_char = vec_GF2_to_char(useful_info);
	char confuse_info_char = vec_GF2_to_char(confuse_info);

	char useful_sbox_res = LookUpTable(useful_info_char);//g
	char confuse_sbox_res = LookUpTable(confuse_info_char);//h

	char_to_vec_GF2(useful_sbox_res, temp_output1);
	char_to_vec_GF2(confuse_sbox_res, temp_output2);

	for (int i = 0; i < 8; i++)
	{
		output[i] = temp_output1[i];
		output[i + 8] = temp_output2[i];
	}
	aij.kill();
	useful_info.kill();
	confuse_info.kill();
	temp_output1.kill();
	temp_output2.kill();
}

std::map<char, uint64_t> BuildOneSboxTable(int num, int tij)
{
	std::map<char, uint64_t> Table;
	mat_GF2 GF2_B1(INIT_SIZE, 8, 8);
	mat_GF2 GF2_B2(INIT_SIZE, 8, 8);
	mat_GF2 GF2_B3(INIT_SIZE, 8, 8);

	mat_GF2 B1g(INIT_SIZE, 8, 1);
	mat_GF2 B2g(INIT_SIZE, 8, 1);
	mat_GF2 B3g(INIT_SIZE, 8, 1);

	mat_GF2 B1h(INIT_SIZE, 8, 1);
	mat_GF2 B2h(INIT_SIZE, 8, 1);
	mat_GF2 B3h(INIT_SIZE, 8, 1);

	

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			GF2_B1[i][j] = B1[i][j];
			GF2_B2[i][j] = B2[i][j];
			GF2_B3[i][j] = B3[i][j];
		}
	}

	for (int i = 0; i < 256; i++)
	{
		char input = (char)(i & 0xff);
		vec_GF2 sbox_input(INIT_SIZE, 8);
		vec_GF2 dummy_round_key(INIT_SIZE, 8);
		vec_GF2 dummy_output(INIT_SIZE, 16);
		vec_GF2 result64(INIT_SIZE, 64);
		mat_GF2 g(INIT_SIZE, 8, 1);//useful
		mat_GF2 h(INIT_SIZE, 8, 1);//useless

		char_to_vec_GF2(input, sbox_input);
		NewSbox(sbox_input, dummy_round_key, dummy_output);
		for (int j = 0; j < 8; j++)
		{
			g[j][0] = dummy_output[j];
			h[j][0] = dummy_output[j + 8];
		}


		B1g = GF2_B1 * g;
		B2g = GF2_B2 * g;
		B3g = GF2_B3 * g;

		B1h = GF2_B1 * h;
		B2h = GF2_B2 * h;
		B3h = GF2_B3 * h;

		if (tij == 0)//g first, useful first
		{
			if (num == 0)//B1g B1h B3g B3h B2g B2h B2g B2h
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B1g[j][0];
					result64[j + 8] = B1h[j][0];
					result64[j + 16] = B3g[j][0];
					result64[j + 24] = B3h[j][0];

					result64[j + 32] = B2g[j][0];
					result64[j + 40] = B2g[j][0];
					result64[j + 48] = B2g[j][0];
					result64[j + 56] = B2h[j][0];
				}
			}

			if (num == 1)//B2g B2h B1g B1h B3g B3h B2g B2h
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B2g[j][0];
					result64[j + 8] = B2h[j][0];
					result64[j + 16] = B1g[j][0];
					result64[j+ 24] = B1h[j][0];

					result64[j+ 32] = B3g[j][0];
					result64[j + 40] = B3h[j][0];
					result64[j + 48] = B2g[j][0];
					result64[j + 56] = B2h[j][0];
				}
			}

			if (num == 2)//B2g B2h B2g B2h B1g B1h B3g B3h
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B2g[j][0];
					result64[j + 8] = B2h[j][0];
					result64[j + 16] = B2g[j][0];
					result64[j + 24] = B2h[j][0];

					result64[j+ 32] = B1g[j][0];
					result64[j + 40] = B1h[j][0];
					result64[j + 48] = B3g[j][0];
					result64[j + 56] = B3h[j][0];
				}
			}

			if (num == 3)//B3g B3h B2g B2h B2g B2h B1g B1h
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B3g[j][0];
					result64[j + 8] = B3h[j][0];
					result64[j + 16] = B2g[j][0];
					result64[j + 24] = B2h[j][0];

					result64[j + 32] = B2g[j][0];
					result64[j + 40] = B2h[j][0];
					result64[j + 48] = B1g[j][0];
					result64[j + 56] = B1h[j][0];
				}
			}
		}

		else if (tij == 1)//h first, useless first
		{
			if (num == 0)//B1h B1g B3h B3g B2h B2g B2h B2g
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B1h[j][0];
					result64[j+ 8] = B1g[j][0];
					result64[j + 16] = B3h[j][0];
					result64[j + 24] = B3g[j][0];

					result64[j + 32] = B2h[j][0];
					result64[j + 40] = B2g[j][0];
					result64[j + 48] = B2h[j][0];
					result64[j + 56] = B2g[j][0];
				}
			}

			if (num == 1)//B2h B2g B1h B1g B3h B3g B2h B2g
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B2h[j][0];
					result64[j+ 8] = B2g[j][0];
					result64[j + 16] = B1h[j][0];
					result64[j + 24] = B1g[j][0];

					result64[j + 32] = B3h[j][0];
					result64[j + 40] = B3g[j][0];
					result64[j+ 48] = B2h[j][0];
					result64[j + 56] = B2g[j][0];
				}
			}

			if (num == 2)//B2h B2g B2h B2g B1h B1g B3h B3g
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B2h[j][0];
					result64[j + 8] = B2g[j][0];
					result64[j+ 16] = B2h[j][0];
					result64[j + 24] = B2g[j][0];

					result64[j + 32] = B1h[j][0];
					result64[j + 40] = B1g[j][0];
					result64[j + 48] = B3h[j][0];
					result64[j + 56] = B3g[j][0];
				}
			}

			if (num == 3)//B3h B3g B2h B2g B2h B2g B1h B1g
			{
				for (int j = 0; j < 8; j++)
				{
					result64[j] = B3h[j][0];
					result64[j + 8] = B3g[j][0];
					result64[j+ 16] = B2h[j][0];
					result64[j+ 24] = B2g[j][0];

					result64[j + 32] = B2h[j][0];
					result64[j + 40] = B2g[j][0];
					result64[j + 48] = B1h[j][0];
					result64[j + 56] = B1g[j][0];
				}
			}

			
		}

		uint64_t TableResult = vec_GF2_to_u64(result64);
		//std::printf("%016llX\n", TableResult);
		Table.insert(pair<char, uint64_t>(char(i), TableResult));
	}

	return Table;
}


uint32_t ShiftAfterLookUpTable(uint64_t* four_input, int* tij)
{
	uint64_t LookUpTable0Result = four_input[0];
	uint64_t LookUpTable1Result = four_input[1];
	uint64_t LookUpTable2Result = four_input[2];
	uint64_t LookUpTable3Result = four_input[3];

	vec_GF2 vec_GF2_res0 = u64_to_vec_GF2(LookUpTable0Result);
	vec_GF2 vec_GF2_res1 = u64_to_vec_GF2(LookUpTable1Result);
	vec_GF2 vec_GF2_res2 = u64_to_vec_GF2(LookUpTable2Result);
	vec_GF2 vec_GF2_res3 = u64_to_vec_GF2(LookUpTable3Result);

	vec_GF2 Shifted_res(INIT_SIZE, 64);
	vec_GF2 temp_result(INIT_SIZE, 64);
	vec_GF2 temp_result32(INIT_SIZE, 32);
	
	int UselessFirst = tij[0];
	if (UselessFirst)
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res0[i + 8];
			Shifted_res[i + 8] = vec_GF2_res0[i + 24];
			Shifted_res[i + 16] = vec_GF2_res0[i + 40];
			Shifted_res[i + 24] = vec_GF2_res0[i + 56];

			Shifted_res[i + 32] = vec_GF2_res0[i];
			Shifted_res[i + 40] = vec_GF2_res0[i + 16];
			Shifted_res[i + 48] = vec_GF2_res0[i + 32];
			Shifted_res[i + 56] = vec_GF2_res0[i + 48];
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res0[i];
			Shifted_res[i + 8] = vec_GF2_res0[i + 16];
			Shifted_res[i + 16] = vec_GF2_res0[i + 32];
			Shifted_res[i + 24] = vec_GF2_res0[i + 48];

			Shifted_res[i + 32] = vec_GF2_res0[i+8];
			Shifted_res[i + 40] = vec_GF2_res0[i + 24];
			Shifted_res[i + 48] = vec_GF2_res0[i + 40];
			Shifted_res[i + 56] = vec_GF2_res0[i + 56];
		}
	}
	temp_result = temp_result + Shifted_res;


	UselessFirst = tij[1];
	if (UselessFirst)
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res1[i + 8];
			Shifted_res[i + 8] = vec_GF2_res1[i + 24];
			Shifted_res[i + 16] = vec_GF2_res1[i + 40];
			Shifted_res[i + 24] = vec_GF2_res1[i + 56];

			Shifted_res[i + 32] = vec_GF2_res1[i];
			Shifted_res[i + 40] = vec_GF2_res1[i + 16];
			Shifted_res[i + 48] = vec_GF2_res1[i + 32];
			Shifted_res[i + 56] = vec_GF2_res1[i + 48];
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res1[i];
			Shifted_res[i + 8] = vec_GF2_res1[i + 16];
			Shifted_res[i + 16] = vec_GF2_res1[i + 32];
			Shifted_res[i + 24] = vec_GF2_res1[i + 48];

			Shifted_res[i + 32] = vec_GF2_res1[i + 8];
			Shifted_res[i + 40] = vec_GF2_res1[i + 24];
			Shifted_res[i + 48] = vec_GF2_res1[i + 40];
			Shifted_res[i + 56] = vec_GF2_res1[i + 56];
		}
	}
	
	temp_result = temp_result + Shifted_res;


	UselessFirst = tij[2];
	if (UselessFirst)
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res2[i + 8];
			Shifted_res[i + 8] = vec_GF2_res2[i + 24];
			Shifted_res[i + 16] = vec_GF2_res2[i + 40];
			Shifted_res[i + 24] = vec_GF2_res2[i + 56];

			Shifted_res[i + 32] = vec_GF2_res2[i];
			Shifted_res[i + 40] = vec_GF2_res2[i + 16];
			Shifted_res[i + 48] = vec_GF2_res2[i + 32];
			Shifted_res[i + 56] = vec_GF2_res2[i + 48];
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res2[i];
			Shifted_res[i + 8] = vec_GF2_res2[i + 16];
			Shifted_res[i + 16] = vec_GF2_res2[i + 32];
			Shifted_res[i + 24] = vec_GF2_res2[i + 48];

			Shifted_res[i + 32] = vec_GF2_res2[i + 8];
			Shifted_res[i + 40] = vec_GF2_res2[i + 24];
			Shifted_res[i + 48] = vec_GF2_res2[i + 40];
			Shifted_res[i + 56] = vec_GF2_res2[i + 56];
		}
	}
	
	temp_result = temp_result + Shifted_res;


	UselessFirst = tij[3];
	if (UselessFirst)
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res3[i + 8];
			Shifted_res[i + 8] = vec_GF2_res3[i + 24];
			Shifted_res[i + 16] = vec_GF2_res3[i + 40];
			Shifted_res[i + 24] = vec_GF2_res3[i + 56];

			Shifted_res[i + 32] = vec_GF2_res3[i];
			Shifted_res[i + 40] = vec_GF2_res3[i + 16];
			Shifted_res[i + 48] = vec_GF2_res3[i + 32];
			Shifted_res[i + 56] = vec_GF2_res3[i + 48];
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			Shifted_res[i] = vec_GF2_res3[i];
			Shifted_res[i + 8] = vec_GF2_res3[i + 16];
			Shifted_res[i + 16] = vec_GF2_res3[i + 32];
			Shifted_res[i + 24] = vec_GF2_res3[i + 48];

			Shifted_res[i + 32] = vec_GF2_res3[i + 8];
			Shifted_res[i + 40] = vec_GF2_res3[i + 24];
			Shifted_res[i + 48] = vec_GF2_res3[i + 40];
			Shifted_res[i + 56] = vec_GF2_res3[i + 56];
		}
	}
	temp_result = temp_result + Shifted_res;
	for (int i = 0; i < 32; i++)
		temp_result32[i] = temp_result[i];
	uint32_t temp_result_u32 = vec_GF2_to_u32(temp_result32);
	return temp_result_u32;
}

uint32_t EncodeLookUpTableResult(uint32_t input, mat_GF2 &inv_A, vec_GF2 &b)
{
	mat_GF2 A(INIT_SIZE, 32, 32);
	mat_GF2 inv_A_temp(INIT_SIZE, 32, 32);
	vec_GF2 b_tmp(INIT_SIZE, 32);
	vec_GF2 input_tmp(INIT_SIZE, 32);
	Generate_2Random_Inverse_Matrix(32, A, inv_A_temp);
	b_tmp = GenerateGF2RandomVector(32);
	mat_GF2 input_temp_mat(INIT_SIZE, 32, 1);
	vec_GF2 temp_res(INIT_SIZE, 32);
	input_tmp = u32_to_vec_GF2(input);
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
			inv_A[i][j] = inv_A_temp[i][j];
		b[i] = b_tmp[i];
	}
	temp_res = A * input_tmp;
	temp_res = temp_res + b_tmp;
	uint32_t result = vec_GF2_to_u32(temp_res);

	A.kill();
	inv_A_temp.kill();
	b_tmp.kill();
	input_tmp.kill();
	input_temp_mat.kill();
	temp_res.kill();

	return result;
}

uint32_t RecoverLookUpTableResult(uint32_t input, mat_GF2& A, vec_GF2& b)
{
	vec_GF2 input_vec = u32_to_vec_GF2(input);
	input_vec = input_vec + b;
	
	input_vec = A * input_vec;
	uint32_t res= vec_GF2_to_u32(input_vec);
	input_vec.kill();
	return res;
}

uint32_t Encode32bit(uint32_t input, mat_GF2& inv_A, vec_GF2& b)
{
	return EncodeLookUpTableResult(input, inv_A, b);
}

uint32_t Decode32bit(uint32_t input, mat_GF2& A, vec_GF2& b)
{
	return RecoverLookUpTableResult(input, A, b);
}

uint32_t DiaAffineTransfer(uint32_t input, mat_GF2& A1, mat_GF2& A2, mat_GF2& A3, mat_GF2& A4, vec_GF2 b)
{
	vec_GF2 input_vec = u32_to_vec_GF2(input);

	vec_GF2 input_vec1(INIT_SIZE, 8);
	vec_GF2 input_vec2(INIT_SIZE, 8);
	vec_GF2 input_vec3(INIT_SIZE, 8);
	vec_GF2 input_vec4(INIT_SIZE, 8);

	vec_GF2 res1(INIT_SIZE, 8);
	vec_GF2 res2(INIT_SIZE, 8);
	vec_GF2 res3(INIT_SIZE, 8);
	vec_GF2 res4(INIT_SIZE, 8);

	vec_GF2 b1(INIT_SIZE, 8);
	vec_GF2 b2(INIT_SIZE, 8);
	vec_GF2 b3(INIT_SIZE, 8);
	vec_GF2 b4(INIT_SIZE, 8);

	vec_GF2 result_vec(INIT_SIZE, 32);

	for (int i = 0; i < 8; i++)
	{
		input_vec1[i] = input_vec[i];
		input_vec2[i] = input_vec[i + 8];
		input_vec3[i] = input_vec[i + 16];
		input_vec4[i] = input_vec[i + 24];

		b1[i] = b[i];
		b2[i] = b[i + 8];
		b3[i] = b[i + 16];
		b4[i] = b[i + 24];
	}
	
	res1 = A1 * input_vec1;
	res2 = A2 * input_vec2;
	res3 = A3 * input_vec3;
	res4 = A4 * input_vec4;

	res1 = res1 + b1;
	res2 = res2 + b2;
	res3 = res3 + b3;
	res4 = res4 + b4;

	for (int i = 0; i < 8; i++)
	{
		result_vec[i] = res1[i];
		result_vec[i + 8] = res2[i];
		result_vec[i + 16] = res3[i];
		result_vec[i + 24] = res4[i];
	}

	return vec_GF2_to_u32(result_vec);
}

uint32_t DiaAffineTransfer_inv(uint32_t input, mat_GF2& A1, mat_GF2& A2, mat_GF2& A3, mat_GF2& A4, vec_GF2 b)
{
	vec_GF2 input_vec = u32_to_vec_GF2(input);

	vec_GF2 input_vec1(INIT_SIZE, 8);
	vec_GF2 input_vec2(INIT_SIZE, 8);
	vec_GF2 input_vec3(INIT_SIZE, 8);
	vec_GF2 input_vec4(INIT_SIZE, 8);

	vec_GF2 res1(INIT_SIZE, 8);
	vec_GF2 res2(INIT_SIZE, 8);
	vec_GF2 res3(INIT_SIZE, 8);
	vec_GF2 res4(INIT_SIZE, 8);

	vec_GF2 b1(INIT_SIZE, 8);
	vec_GF2 b2(INIT_SIZE, 8);
	vec_GF2 b3(INIT_SIZE, 8);
	vec_GF2 b4(INIT_SIZE, 8);

	vec_GF2 result_vec(INIT_SIZE, 32);

	for (int i = 0; i < 8; i++)
	{
		input_vec1[i] = input_vec[i];
		input_vec2[i] = input_vec[i + 8];
		input_vec3[i] = input_vec[i + 16];
		input_vec4[i] = input_vec[i + 24];

		b1[i] = b[i];
		b2[i] = b[i + 8];
		b3[i] = b[i + 16];
		b4[i] = b[i + 24];
	}

	

	res1 = input_vec1 + b1;
	res2 = input_vec2 + b2;
	res3 = input_vec3 + b3;
	res4 = input_vec4 + b4;

	res1 = A1 * res1;
	res2 = A2 * res2;
	res3 = A3 * res3;
	res4 = A4 * res4;

	for (int i = 0; i < 8; i++)
	{
		result_vec[i] = res1[i];
		result_vec[i + 8] = res2[i];
		result_vec[i + 16] = res3[i];
		result_vec[i + 24] = res4[i];
	}

	return vec_GF2_to_u32(result_vec);
}

uint32_t LastEncode(uint32_t input, mat_GF2& A, vec_GF2& b)
{
	vec_GF2 input_vec = u32_to_vec_GF2(input);
	input_vec = A * input_vec;
	input_vec = input_vec + b;
	return vec_GF2_to_u32(input_vec);
}

void LookUpTableEncryptSM4(char* plaintext, char* key, char* ciphertext)
{
	uint32_t* state = (uint32_t*)malloc(4 * sizeof(uint32_t));
	//uint32_t* EncodedState = (uint32_t*)malloc(4 * sizeof(uint32_t));
	//uint32_t* DecodedState = (uint32_t*)malloc(4 * sizeof(uint32_t));
	uint32_t DecodedState[4] = { 0 };
	uint32_t EncodedState[4] = { 0 };
	uint32_t* key_u32 = (uint32_t*)malloc(4 * sizeof(uint32_t));
	char4** round_key = (char4**)malloc(32 * sizeof(char4*));
	char**round_key_char=(char**)malloc(32 * sizeof(char*));
	for (int i = 0; i < 32; i++)
	{
		round_key[i] = (char4*)malloc(32 * sizeof(char4));
		round_key_char[i] = (char*)malloc(4 * sizeof(char));
	}
	KeyExpansion(key, round_key);
	for (int i = 0; i < 32; i++)
		Convert_to_char_32(round_key[i], round_key_char[i]);
	for (int i = 0; i < 4; i++)
	{
		char byte1 = plaintext[4 * i];
		char byte2 = plaintext[4 * i + 1];
		char byte3 = plaintext[4 * i + 2];
		char byte4 = plaintext[4 * i + 3];
		state[i] = fourCharToInt(byte1, byte2, byte3, byte4);
	}
	
	mat_GF2 Ai(INIT_SIZE, 32, 32);
	mat_GF2 Ai1(INIT_SIZE, 32, 32);
	mat_GF2 Ai2(INIT_SIZE, 32, 32);
	mat_GF2 Ai3(INIT_SIZE, 32, 32);
	mat_GF2 Ai4(INIT_SIZE, 32, 32);

	mat_GF2 Ai_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai1_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai2_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai3_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai4_inv(INIT_SIZE, 32, 32);

	vec_GF2 bi(INIT_SIZE, 32);
	vec_GF2 bi1(INIT_SIZE, 32);
	vec_GF2 bi2(INIT_SIZE, 32);
	vec_GF2 bi3(INIT_SIZE, 32);
	vec_GF2 bi4(INIT_SIZE, 32);

	vec_GF2 b_ai4(INIT_SIZE, 32); //ai+4'
	vec_GF2 b1_ai4(INIT_SIZE, 32); //ai+4''

	mat_GF2 Ri0(INIT_SIZE, 8, 8);
	mat_GF2 Ri1(INIT_SIZE, 8, 8);
	mat_GF2 Ri2(INIT_SIZE, 8, 8);
	mat_GF2 Ri3(INIT_SIZE, 8, 8);

	mat_GF2 Ri0_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri1_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri2_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri3_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri4_inv(INIT_SIZE, 8, 8);

	mat_GF2 LookUpTableEncodeingA_inv(INIT_SIZE, 32, 32);
	vec_GF2 LookUpTableEncodeing_b(INIT_SIZE, 32);

	vec_GF2 Rbi(INIT_SIZE, 32);
	


	EncodedState[0] = Encode32bit(state[0], Ai_inv, bi);
	EncodedState[1] = Encode32bit(state[1], Ai1_inv, bi1);
	EncodedState[2] = Encode32bit(state[2], Ai2_inv, bi2);
	EncodedState[3] = Encode32bit(state[3], Ai3_inv, bi3);


	for (int i = 0; i < 33; i++)
	{
		
		DecodedState[0] = Decode32bit(EncodedState[0], Ai_inv, bi);//Xi
		DecodedState[1] = Decode32bit(EncodedState[1], Ai1_inv, bi1);
		DecodedState[2] = Decode32bit(EncodedState[2], Ai2_inv, bi2);
		DecodedState[3] = Decode32bit(EncodedState[3], Ai3_inv, bi3);

		if (i == 32)
		{
			for (int j = 0; j < 4; j++)
			{
				char temp[4] = { 0 };
				int_to_char(DecodedState[3-j], temp);
				for (int k = 0; k < 4; k++)
					ciphertext[4 * j + k] = temp[k];
			}

			break;
		}
		//printf("Round %d Decoded State:\n", i + 1);
		//printf("%08lX %08lX %08lX %08lX ", DecodedState[0], DecodedState[1], DecodedState[2], DecodedState[3]);
		

		Generate_2Random_Inverse_Matrix(8, Ri0, Ri0_inv);
		Generate_2Random_Inverse_Matrix(8, Ri1, Ri1_inv);
		Generate_2Random_Inverse_Matrix(8, Ri2, Ri2_inv);
		Generate_2Random_Inverse_Matrix(8, Ri3, Ri3_inv);

		Rbi = GenerateGF2RandomVector(32);
		uint32_t RijX1 = DiaAffineTransfer(DecodedState[1], Ri0, Ri1, Ri2, Ri3, Rbi);
		uint32_t RijX2 = DiaAffineTransfer(DecodedState[2], Ri0, Ri1, Ri2, Ri3, Rbi);
		uint32_t RijX3 = DiaAffineTransfer(DecodedState[3], Ri0, Ri1, Ri2, Ri3, Rbi);

		uint32_t X = RijX1 ^ RijX2 ^ RijX3;
		uint32_t XV = DecodedState[1] ^ DecodedState[2] ^ DecodedState[3];
		
		X = DiaAffineTransfer_inv(X, Ri0_inv, Ri1_inv, Ri2_inv, Ri3_inv, Rbi);
		
		std::default_random_engine RandomTij;
		RandomTij.seed(time(0));
		int tij[4] = { 0 };

		for (int j = 0; i < j; j++)
			tij[j] = RandomTij() % 2;

		std::map<char, uint64_t>table0 = BuildOneSboxTable(0, tij[0]);
		std::map<char, uint64_t>table1 = BuildOneSboxTable(1, tij[1]);
		std::map<char, uint64_t>table2 = BuildOneSboxTable(2, tij[2]);
		std::map<char, uint64_t>table3 = BuildOneSboxTable(3, tij[3]);

		uint64_t result[4] = { 0 };

		char input_char[4] = { 0 };
		uint32_t one_round_key = fourCharToInt(round_key_char[i][0], round_key_char[i][1], round_key_char[i][2], round_key_char[i][3]);
		int_to_char(X ^ one_round_key, input_char);
		
		result[0] = table0[input_char[0]];
		result[1] = table1[input_char[1]];
		result[2] = table2[input_char[2]];
		result[3] = table3[input_char[3]];

		uint32_t TResult = ShiftAfterLookUpTable(result, tij);
		uint32_t EncodedTResult = EncodeLookUpTableResult(TResult, LookUpTableEncodeingA_inv, LookUpTableEncodeing_b);
		//Yi
		uint32_t DecodedTResult = RecoverLookUpTableResult(EncodedTResult, LookUpTableEncodeingA_inv, LookUpTableEncodeing_b);
		

		uint32_t Unencoded = DecodedTResult ^ DecodedState[0];
		//printf(" Xi+4: %08lX\n", Unencoded);

		bi4 = GenerateGF2RandomVector(32); //bi for Wi+4
		b_ai4 = GenerateGF2RandomVector(32);// ai+4'
		b1_ai4 = bi4 + b_ai4;//ai+4''
		
		Generate_2Random_Inverse_Matrix(32, Ai4, Ai4_inv);

		//Encode Xi+4
		uint32_t EncodedXi = LastEncode(DecodedState[0], Ai4, b_ai4);
		uint32_t EncodedYi = LastEncode(DecodedTResult, Ai4, b1_ai4);

		uint32_t Xi4 = EncodedXi ^ EncodedYi;

		EncodedState[0] = DecodedState[1];
		EncodedState[1] = DecodedState[2];
		EncodedState[2] = DecodedState[3];
		EncodedState[3] = Xi4;

		//printf("Round %d Encoded State:\n", i + 1);
		//printf("%08llX %08llX %08llX %08llX\n", EncodedState[0], EncodedState[1], EncodedState[2], EncodedState[3]);

		EncodedState[0] = Encode32bit(EncodedState[0], Ai_inv, bi);
		EncodedState[1] = Encode32bit(EncodedState[1], Ai1_inv, bi1);
		EncodedState[2] = Encode32bit(EncodedState[2], Ai2_inv, bi2);
		Ai3_inv = Ai4_inv;
		bi3.move(bi4);
	}
}

void LookUpTableDecryptSM4(char* plaintext, char* key, char* ciphertext)
{
	uint32_t* state = (uint32_t*)malloc(4 * sizeof(uint32_t));
	//uint32_t* EncodedState = (uint32_t*)malloc(4 * sizeof(uint32_t));
	//uint32_t* DecodedState = (uint32_t*)malloc(4 * sizeof(uint32_t));
	uint32_t DecodedState[4] = { 0 };
	uint32_t EncodedState[4] = { 0 };
	uint32_t* key_u32 = (uint32_t*)malloc(4 * sizeof(uint32_t));
	char4** round_key = (char4**)malloc(32 * sizeof(char4*));
	char** round_key_char = (char**)malloc(32 * sizeof(char*));
	for (int i = 0; i < 32; i++)
	{
		round_key[i] = (char4*)malloc(32 * sizeof(char4));
		round_key_char[i] = (char*)malloc(4 * sizeof(char));
	}
	KeyExpansion(key, round_key);
	for (int i = 0; i < 32; i++)
		Convert_to_char_32(round_key[i], round_key_char[i]);
	for (int i = 0; i < 4; i++)
	{
		char byte1 = plaintext[4 * i];
		char byte2 = plaintext[4 * i + 1];
		char byte3 = plaintext[4 * i + 2];
		char byte4 = plaintext[4 * i + 3];
		state[i] = fourCharToInt(byte1, byte2, byte3, byte4);
	}

	mat_GF2 Ai(INIT_SIZE, 32, 32);
	mat_GF2 Ai1(INIT_SIZE, 32, 32);
	mat_GF2 Ai2(INIT_SIZE, 32, 32);
	mat_GF2 Ai3(INIT_SIZE, 32, 32);
	mat_GF2 Ai4(INIT_SIZE, 32, 32);

	mat_GF2 Ai_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai1_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai2_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai3_inv(INIT_SIZE, 32, 32);
	mat_GF2 Ai4_inv(INIT_SIZE, 32, 32);

	vec_GF2 bi(INIT_SIZE, 32);
	vec_GF2 bi1(INIT_SIZE, 32);
	vec_GF2 bi2(INIT_SIZE, 32);
	vec_GF2 bi3(INIT_SIZE, 32);
	vec_GF2 bi4(INIT_SIZE, 32);

	vec_GF2 b_ai4(INIT_SIZE, 32); //ai+4'
	vec_GF2 b1_ai4(INIT_SIZE, 32); //ai+4''

	mat_GF2 Ri0(INIT_SIZE, 8, 8);
	mat_GF2 Ri1(INIT_SIZE, 8, 8);
	mat_GF2 Ri2(INIT_SIZE, 8, 8);
	mat_GF2 Ri3(INIT_SIZE, 8, 8);

	mat_GF2 Ri0_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri1_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri2_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri3_inv(INIT_SIZE, 8, 8);
	mat_GF2 Ri4_inv(INIT_SIZE, 8, 8);

	mat_GF2 LookUpTableEncodeingA_inv(INIT_SIZE, 32, 32);
	vec_GF2 LookUpTableEncodeing_b(INIT_SIZE, 32);

	vec_GF2 Rbi(INIT_SIZE, 32);



	EncodedState[0] = Encode32bit(state[0], Ai_inv, bi);
	EncodedState[1] = Encode32bit(state[1], Ai1_inv, bi1);
	EncodedState[2] = Encode32bit(state[2], Ai2_inv, bi2);
	EncodedState[3] = Encode32bit(state[3], Ai3_inv, bi3);


	for (int i = 0; i < 33; i++)
	{

		DecodedState[0] = Decode32bit(EncodedState[0], Ai_inv, bi);//Xi
		DecodedState[1] = Decode32bit(EncodedState[1], Ai1_inv, bi1);
		DecodedState[2] = Decode32bit(EncodedState[2], Ai2_inv, bi2);
		DecodedState[3] = Decode32bit(EncodedState[3], Ai3_inv, bi3);

		if (i == 32)
		{
			for (int j = 0; j < 4; j++)
			{
				char temp[4] = { 0 };
				int_to_char(DecodedState[3 - j], temp);
				for (int k = 0; k < 4; k++)
					ciphertext[4 * j + k] = temp[k];
			}

			break;
		}
		//printf("Round %d Decoded State:\n", i + 1);
		//printf("%08lX %08lX %08lX %08lX ", DecodedState[0], DecodedState[1], DecodedState[2], DecodedState[3]);


		Generate_2Random_Inverse_Matrix(8, Ri0, Ri0_inv);
		Generate_2Random_Inverse_Matrix(8, Ri1, Ri1_inv);
		Generate_2Random_Inverse_Matrix(8, Ri2, Ri2_inv);
		Generate_2Random_Inverse_Matrix(8, Ri3, Ri3_inv);

		Rbi = GenerateGF2RandomVector(32);
		uint32_t RijX1 = DiaAffineTransfer(DecodedState[1], Ri0, Ri1, Ri2, Ri3, Rbi);
		uint32_t RijX2 = DiaAffineTransfer(DecodedState[2], Ri0, Ri1, Ri2, Ri3, Rbi);
		uint32_t RijX3 = DiaAffineTransfer(DecodedState[3], Ri0, Ri1, Ri2, Ri3, Rbi);

		uint32_t X = RijX1 ^ RijX2 ^ RijX3;
		uint32_t XV = DecodedState[1] ^ DecodedState[2] ^ DecodedState[3];

		X = DiaAffineTransfer_inv(X, Ri0_inv, Ri1_inv, Ri2_inv, Ri3_inv, Rbi);

		std::default_random_engine RandomTij;
		RandomTij.seed(time(0));
		int tij[4] = { 0 };

		for (int j = 0; i < j; j++)
			tij[j] = RandomTij() % 2;

		std::map<char, uint64_t>table0 = BuildOneSboxTable(0, tij[0]);
		std::map<char, uint64_t>table1 = BuildOneSboxTable(1, tij[1]);
		std::map<char, uint64_t>table2 = BuildOneSboxTable(2, tij[2]);
		std::map<char, uint64_t>table3 = BuildOneSboxTable(3, tij[3]);

		uint64_t result[4] = { 0 };

		char input_char[4] = { 0 };
		uint32_t one_round_key = fourCharToInt(round_key_char[31 - i][0], round_key_char[31 - i][1], round_key_char[31 - i][2], round_key_char[31 - i][3]);
		int_to_char(X ^ one_round_key, input_char);

		result[0] = table0[input_char[0]];
		result[1] = table1[input_char[1]];
		result[2] = table2[input_char[2]];
		result[3] = table3[input_char[3]];

		uint32_t TResult = ShiftAfterLookUpTable(result, tij);
		uint32_t EncodedTResult = EncodeLookUpTableResult(TResult, LookUpTableEncodeingA_inv, LookUpTableEncodeing_b);
		//Yi
		uint32_t DecodedTResult = RecoverLookUpTableResult(EncodedTResult, LookUpTableEncodeingA_inv, LookUpTableEncodeing_b);


		uint32_t Unencoded = DecodedTResult ^ DecodedState[0];
		//printf(" Xi+4: %08lX\n", Unencoded);

		bi4 = GenerateGF2RandomVector(32); //bi for Wi+4
		b_ai4 = GenerateGF2RandomVector(32);// ai+4'
		b1_ai4 = bi4 + b_ai4;//ai+4''

		Generate_2Random_Inverse_Matrix(32, Ai4, Ai4_inv);

		//Encode Xi+4
		uint32_t EncodedXi = LastEncode(DecodedState[0], Ai4, b_ai4);
		uint32_t EncodedYi = LastEncode(DecodedTResult, Ai4, b1_ai4);

		uint32_t Xi4 = EncodedXi ^ EncodedYi;

		EncodedState[0] = DecodedState[1];
		EncodedState[1] = DecodedState[2];
		EncodedState[2] = DecodedState[3];
		EncodedState[3] = Xi4;

		//printf("Round %d Encoded State:\n", i + 1);
		//printf("%08llX %08llX %08llX %08llX\n", EncodedState[0], EncodedState[1], EncodedState[2], EncodedState[3]);

		EncodedState[0] = Encode32bit(EncodedState[0], Ai_inv, bi);
		EncodedState[1] = Encode32bit(EncodedState[1], Ai1_inv, bi1);
		EncodedState[2] = Encode32bit(EncodedState[2], Ai2_inv, bi2);
		Ai3_inv = Ai4_inv;
		bi3.move(bi4);
	}
}



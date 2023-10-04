#include"look_up_table_aes.h"

std::map<char, uint32_t> AESBuildTable_AS(int rij, char round_key, mat_GF2& input_decode_A, vec_GF2& input_decode_b, mat_GF2* output_encode_A, vec_GF2& output_encode_b)
{
	char useful = 0x00;
	char useless = 0x00;
	vec_GF2 useful_vec(INIT_SIZE, 8);
	vec_GF2 useless_vec(INIT_SIZE, 8);

	vec_GF2 output_encode_res1(INIT_SIZE, 8);
	vec_GF2 output_encode_res2(INIT_SIZE, 8);
	vec_GF2 output_encode_res3(INIT_SIZE, 8);
	vec_GF2 output_encode_res4(INIT_SIZE, 8);

	vec_GF2 b1(INIT_SIZE, 8);
	vec_GF2 b2(INIT_SIZE, 8);
	vec_GF2 b3(INIT_SIZE, 8);
	vec_GF2 b4(INIT_SIZE, 8);

	char res1_char = 0x00;
	char res2_char = 0x00;
	char res3_char = 0x00;
	char res4_char = 0x00;

	char useful_index = 0;

	char useful_res = 0x00;
	char useless_res = 0x00;
	uint32_t result = 0;

	for (int i = 0; i < 8; i++)
	{
		b1[i] = output_encode_b[i];
		b2[i] = output_encode_b[i + 8];
		b3[i] = output_encode_b[i + 16];
		b4[i] = output_encode_b[i + 24];
	}
	
	uint32_t result1 = 0;
	uint32_t result2 = 0;

	std::map<char, uint32_t> Table;
	std::default_random_engine rand_engine;
	rand_engine.seed(time(0));

	for (int i = 0; i < 256; i++)
	{
		useful = (char)i;
		useless = (char)i;
		char rnd8 = rand_engine() % 0xff;

		
		useless = useless ^ rnd8;
		
		//decode x^round_key
		useful = decode8(useful, input_decode_A, input_decode_b);
		//decode x^rand
		useless = decode8(useless, input_decode_A, input_decode_b);
		
		useful = useful ^ round_key;

		//S(x^round_key)
		useful_res = LookUpTable_AES(useful);
		//S(x^rand)
		useless_res = LookUpTable_AES(useless);

		if (rij == 0)//Encode(k a a k)
			result = fourCharToInt(useless_res, useful_res, useful_res, useless_res);
		else if (rij == 1)//Encode(a k k a)
			result = fourCharToInt(useful_res, useless_res, useless_res, useful_res);
		
		//output encode
		uint32_t encoded_result = 0;
		encoded_result = encode32dia(result, output_encode_A, output_encode_b);
		Table.insert(pair<char, uint32_t>((char)i, encoded_result));
	}

	useful_vec.kill();
	useless_vec.kill();
	output_encode_res1.kill();
	output_encode_res2.kill();
	output_encode_res3.kill();
	output_encode_res4.kill();
	b1.kill();
	b2.kill();
	b3.kill();
	b4.kill();
	return Table;
}

char encode8(char input, mat_GF2& A, vec_GF2& b)
{
	vec_GF2 input_vec(INIT_SIZE, 8);
	char_to_vec_GF2(input, input_vec);
	input_vec = A * input_vec;
	//PrintVector(input_vec);
	input_vec = input_vec + b;
	char res = vec_GF2_to_char(input_vec);
	input_vec.kill();
	return res;
}	

char decode8(char input, mat_GF2& A, vec_GF2& b)
{
	vec_GF2 input_vec(INIT_SIZE, 8);
	char_to_vec_GF2(input, input_vec);
	input_vec = input_vec + b;
	//PrintVector(input_vec);
	input_vec = A * input_vec;
	char res = vec_GF2_to_char(input_vec);
	input_vec.kill();
	return res;
}

uint32_t encode32(uint32_t input, mat_GF2& A, vec_GF2& b)
{
	vec_GF2 input_vec = u32_to_vec_GF2(input);
	input_vec = A * input_vec;
	input_vec = input_vec + b;
	uint32_t res = vec_GF2_to_u32(input_vec);
	input_vec.kill();
	return res;
}

uint32_t decode32(uint32_t input, mat_GF2& A, vec_GF2& b)
{
	vec_GF2 input_vec = u32_to_vec_GF2(input);
	input_vec = input_vec + b;
	input_vec = A * input_vec;
	uint32_t res = vec_GF2_to_u32(input_vec);
	input_vec.kill();
	return res;
}

uint32_t encode32dia(uint32_t input, mat_GF2* A, vec_GF2& b)
{
	vec_GF2 input_vec1(INIT_SIZE, 8);
	vec_GF2 input_vec2(INIT_SIZE, 8);
	vec_GF2 input_vec3(INIT_SIZE, 8);
	vec_GF2 input_vec4(INIT_SIZE, 8);

	vec_GF2 b1(INIT_SIZE, 8);
	vec_GF2 b2(INIT_SIZE, 8);
	vec_GF2 b3(INIT_SIZE, 8);
	vec_GF2 b4(INIT_SIZE, 8);

	vec_GF2 input_vec = u32_to_vec_GF2(input);

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
	input_vec1 = A[0] * input_vec1;
	input_vec1 = input_vec1 + b1;
	//PrintVector(input_vec1);

	input_vec2 = A[1] * input_vec2;
	input_vec2 = input_vec2 + b2;

	input_vec3 = A[2] * input_vec3;
	input_vec3 = input_vec3 + b3;

	input_vec4 = A[3] * input_vec4;
	input_vec4 = input_vec4 + b4;

	char output1 = vec_GF2_to_char(input_vec1);
	char output2 = vec_GF2_to_char(input_vec2);
	char output3 = vec_GF2_to_char(input_vec3);
	char output4 = vec_GF2_to_char(input_vec4);


	uint32_t result = fourCharToInt(output1, output2, output3, output4);

	input_vec1.kill();
	input_vec2.kill();
	input_vec3.kill();
	input_vec4.kill();

	b1.kill();
	b1.kill();
	b1.kill();
	b1.kill();

	input_vec.kill();

	return result;
}


uint32_t decode32dia(uint32_t input, mat_GF2* A, vec_GF2& b)
{
	vec_GF2 input_vec1(INIT_SIZE, 8);
	vec_GF2 input_vec2(INIT_SIZE, 8);
	vec_GF2 input_vec3(INIT_SIZE, 8);
	vec_GF2 input_vec4(INIT_SIZE, 8);

	vec_GF2 b1(INIT_SIZE, 8);
	vec_GF2 b2(INIT_SIZE, 8);
	vec_GF2 b3(INIT_SIZE, 8);
	vec_GF2 b4(INIT_SIZE, 8);

	vec_GF2 input_vec = u32_to_vec_GF2(input);

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
	input_vec1 = input_vec1 + b1;
	input_vec1 = A[0] * input_vec1;
	
	input_vec2 = input_vec2 + b2;
	input_vec2 = A[1] * input_vec2;
	
	input_vec3 = input_vec3 + b3;
	input_vec3 = A[2] * input_vec3;
	
	input_vec4 = input_vec4 + b4;
	input_vec4 = A[3] * input_vec4;

	char output1 = vec_GF2_to_char(input_vec1);
	char output2 = vec_GF2_to_char(input_vec2);
	char output3 = vec_GF2_to_char(input_vec3);
	char output4 = vec_GF2_to_char(input_vec4);

	uint32_t result = fourCharToInt(output1, output2, output3, output4);

	input_vec1.kill();
	input_vec2.kill();
	input_vec3.kill();
	input_vec4.kill();

	b1.kill();
	b1.kill();
	b1.kill();
	b1.kill();

	input_vec.kill();

	return result;
}

void ExtendedShiftRow(uint32_t* shift_row_input, uint32_t* shift_row_output)
{
	uint32_t temp[16] = { 0 };
	
	temp[0] = shift_row_input[0];
	temp[4] = shift_row_input[4];
	temp[8] = shift_row_input[8];
	temp[12] = shift_row_input[12];

	temp[1] = shift_row_input[5];
	temp[5] = shift_row_input[9];
	temp[9] = shift_row_input[13];
	temp[13] = shift_row_input[1];

	temp[2] = shift_row_input[10];
	temp[6] = shift_row_input[14];
	temp[10] = shift_row_input[2];
	temp[14] = shift_row_input[6];

	temp[3] = shift_row_input[15];
	temp[7] = shift_row_input[3];
	temp[11] = shift_row_input[7];
	temp[15] = shift_row_input[11];

	for (int i = 0; i < 16; i++)
		shift_row_output[i] = temp[i];
}

char RecoverSequence(uint32_t input, int rij)
{
	char temp[4] = { 0 };
	int_to_char(input, temp);
	if (rij == 0)
		return temp[1];
	else
		return temp[0];

}


std::map<uint32_t, uint32_t> AESBuildTable_MC_mult2(int rij, mat_GF2* last_output_encode_A, vec_GF2& last_output_encode_b, mat_GF2* output_encode_A, vec_GF2& output_encode_b)
{
	char useful = 0x00;
	char useless = 0x00;
	std::map<uint32_t, uint32_t> Table;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			useful = (char)i;
			useless = (char)j;
			uint32_t index = 0;
			uint32_t kaak = fourCharToInt(useless, useful, useful, useless);
			uint32_t akka = fourCharToInt(useful, useless, useless, useful);
			//The index is supposed to be encoded, because the last look-up-table output is encoded 
			index = (rij == 0) ? encode32dia(kaak, last_output_encode_A, last_output_encode_b) : encode32dia(akka, last_output_encode_A, last_output_encode_b);

			char mx_output = Mult2(useful);
			char useless_output = Mult2(useless);

			uint32_t unencoded_output = 0;

			if (rij == 0)//kaak
				unencoded_output = fourCharToInt(useless_output, mx_output, mx_output, useless_output);
			else//akka
				unencoded_output = fourCharToInt(mx_output, useless_output, useless_output, mx_output);

			uint32_t encoded_output = encode32dia(unencoded_output, output_encode_A, output_encode_b);
			Table.insert(pair<uint32_t, uint32_t>(index, encoded_output));
		}
	}
	return Table;
}

std::map<uint32_t, uint32_t> AESBuildTable_MC_mult3(int rij, mat_GF2* last_output_encode_A, vec_GF2& last_output_encode_b, mat_GF2* output_encode_A, vec_GF2& output_encode_b)
{
	char useful = 0x00;
	char useless = 0x00;
	std::map<uint32_t, uint32_t> Table;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			useful = (char)i;
			useless = (char)j;
			uint32_t index = 0;
			uint32_t kaak = fourCharToInt(useless, useful, useful, useless);
			uint32_t akka = fourCharToInt(useful, useless, useless, useful);
			//The index is supposed to be encoded, because the last look-up-table output is encoded 
			index = (rij == 0) ? encode32dia(kaak, last_output_encode_A, last_output_encode_b) : encode32dia(akka, last_output_encode_A, last_output_encode_b);

			char mx_output = Mult3(useful);
			char useless_output = Mult3(useless);

			uint32_t unencoded_output = 0;

			if (rij == 0)//kaak
				unencoded_output = fourCharToInt(useless_output, mx_output, mx_output, useless_output);
			else//akka
				unencoded_output = fourCharToInt(mx_output, useless_output, useless_output, mx_output);

			uint32_t encoded_output = encode32dia(unencoded_output, output_encode_A, output_encode_b);
			Table.insert(pair<uint32_t, uint32_t>(index, encoded_output));
		}
	}
	return Table;
}

void GenerateEncode32Dia(mat_GF2* A, mat_GF2* inv_A, vec_GF2 &b)
{
	vec_GF2 b_temp(INIT_SIZE, 32);

	mat_GF2 A1(INIT_SIZE, 8, 8);
	mat_GF2 A2(INIT_SIZE, 8, 8);
	mat_GF2 A3(INIT_SIZE, 8, 8);
	mat_GF2 A4(INIT_SIZE, 8, 8);

	mat_GF2 invA1(INIT_SIZE, 8, 8);
	mat_GF2 invA2(INIT_SIZE, 8, 8);
	mat_GF2 invA3(INIT_SIZE, 8, 8);
	mat_GF2 invA4(INIT_SIZE, 8, 8);
	
	Generate_2Random_Inverse_Matrix(8, A1, invA1);
	Generate_2Random_Inverse_Matrix(8, A2, invA2);
	Generate_2Random_Inverse_Matrix(8, A3, invA3);
	Generate_2Random_Inverse_Matrix(8, A4, invA4);
	b_temp = GenerateGF2RandomVector(32);

	A[0].move(A1);
	A[1].move(A2);
	A[2].move(A3);
	A[3].move(A4);

	inv_A[0].move(invA1);
	inv_A[1].move(invA2);
	inv_A[2].move(invA3);
	inv_A[3].move(invA4);

	b.move(b_temp);

	b_temp.kill();
	
	A1.kill();
	A2.kill();
	A3.kill();
	A4.kill();

	invA1.kill();
	invA2.kill();
	invA3.kill();
	invA4.kill();
}

void GenerateEncode8(mat_GF2& A, mat_GF2& inv_A, vec_GF2& b)
{
	mat_GF2 A_temp(INIT_SIZE, 8, 8);
	mat_GF2 invA_temp(INIT_SIZE, 8, 8);
	vec_GF2 b_temp(INIT_SIZE, 8);

	Generate_2Random_Inverse_Matrix(8, A_temp, invA_temp);
	b_temp = GenerateGF2RandomVector(8);
	A.move(A_temp);
	inv_A.move(invA_temp);
	b.move(b_temp);

	b_temp.kill();
	invA_temp.kill();
	A_temp.kill();
}


void LookUpTableEncryptAES(char* plaintext, char* key, char* ciphertext)
{
	char4** round_key = (char4**)malloc(10 * sizeof(char4*));
	char** round_key_char = (char**)malloc(11 * sizeof(char*));
	char state[16] = { 0 };
	int rij[10][16];
	std::default_random_engine rand_engine;
	rand_engine.seed(time(0));
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 16; j++)
			rij[i][j] = rand_engine() % 2;
	}

	for (int i = 0; i < 10; i++)
		round_key[i] = (char4*)malloc(128 * sizeof(char4));

	for (int i = 0; i < 11; i++)
		round_key_char[i] = (char*)malloc(16 * sizeof(char));
	AESKeyExpansion(key, round_key);
	for (int i = 0; i < 16; i++)
		round_key_char[0][i] = key[i];
	for (int i = 0; i < 10; i++)
		Convert_to_char(round_key[i], round_key_char[i + 1]);

	for (int i = 0; i < 16; i++)
		state[i] = plaintext[i];

	for (int i = 0; i < 9; i++)
	{
		//16 Affine Transfer for 16bytes input
		mat_GF2 AS_input_encode_A[16];
		mat_GF2 AS_input_decode_A[16];

		mat_GF2 AS_output_encode_A[16][4];
		mat_GF2 AS_output_decode_A[16][4];
		mat_GF2 MX_output_encode_A[16][4];
		mat_GF2 MX_output_decode_A[16][4];

		mat_GF2 Shifted_AS_output_encode_A[16][4];
		mat_GF2 Shifted_AS_output_decode_A[16][4];
		vec_GF2 Shifted_AS_output_b[16];

		vec_GF2 AS_input_b[16];
		vec_GF2 AS_output_b[16];
		vec_GF2 MX_output_b[16];

		char AS_encoded_input[16] = { 0 };
		
		std::map<char, uint32_t> AS_table[16];

		uint32_t AS_encoded_output[16] = { 0 };
		uint32_t ShiftRowResult[16] = { 0 };
		uint32_t MX_result[16] = { 0 };

		int Shifted_rij[16] = { 0 };
		for (int j = 0; j < 16; j++)
		{
			GenerateEncode8(AS_input_encode_A[j], AS_input_decode_A[j], AS_input_b[j]);
			GenerateEncode32Dia(AS_output_encode_A[j], AS_output_decode_A[j], AS_output_b[j]);
			GenerateEncode32Dia(MX_output_encode_A[j], MX_output_decode_A[j], MX_output_b[j]);
		}

		for (int j = 0; j < 16; j++)
		{
			AS_table[j] = AESBuildTable_AS(rij[i][j], round_key_char[i][j], AS_input_decode_A[j], AS_input_b[j], AS_output_encode_A[j], AS_output_b[j]);
			AS_encoded_input[j] = encode8(state[j], AS_input_encode_A[j], AS_input_b[j]);
		}

		for (int j = 0; j < 16; j++)
		{
			AS_encoded_output[j] = AS_table[j][AS_encoded_input[j]];
			uint32_t AS_decoded_output = decode32dia(AS_encoded_output[j], AS_output_decode_A[j], AS_output_b[j]);
			char re_seq = RecoverSequence(AS_decoded_output, rij[i][j]);
			//printf("%02X ", (unsigned char)re_seq);
			//printf("%08llX ", AS_decoded_output);
		}
		//printf("\n");
		
		ExtendedShiftRow(AS_encoded_output, ShiftRowResult);

		//The Encode-Decode Should Be Shifted Too!!!!!!

		//Shift Output Encode
		for (int j = 0; j < 4; j++)
		{
			Shifted_AS_output_encode_A[0][j] = AS_output_encode_A[0][j];
			Shifted_AS_output_encode_A[4][j] = AS_output_encode_A[4][j];
			Shifted_AS_output_encode_A[8][j] = AS_output_encode_A[8][j];
			Shifted_AS_output_encode_A[12][j] = AS_output_encode_A[12][j];

			Shifted_AS_output_encode_A[1][j] = AS_output_encode_A[5][j];
			Shifted_AS_output_encode_A[5][j] = AS_output_encode_A[9][j];
			Shifted_AS_output_encode_A[9][j] = AS_output_encode_A[13][j];
			Shifted_AS_output_encode_A[13][j] = AS_output_encode_A[1][j];

			Shifted_AS_output_encode_A[2][j] = AS_output_encode_A[10][j];
			Shifted_AS_output_encode_A[6][j] = AS_output_encode_A[14][j];
			Shifted_AS_output_encode_A[10][j] = AS_output_encode_A[2][j];
			Shifted_AS_output_encode_A[14][j] = AS_output_encode_A[6][j];

			Shifted_AS_output_encode_A[3][j] = AS_output_encode_A[15][j];
			Shifted_AS_output_encode_A[7][j] = AS_output_encode_A[3][j];
			Shifted_AS_output_encode_A[11][j] = AS_output_encode_A[7][j];
			Shifted_AS_output_encode_A[15][j] = AS_output_encode_A[11][j];
		}

		//Shift Output Decode
		for (int j = 0; j < 4; j++)
		{
			Shifted_AS_output_decode_A[0][j] = AS_output_decode_A[0][j];
			Shifted_AS_output_decode_A[4][j] = AS_output_decode_A[4][j];
			Shifted_AS_output_decode_A[8][j] = AS_output_decode_A[8][j];
			Shifted_AS_output_decode_A[12][j] = AS_output_decode_A[12][j];

			Shifted_AS_output_decode_A[1][j] = AS_output_decode_A[5][j];
			Shifted_AS_output_decode_A[5][j] = AS_output_decode_A[9][j];
			Shifted_AS_output_decode_A[9][j] = AS_output_decode_A[13][j];
			Shifted_AS_output_decode_A[13][j] = AS_output_decode_A[1][j];

			Shifted_AS_output_decode_A[2][j] = AS_output_decode_A[10][j];
			Shifted_AS_output_decode_A[6][j] = AS_output_decode_A[14][j];
			Shifted_AS_output_decode_A[10][j] = AS_output_decode_A[2][j];
			Shifted_AS_output_decode_A[14][j] = AS_output_decode_A[6][j];

			Shifted_AS_output_decode_A[3][j] = AS_output_decode_A[15][j];
			Shifted_AS_output_decode_A[7][j] = AS_output_decode_A[3][j];
			Shifted_AS_output_decode_A[11][j] = AS_output_decode_A[7][j];
			Shifted_AS_output_decode_A[15][j] = AS_output_decode_A[11][j];
		}

		Shifted_AS_output_b[0] = AS_output_b[0];
		Shifted_AS_output_b[4] = AS_output_b[4];
		Shifted_AS_output_b[8] = AS_output_b[8];
		Shifted_AS_output_b[12] = AS_output_b[12];

		Shifted_AS_output_b[1] = AS_output_b[5];
		Shifted_AS_output_b[5] = AS_output_b[9];
		Shifted_AS_output_b[9] = AS_output_b[13];
		Shifted_AS_output_b[13] = AS_output_b[1];

		Shifted_AS_output_b[2] = AS_output_b[10];
		Shifted_AS_output_b[6] = AS_output_b[14];
		Shifted_AS_output_b[10] = AS_output_b[2];
		Shifted_AS_output_b[14] = AS_output_b[6];

		Shifted_AS_output_b[3] = AS_output_b[15];
		Shifted_AS_output_b[7] = AS_output_b[3];
		Shifted_AS_output_b[11] = AS_output_b[7];
		Shifted_AS_output_b[15] = AS_output_b[11];

		Shifted_rij[0] = rij[i][0];
		Shifted_rij[4] = rij[i][4];
		Shifted_rij[8] = rij[i][8];
		Shifted_rij[12] = rij[i][12];

		Shifted_rij[1] = rij[i][5];
		Shifted_rij[5] = rij[i][9];
		Shifted_rij[9] = rij[i][13];
		Shifted_rij[13] = rij[i][1];

		Shifted_rij[2] = rij[i][10];
		Shifted_rij[6] = rij[i][14];
		Shifted_rij[10] = rij[i][2];
		Shifted_rij[14] = rij[i][6];

		Shifted_rij[3] = rij[i][15];
		Shifted_rij[7] = rij[i][3];
		Shifted_rij[11] = rij[i][7];
		Shifted_rij[15] = rij[i][11];

		std::map<uint32_t, uint32_t> MX_mul2[16];
		std::map<uint32_t, uint32_t> MX_mul3[16];
		uint32_t mul2_temp[16] = { 0 };
		uint32_t mul3_temp[16] = { 0 };
		uint32_t mul_temp1[16] = { 0 };
		uint32_t mul_temp2[16] = { 0 };


		//2  6  10  14   
		//3  7 11 15
		//0  4  8  12
		//1  5  9  13
		for (int j = 2; j < 6; j++)
		{
			int temp = j;
			j = j % 4;

			mul_temp1[j] = decode32dia(ShiftRowResult[j], Shifted_AS_output_decode_A[j], Shifted_AS_output_b[j]);
			mul_temp1[j + 4] = decode32dia(ShiftRowResult[j + 4], Shifted_AS_output_decode_A[j + 4], Shifted_AS_output_b[j + 4]);
			mul_temp1[j + 8] = decode32dia(ShiftRowResult[j + 8], Shifted_AS_output_decode_A[j + 8], Shifted_AS_output_b[j + 8]);
			mul_temp1[j + 12] = decode32dia(ShiftRowResult[j + 12], Shifted_AS_output_decode_A[j + 12], Shifted_AS_output_b[j + 12]);

			//printf("%08lX %08lX %08lX %08lX\n", mul_temp1[j], mul_temp1[j + 4], mul_temp1[j + 8], mul_temp1[j + 12]);
			j = temp;
		}

		//printf("\n\n");

		for (int j = 3; j < 7; j++)
		{
			int temp = j;
			j = j % 4;

			mul_temp2[j] = decode32dia(ShiftRowResult[j], Shifted_AS_output_decode_A[j], Shifted_AS_output_b[j]);
			mul_temp2[j + 4] = decode32dia(ShiftRowResult[j + 4], Shifted_AS_output_decode_A[j + 4], Shifted_AS_output_b[j + 4]);
			mul_temp2[j + 8] = decode32dia(ShiftRowResult[j + 8], Shifted_AS_output_decode_A[j + 8], Shifted_AS_output_b[j + 8]);
			mul_temp2[j + 12] = decode32dia(ShiftRowResult[j + 12], Shifted_AS_output_decode_A[j + 12], Shifted_AS_output_b[j + 12]);

			//printf("%08lX %08lX %08lX %08lX\n", mul_temp2[j], mul_temp2[j + 4], mul_temp2[j + 8], mul_temp2[j + 12]);
			j = temp;
		}

		//printf("\n\n");

		for (int j = 0; j < 4; j++)
		{
			MX_mul2[j] = AESBuildTable_MC_mult2(Shifted_rij[j], Shifted_AS_output_encode_A[j], Shifted_AS_output_b[j], MX_output_encode_A[j], MX_output_b[j]);
			MX_mul2[j + 4] = AESBuildTable_MC_mult2(Shifted_rij[j + 4], Shifted_AS_output_encode_A[j + 4], Shifted_AS_output_b[j + 4], MX_output_encode_A[j + 4], MX_output_b[j + 4]);
			MX_mul2[j + 8] = AESBuildTable_MC_mult2(Shifted_rij[j + 8], Shifted_AS_output_encode_A[j + 8], Shifted_AS_output_b[j + 8], MX_output_encode_A[j + 8], MX_output_b[j + 8]);
			MX_mul2[j + 12] = AESBuildTable_MC_mult2(Shifted_rij[j + 12], Shifted_AS_output_encode_A[j + 12], Shifted_AS_output_b[j + 12], MX_output_encode_A[j + 12], MX_output_b[j + 12]);
		}
		for (int j = 0; j < 4; j++)
		{
			mul2_temp[j] = MX_mul2[j][ShiftRowResult[j]];
			mul2_temp[j + 4] = MX_mul2[j + 4][ShiftRowResult[j + 4]];
			mul2_temp[j + 8] = MX_mul2[j + 8][ShiftRowResult[j + 8]];
			mul2_temp[j + 12] = MX_mul2[j + 12][ShiftRowResult[j + 12]];

			mul2_temp[j] = decode32dia(mul2_temp[j], MX_output_decode_A[j], MX_output_b[j]);
			mul2_temp[j + 4] = decode32dia(mul2_temp[j + 4], MX_output_decode_A[j + 4], MX_output_b[j + 4]);
			mul2_temp[j + 8] = decode32dia(mul2_temp[j + 8], MX_output_decode_A[j + 8], MX_output_b[j + 8]);
			mul2_temp[j + 12] = decode32dia(mul2_temp[j + 12], MX_output_decode_A[j + 12], MX_output_b[j + 12]);

			//printf("%08lX %08lX %08lX %08lX\n", mul2_temp[j], mul2_temp[j + 4], mul2_temp[j + 8], mul2_temp[j + 12]);
		}
		//printf("\n\n");

		for (int j = 1; j < 5; j++)
		{
			int temp = j;
			j = j % 4;
			MX_mul3[j] = AESBuildTable_MC_mult3(Shifted_rij[j], Shifted_AS_output_encode_A[j], Shifted_AS_output_b[j], MX_output_encode_A[j], MX_output_b[j]);
			MX_mul3[j + 4] = AESBuildTable_MC_mult3(Shifted_rij[j + 4], Shifted_AS_output_encode_A[j + 4], Shifted_AS_output_b[j + 4], MX_output_encode_A[j + 4], MX_output_b[j + 4]);
			MX_mul3[j + 8] = AESBuildTable_MC_mult3(Shifted_rij[j + 8], Shifted_AS_output_encode_A[j + 8], Shifted_AS_output_b[j + 8], MX_output_encode_A[j + 8], MX_output_b[j + 8]);
			MX_mul3[j + 12] = AESBuildTable_MC_mult3(Shifted_rij[j + 12], Shifted_AS_output_encode_A[j + 12], Shifted_AS_output_b[j + 12], MX_output_encode_A[j + 12], MX_output_b[j + 12]);
			j = temp;
		}
		for (int j = 1; j < 5; j++)
		{
			int temp = j;
			j = j % 4;
			mul3_temp[j] = MX_mul3[j][ShiftRowResult[j]];
			mul3_temp[j + 4] = MX_mul3[j + 4][ShiftRowResult[j + 4]];
			mul3_temp[j + 8] = MX_mul3[j + 8][ShiftRowResult[j + 8]];
			mul3_temp[j + 12] = MX_mul3[j + 12][ShiftRowResult[j + 12]];

			mul3_temp[j] = decode32dia(mul3_temp[j], MX_output_decode_A[j], MX_output_b[j]);
			mul3_temp[j + 4] = decode32dia(mul3_temp[j + 4], MX_output_decode_A[j + 4], MX_output_b[j + 4]);
			mul3_temp[j + 8] = decode32dia(mul3_temp[j + 8], MX_output_decode_A[j + 8], MX_output_b[j + 8]);
			mul3_temp[j + 12] = decode32dia(mul3_temp[j + 12], MX_output_decode_A[j + 12], MX_output_b[j + 12]);

			//printf("%08lX %08lX %08lX %08lX\n", mul3_temp[j], mul3_temp[j + 4], mul3_temp[j + 8], mul3_temp[j + 12]);
			j = temp;
		}
	

		for (int j = 0; j < 16; j = j + 4)
		{
			char temp1 = RecoverSequence(mul2_temp[j], Shifted_rij[j]);
			char temp2 = RecoverSequence(mul3_temp[j + 1], Shifted_rij[j + 1]);
			char temp3 = RecoverSequence(mul_temp1[j + 2], Shifted_rij[j + 2]);
			char temp4 = RecoverSequence(mul_temp2[j + 3], Shifted_rij[j + 3]);
			char res = temp1 ^ temp2 ^ temp3 ^ temp4;
			state[j] = res;
		}
		
		for (int j = 0; j < 16; j = j + 4)
		{
			char temp1 = RecoverSequence(mul_temp1[j], Shifted_rij[j]);
			char temp2 = RecoverSequence(mul2_temp[j + 1], Shifted_rij[j + 1]);
			char temp3 = RecoverSequence(mul3_temp[j + 2], Shifted_rij[j + 2]);
			char temp4 = RecoverSequence(mul_temp2[j + 3], Shifted_rij[j + 3]);
			char res = temp1 ^ temp2 ^ temp3 ^ temp4;
			state[j + 1] = res;
		}

		for (int j = 0; j < 16; j = j + 4)
		{
			char temp1 = RecoverSequence(mul_temp1[j], Shifted_rij[j]);
			char temp2 = RecoverSequence(mul_temp2[j + 1], Shifted_rij[j + 1]);
			char temp3 = RecoverSequence(mul2_temp[j + 2], Shifted_rij[j + 2]);
			char temp4 = RecoverSequence(mul3_temp[j + 3], Shifted_rij[j + 3]);
			char res = temp1 ^ temp2 ^ temp3 ^ temp4;
			state[j + 2] = res;
		}

		for (int j = 0; j < 16; j = j + 4)
		{
			char temp1 = RecoverSequence(mul3_temp[j], Shifted_rij[j]);
			char temp2 = RecoverSequence(mul_temp1[j + 1], Shifted_rij[j + 1]);
			char temp3 = RecoverSequence(mul_temp2[j + 2], Shifted_rij[j + 2]);
			char temp4 = RecoverSequence(mul2_temp[j + 3], Shifted_rij[j + 3]);
			char res = temp1 ^ temp2 ^ temp3 ^ temp4;
			state[j + 3] = res;
		}
		print_char(state, 16);

	}////

	for (int i = 0; i < 16; i++)
		state[i] = state[i] ^ round_key_char[9][i];
	for (int i = 0; i < 16; i++)
		state[i] = LookUpTable_AES(state[i]);
	ShiftRowVerification(state, state);
	for (int i = 0; i < 16; i++)
		state[i] = state[i] ^ round_key_char[10][i];
	print_char(state, 16);
}

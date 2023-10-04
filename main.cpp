#include"masked_aes.h"
#include"masked_sm4.h"
#include"look_up_table_aes.h"
#include"look_up_table_sm4.h"
#include"utils.h"
int main()
{
	std::default_random_engine UnitTest;
	UnitTest.seed(time(0));
	for (int i = 0; i < 1; i++)
	{
		//printf("Round: %d\r", i + 1);
		char round_key = 0x2b;
		char state = 0x99;
		char rand_char = 0xc7;
		int rij = 0;

		round_key = UnitTest() % 0xff;
		state = UnitTest() % 0xff;
		rand_char = UnitTest() % 0xff;
		rij = UnitTest() % 2;


		char TrueInput = state ^ round_key;
		char TrueOutput = LookUpTable_AES(TrueInput);

		mat_GF2 input_encode(INIT_SIZE, 8, 8);
		mat_GF2 input_decode(INIT_SIZE, 8, 8);
		vec_GF2 input_decode_b(INIT_SIZE, 8);

		mat_GF2 output_encode[4];
		mat_GF2 output_decode[4];
		vec_GF2 output_encode_b(INIT_SIZE, 32);

		mat_GF2 mx_output_encode[4];
		mat_GF2 mx_output_decode[4];
		vec_GF2 mx_output_encode_b = GenerateGF2RandomVector(32);

		GenerateEncode8(input_encode, input_decode, input_decode_b);
		//PrintVector(input_decode_b);
		GenerateEncode32Dia(output_encode, output_decode, output_encode_b);
		//PrintVector(output_encode_b);
		std::map<char, uint32_t> Table = AESBuildTable_AS(rij, round_key, input_decode, input_decode_b, output_encode, output_encode_b);
		char encoded_input = encode8(state, input_encode, input_decode_b);

		uint32_t encoded_output = Table[encoded_input];
		uint32_t decoded_output = decode32dia(encoded_output, output_decode, output_encode_b);

		GenerateEncode32Dia(mx_output_encode, mx_output_decode, mx_output_encode_b);
		std::map<uint32_t, uint32_t> Table2= AESBuildTable_MC_mult3(rij, output_encode, output_encode_b, mx_output_encode, mx_output_encode_b);

		uint32_t mx_encoded_output = Table2[encoded_output];
		uint32_t mx_decoded_output = decode32dia(mx_encoded_output, mx_output_decode, mx_output_encode_b);

		char temp[4] = { 0 };
		char mx_res = 0;
		int_to_char(mx_decoded_output, temp);
		//print_char(temp, 4);
		//printf("\r");
		if (rij == 0)
			mx_res = temp[1];
		else
			mx_res = temp[0];
		char realMul2res = Mult3(TrueOutput);

		if (realMul2res != mx_res)
		{
			printf("Failed\n");
			break;
		}
	}

	char Plaintext[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	char Key[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	char AESCiphertext[16] = { 0 };
	for (int i = 0; i < 16; i++)
	{
		Plaintext[i] = rand() % 0xff;
		Key[i] = rand() % 0xff;
	}
	print_char(Plaintext, 16);
	print_char(Key, 16);
	encryptAES(Plaintext, Key, AESCiphertext);

	printf("\n\n\n\n");
	
	LookUpTableEncryptAES(Plaintext, Key, AESCiphertext);
}

	


	/*for (int i = 0; i < 10; i++)
	{
		srand((int)time(0));
		char Plaintext[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
		char Key[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
		char SM4Ciphertext[16] = { 0 };
		char SM4decryptText[16] = { 0 };

		for (int i = 0; i < 16; i++)
		{
			Plaintext[i] = rand() % 0xff;
			Key[i] = rand() % 0xff;
		}

		LookUpTableEncryptSM4(Plaintext, Key, SM4Ciphertext);

		LookUpTableDecryptSM4(SM4Ciphertext, Key, SM4decryptText);
		if (memcmp(Plaintext, SM4decryptText, 16) != 0)
		{
			printf("Failed\n");
			break;
		}
	}
	printf("Succeed\n");*/

	



	//char AESCiphertext[16] = { 0 };
	//
	//char AESdecryptText[16] = { 0 };
	//
	//unsigned int Iteration = 1;
	//unsigned int TestSucceed = 1;

	////----------------------------------------Boolean Circuit Test--------------------------------------
	//for (int i = 0; i < Iteration; i++)
	//{

	//	if (Iteration > 1)
	//	{
	//		for (int i = 0; i < 16; i++)
	//		{
	//			Plaintext[i] = rand() % 0xff;
	//			Key[i] = rand() % 0xff;
	//		}
	//	}

	//	encryptAES(Plaintext, Key, AESCiphertext);
	//	decryptAES(AESCiphertext, Key, AESdecryptText);

	//	encryptSM4(Plaintext, Key, SM4Ciphertext);
	//	decryptSM4(SM4Ciphertext, Key, SM4decryptText);

	//	if (Iteration == 1)
	//	{
	//		printf("Plaintext:\n");
	//		print_char(Plaintext, 16);
	//		printf("Key:\n");
	//		print_char(Key, 16);
	//		printf("AES Ciphertext:\n");
	//		print_char(AESCiphertext, 16);
	//		printf("decypted Text:\n");
	//		print_char(AESdecryptText, 16);
	//		printf("\n\n");


	//		printf("SM4 Ciphertext:\n");
	//		print_char(SM4Ciphertext, 16);
	//		printf("decypted Text:\n");
	//		print_char(SM4decryptText, 16);
	//	}




	//	if (memcmp(AESdecryptText, Plaintext, 16 * sizeof(char)) != 0)
	//	{
	//		printf("AES Encryption-Decryption Test Failed\n");
	//		TestSucceed = 0;
	//	}

	//	if (memcmp(SM4decryptText, Plaintext, 16 * sizeof(char)) != 0)
	//	{
	//		printf("SM4 Encryption-Decryption Test Failed\n");
	//		TestSucceed = 0;
	//	}

	//	if (!TestSucceed)break;
	//	printf("Test Progress: %d%%\r", ((i + 1) * 100) / Iteration);
	//}
	//if (TestSucceed)
	//	printf("Boolean Circuit Test Succeed\n");
	//else
	//	printf("Boolean Circuit Test Failed\n");

	



	

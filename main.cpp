#include"masked_aes.h"
#include"masked_sm4.h"
#include"look_up_table_aes.h"
#include"look_up_table_sm4.h"
#include"utils.h"
#include"gcm.h"
#define TEST_CASE (2)





int main(int argc, char* argv[])
{

#if defined(TEST_CASE) && (TEST_CASE==1)
	uint8_t key[AES_BLOCK_SIZE] = { 0 };
	uint8_t* input = NULL;
	uint8_t* output = NULL;
	size_t length = 0;
	uint8_t* add = NULL;
	size_t add_len = 0;
	uint8_t iv[GCM_DEFAULT_IV_LEN] = { 0 };
	size_t iv_len = GCM_DEFAULT_IV_LEN;

#elif defined(TEST_CASE) && (TEST_CASE==2)
	uint8_t key[AES_BLOCK_SIZE] = { 0 };
	uint8_t input[AES_BLOCK_SIZE] = { 0 };
	uint8_t output[AES_BLOCK_SIZE];
	size_t length = AES_BLOCK_SIZE;
	uint8_t* add = NULL;
	size_t add_len = 0;
	uint8_t iv[GCM_DEFAULT_IV_LEN] = { 0 };
	size_t iv_len = GCM_DEFAULT_IV_LEN;

#elif defined(TEST_CASE) && (TEST_CASE==3)
	uint8_t key[AES_BLOCK_SIZE] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
	size_t length = AES_BLOCK_SIZE * 4;
	uint8_t input[AES_BLOCK_SIZE * 4] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55 };
	uint8_t output[AES_BLOCK_SIZE * 4];
	size_t add_len = 0;
	uint8_t* add = NULL;
	size_t iv_len = GCM_DEFAULT_IV_LEN;
	uint8_t iv[GCM_DEFAULT_IV_LEN] = {
		0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88 };

#elif defined(TEST_CASE) && (TEST_CASE==4)
	uint8_t key[AES_BLOCK_SIZE] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
	size_t length = AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN;
	uint8_t input[AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39 };
	uint8_t output[AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN];
	size_t add_len = AES_BLOCK_SIZE + 4;
	uint8_t add[AES_BLOCK_SIZE + 4] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2 };
	size_t iv_len = GCM_DEFAULT_IV_LEN;
	uint8_t iv[GCM_DEFAULT_IV_LEN] = {
		0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88 };

#elif defined(TEST_CASE) && (TEST_CASE==5)
	uint8_t key[AES_BLOCK_SIZE] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
	size_t length = AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN;
	uint8_t input[AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39 };
	uint8_t output[AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN];
	size_t add_len = AES_BLOCK_SIZE + 4;
	uint8_t add[AES_BLOCK_SIZE + 4] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2 };
	size_t iv_len = GCM_DEFAULT_IV_LEN - 4;
	uint8_t iv[GCM_DEFAULT_IV_LEN - 4] = {
		0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad };

#elif defined(TEST_CASE) && (TEST_CASE==6)
	uint8_t key[AES_BLOCK_SIZE] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
	size_t length = AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN;
	uint8_t input[AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39 };
	uint8_t output[AES_BLOCK_SIZE * 3 + GCM_DEFAULT_IV_LEN];
	size_t add_len = AES_BLOCK_SIZE + 4;
	uint8_t add[AES_BLOCK_SIZE + 4] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2 };
	size_t iv_len = 4 * AES_BLOCK_SIZE - 4;
	uint8_t iv[4 * AES_BLOCK_SIZE - 4] = {
		0x93, 0x13, 0x22, 0x5d, 0xf8, 0x84, 0x06, 0xe5, 0x55, 0x90, 0x9c, 0x5a, 0xff, 0x52, 0x69, 0xaa,
		0x6a, 0x7a, 0x95, 0x38, 0x53, 0x4f, 0x7d, 0xa1, 0xe4, 0xc3, 0x03, 0xd2, 0xa3, 0x18, 0xa7, 0x28,
		0xc3, 0xc0, 0xc9, 0x51, 0x56, 0x80, 0x95, 0x39, 0xfc, 0xf0, 0xe2, 0x42, 0x9a, 0x6b, 0x52, 0x54,
		0x16, 0xae, 0xdb, 0xf5, 0xa0, 0xde, 0x6a, 0x57, 0xa6, 0x37, 0xb3, 0x9b };
#elif defined(TEST_CASE) && (TEST_CASE==7)
	uint8_t key[AES_BLOCK_SIZE] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
	size_t length = 256;
	uint8_t input[256] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39 };
	uint8_t output[256];
	size_t add_len = AES_BLOCK_SIZE + 4;
	uint8_t add[AES_BLOCK_SIZE + 4] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2 };
	size_t iv_len = 4 * AES_BLOCK_SIZE - 4;
	uint8_t iv[4 * AES_BLOCK_SIZE - 4] = {
		0x93, 0x13, 0x22, 0x5d, 0xf8, 0x84, 0x06, 0xe5, 0x55, 0x90, 0x9c, 0x5a, 0xff, 0x52, 0x69, 0xaa,
		0x6a, 0x7a, 0x95, 0x38, 0x53, 0x4f, 0x7d, 0xa1, 0xe4, 0xc3, 0x03, 0xd2, 0xa3, 0x18, 0xa7, 0x28,
		0xc3, 0xc0, 0xc9, 0x51, 0x56, 0x80, 0x95, 0x39, 0xfc, 0xf0, 0xe2, 0x42, 0x9a, 0x6b, 0x52, 0x54,
		0x16, 0xae, 0xdb, 0xf5, 0xa0, 0xde, 0x6a, 0x57, 0xa6, 0x37, 0xb3, 0x9b };
#endif

	uint8_t tag[16] = { 0 };
	size_t tag_len = 16;

	encryption_gcm_whitebox(key,
		iv, iv_len,
		add, add_len,
		input, length,
		output,
		tag, tag_len, encryptSM4);

	decryption_gcm_whitebox(key,
		iv, iv_len,
		add, add_len,
		tag, tag_len,
		output, length,
		input, encryptSM4);

		std::default_random_engine UnitTest;
		UnitTest.seed(time(0));
	

		unsigned char Plaintext[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
		unsigned char Key[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };

		unsigned char BooleanCircuitSM4Ciphertext[16] = { 0 };
		unsigned char BooleanCircuitAESCiphertext[16] = { 0 };
		unsigned char BooleanCircuitSM4decryptText[16] = { 0 };
		unsigned char BooleanCircuitAESdecryptText[16] = { 0 };

		unsigned char LookUpTableAESCiphertext[16] = { 0 };
		unsigned char LookUpTableSM4Ciphertext[16] = { 0 };
		unsigned char LookUpTableAESdecryptText[16] = { 0 };
		unsigned char LookUpTableSM4decryptText[16] = { 0 };

		int TestSuccess = 1;

		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				Plaintext[j] = UnitTest() % 0xff;
				Key[j] = UnitTest() % 0xff;
			}

			void (*encryptAESptr)(char*, char*, char*) = NULL;
			encryptAESptr = encryptAES;
			(*encryptAESptr)((char*)Plaintext, (char*)Key, (char*)BooleanCircuitAESCiphertext);

			encryptSM4((char*)Plaintext, (char*)Key, (char*)BooleanCircuitSM4Ciphertext);
			decryptSM4((char*)BooleanCircuitSM4Ciphertext, (char*)Key, (char*)BooleanCircuitSM4decryptText);

			//encryptAES(Plaintext, Key, BooleanCircuitAESCiphertext);
			decryptAES((char*)BooleanCircuitAESCiphertext, (char*)Key, (char*)BooleanCircuitAESdecryptText);



			if (memcmp(BooleanCircuitSM4decryptText, Plaintext, 16) != 0)
			{
				printf("Boolean Circuit AES Test Failed\n");
				TestSuccess = 0;
			}

			if (memcmp(BooleanCircuitAESdecryptText, Plaintext, 16) != 0)
			{
				printf("Boolean Circuit SM4 Test Failed\n");
				TestSuccess = 0;
			}

			if (!TestSuccess)break;
		}
		if (TestSuccess)
		{
			printf("Plaintext:\n");
			print_char((char*)Plaintext, 16);
			printf("Key:\n");
			print_char((char*)Key, 16);
			printf("Boolean Circuit SM4 Ciphertext:\n");
			print_char((char*)BooleanCircuitSM4Ciphertext, 16);
			printf("Boolean Circuit SM4 Decrypted Text:\n");
			print_char((char*)BooleanCircuitSM4decryptText, 16);

			printf("Boolean Circuit AES Ciphertext:\n");
			print_char((char*)BooleanCircuitAESCiphertext, 16);
			printf("Boolean Circuit AES Decrypted Text:\n");
			print_char((char*)BooleanCircuitAESdecryptText, 16);

			printf("---------------------------------Boolean Circuit Test Successful---------------------------------\n");
		}
		else
			printf("---------------------------------Boolean Circuit Test Failed---------------------------------\n");

		TestSuccess = 1;
		printf("\n\n");

		for (int i = 0; i < 1; i++)
		{
			for (int i = 0; i < 16; i++)
			{
				Plaintext[i] = UnitTest() % 0xff;
				Key[i] = UnitTest() % 0xff;
			}

			LookUpTableEncryptSM4((char*)Plaintext, (char*)Key, (char*)LookUpTableSM4Ciphertext);
			LookUpTableDecryptSM4((char*)LookUpTableSM4Ciphertext, (char*)Key, (char*)LookUpTableSM4decryptText);

			LookUpTableEncryptAES((char*)Plaintext, (char*)Key, (char*)LookUpTableAESCiphertext);
			LookUpTableDecryptAES((char*)LookUpTableAESCiphertext, (char*)Key, (char*)LookUpTableAESdecryptText);

			if (memcmp(LookUpTableAESdecryptText, Plaintext, 16) != 0)
			{
				printf("Look Up Table AES Test Failed\n");
				TestSuccess = 0;
			}

			if (memcmp(LookUpTableSM4decryptText, Plaintext, 16) != 0)
			{
				printf("Look Up Table SM4 Test Failed\n");
				TestSuccess = 0;
			}

			if (!TestSuccess)break;
		}
		if (TestSuccess)
		{
			printf("Plaintext:\n");
			print_char((char*)Plaintext, 16);
			printf("Key:\n");
			print_char((char*)Key, 16);
			printf("Look Up Table SM4 Ciphertext:\n");
			print_char((char*)LookUpTableSM4Ciphertext, 16);
			printf("Look Up Table SM4 Decrypted Text:\n");
			print_char((char*)LookUpTableSM4decryptText, 16);

			printf("Look Up Table AES Ciphertext:\n");
			print_char((char*)LookUpTableAESCiphertext, 16);
			printf("Look Up Table AES Decrypted Text:\n");
			print_char((char*)LookUpTableAESdecryptText, 16);

			printf("---------------------------------Look Up Table Test Successful---------------------------------\n");
		}
		else
			printf("---------------------------------Look Up Table Test Failed---------------------------------\n");



	return 0;
}


//int main()
//{
//	std::default_random_engine UnitTest;
//	UnitTest.seed(time(0));
//	
//
//	char Plaintext[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
//	char Key[16] = { 0x01,0x23,0x45,0x67,0x89 ,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
//
//	char BooleanCircuitSM4Ciphertext[16] = { 0 };
//	char BooleanCircuitAESCiphertext[16] = { 0 };
//	char BooleanCircuitSM4decryptText[16] = { 0 };
//	char BooleanCircuitAESdecryptText[16] = { 0 };
//
//	char LookUpTableAESCiphertext[16] = { 0 };
//	char LookUpTableSM4Ciphertext[16] = { 0 };
//	char LookUpTableAESdecryptText[16] = { 0 };
//	char LookUpTableSM4decryptText[16] = { 0 };
//
//	int TestSuccess = 1;
//
//	for (int i = 0; i < 1000; i++)
//	{
//		for (int j = 0; j < 16; j++)
//		{
//			Plaintext[j] = UnitTest() % 0xff;
//			Key[j] = UnitTest() % 0xff;
//		}
//
//		void (*encryptAESptr)(char*, char*, char*) = NULL;
//		encryptAESptr = encryptAES;
//		(*encryptAESptr)(Plaintext, Key, BooleanCircuitAESCiphertext);
//
//		encryptSM4(Plaintext, Key, BooleanCircuitSM4Ciphertext);
//		decryptSM4(BooleanCircuitSM4Ciphertext, Key, BooleanCircuitSM4decryptText);
//
//		//encryptAES(Plaintext, Key, BooleanCircuitAESCiphertext);
//		decryptAES(BooleanCircuitAESCiphertext, Key, BooleanCircuitAESdecryptText);
//
//
//
//		if (memcmp(BooleanCircuitSM4decryptText, Plaintext, 16) != 0)
//		{
//			printf("Boolean Circuit AES Test Failed\n");
//			TestSuccess = 0;
//		}
//
//		if (memcmp(BooleanCircuitAESdecryptText, Plaintext, 16) != 0)
//		{
//			printf("Boolean Circuit SM4 Test Failed\n");
//			TestSuccess = 0;
//		}
//
//		if (!TestSuccess)break;
//	}
//	if (TestSuccess)
//	{
//		printf("Plaintext:\n");
//		print_char(Plaintext, 16);
//		printf("Key:\n");
//		print_char(Key, 16);
//		printf("Boolean Circuit SM4 Ciphertext:\n");
//		print_char(BooleanCircuitSM4Ciphertext, 16);
//		printf("Boolean Circuit SM4 Decrypted Text:\n");
//		print_char(BooleanCircuitSM4decryptText, 16);
//
//		printf("Boolean Circuit AES Ciphertext:\n");
//		print_char(BooleanCircuitAESCiphertext, 16);
//		printf("Boolean Circuit AES Decrypted Text:\n");
//		print_char(BooleanCircuitAESdecryptText, 16);
//
//		printf("---------------------------------Boolean Circuit Test Successful---------------------------------\n");
//	}
//	else
//		printf("---------------------------------Boolean Circuit Test Failed---------------------------------\n");
//
//	TestSuccess = 1;
//	printf("\n\n");
//
//	for (int i = 0; i < 1; i++)
//	{
//		for (int i = 0; i < 16; i++)
//		{
//			Plaintext[i] = UnitTest() % 0xff;
//			Key[i] = UnitTest() % 0xff;
//		}
//
//		LookUpTableEncryptSM4(Plaintext, Key, LookUpTableSM4Ciphertext);
//		LookUpTableDecryptSM4(LookUpTableSM4Ciphertext, Key, LookUpTableSM4decryptText);
//
//		LookUpTableEncryptAES(Plaintext, Key, LookUpTableAESCiphertext);
//		LookUpTableDecryptAES(LookUpTableAESCiphertext, Key, LookUpTableAESdecryptText);
//
//		if (memcmp(LookUpTableAESdecryptText, Plaintext, 16) != 0)
//		{
//			printf("Look Up Table AES Test Failed\n");
//			TestSuccess = 0;
//		}
//
//		if (memcmp(LookUpTableSM4decryptText, Plaintext, 16) != 0)
//		{
//			printf("Look Up Table SM4 Test Failed\n");
//			TestSuccess = 0;
//		}
//
//		if (!TestSuccess)break;
//	}
//	if (TestSuccess)
//	{
//		printf("Plaintext:\n");
//		print_char(Plaintext, 16);
//		printf("Key:\n");
//		print_char(Key, 16);
//		printf("Look Up Table SM4 Ciphertext:\n");
//		print_char(LookUpTableSM4Ciphertext, 16);
//		printf("Look Up Table SM4 Decrypted Text:\n");
//		print_char(LookUpTableSM4decryptText, 16);
//
//		printf("Look Up Table AES Ciphertext:\n");
//		print_char(LookUpTableAESCiphertext, 16);
//		printf("Look Up Table AES Decrypted Text:\n");
//		print_char(LookUpTableAESdecryptText, 16);
//
//		printf("---------------------------------Look Up Table Test Successful---------------------------------\n");
//	}
//	else
//		printf("---------------------------------Look Up Table Test Failed---------------------------------\n");
//
//
//}

	


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

	



	
////----------------------------------------Encode-Decode Unit Test Succeed--------------------------------------
	//char* input = (char*)malloc(16 * sizeof(char));
	//char4* MaksedInput = (char4*)malloc(128 * sizeof(char4));
	//for (int j = 0; j < 10000; j++)
	//{


	//	for (int i = 0; i < 16; i++)
	//		input[i] = UnitTest() % 0xff;

	//	Convert_to_char4(input, MaksedInput);
	//	Convert_to_char(MaksedInput, output);

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (input[i] != output[i])
	//		{
	//			printf("Encode-Decode Unit Test Failed");
	//			TestSucceed = 0;
	//			break;

	//		}
	//	}
	//	if (!TestSucceed)break;
	//}
	//if(TestSucceed)
	//	printf("Encode-Decode Unit Test Succeed\n");
	//

	////----------------------------------------Refresh Unit Test--------------------------------------
	//for (int j = 0; j < 10000; j++)
	//{
	//	char4* RefreshMaskedInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* RefreshResult = (char4*)malloc(128 * sizeof(char4));
	//	char* RefreshResultDecoded = (char*)malloc(16 * sizeof(char));
	//	char* RefreshInput = (char*)malloc(16 * sizeof(char));
	//	for (int i = 0; i < 16; i++)
	//		RefreshInput[i] = UnitTest() % 0xff;
	//	Convert_to_char4(RefreshInput, RefreshMaskedInput);

	//	Refresh(RefreshMaskedInput, RefreshResult);
	//	Convert_to_char(RefreshResult, RefreshResultDecoded);
	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (RefreshResultDecoded[i] != RefreshInput[i])
	//		{
	//			printf("Refresh Unit Test Failed");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}
	//	if (!TestSucceed)
	//		break;
	//}
	//if(TestSucceed)
	//	printf("Refresh Unit Test Succeed\n");


	////----------------------------------------XOR Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* XOR_input1 = (char*)malloc(16 * sizeof(char));
	//	char* XOR_input2 = (char*)malloc(16 * sizeof(char));
	//	char* XOR_result_true = (char*)malloc(16 * sizeof(char));
	//	char* XOR_result = (char*)malloc(16 * sizeof(char));

	//	char* extra_result = (char*)malloc(16 * sizeof(char));

	//	char4* XOR_Masked_input1 = (char4*)malloc(128 * sizeof(char4));
	//	char4* XOR_Masked_input2 = (char4*)malloc(128 * sizeof(char4));
	//	char4* XOR_Masked_result = (char4*)malloc(128 * sizeof(char4));

	//	char4* extra_Masked_result = (char4*)malloc(128 * sizeof(char4));
	//	char4* final_result = (char4*)malloc(128 * sizeof(char4));
	//	for (int i = 0; i < 16; i++)
	//	{
	//		XOR_input1[i] = UnitTest() % 0xff;
	//		XOR_input2[i] = UnitTest() % 0xff;
	//		extra_result[i] = UnitTest() % 0xff;
	//		XOR_result_true[i] = (XOR_input1[i] ^ XOR_input2[i]) & extra_result[i];
	//		//it's different from XOR_input1[i] ^ XOR_input2[i] & extra_result[i], be careful !!
	//	}

	//	Convert_to_char4(XOR_input1, XOR_Masked_input1);
	//	Convert_to_char4(XOR_input2, XOR_Masked_input2);
	//	Convert_to_char4(extra_result, extra_Masked_result);

	//	Eval_XOR(XOR_Masked_input1, XOR_Masked_input2, XOR_Masked_result);
	//	Eval_AND(XOR_Masked_result, extra_Masked_result, final_result);

	//	Convert_to_char(final_result, XOR_result);

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (XOR_result[i] != XOR_result_true[i])
	//		{
	//			printf("XOR Unit Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}
	//	if (!TestSucceed)break;
	//}
	//if(TestSucceed)
	//	printf("XOR Unit Test Succeed\n");


	//TestSucceed = 1;
	////----------------------------------------AND Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* AND_input1 = (char*)malloc(16 * sizeof(char));
	//	char* AND_input2 = (char*)malloc(16 * sizeof(char));
	//	char* AND_result_true = (char*)malloc(16 * sizeof(char));
	//	char* AND_result = (char*)malloc(16 * sizeof(char));

	//	char* extra_result = (char*)malloc(16 * sizeof(char));

	//	char4* AND_Masked_input1 = (char4*)malloc(128 * sizeof(char4));
	//	char4* AND_Masked_input2 = (char4*)malloc(128 * sizeof(char4));
	//	char4* AND_Masked_result = (char4*)malloc(128 * sizeof(char4));

	//	char4* extra_Masked_result = (char4*)malloc(128 * sizeof(char4));
	//	char4* final_result = (char4*)malloc(128 * sizeof(char4));
	//	for (int i = 0; i < 16; i++)
	//	{
	//		AND_input1[i] = UnitTest() % 0xff;
	//		AND_input2[i] = UnitTest() % 0xff;
	//		extra_result[i] = UnitTest() % 0xff;
	//		AND_result_true[i] = (AND_input1[i] & AND_input2[i]) ^ extra_result[i];
	//	}


	//	Convert_to_char4(AND_input1, AND_Masked_input1);
	//	Convert_to_char4(AND_input2, AND_Masked_input2);
	//	Convert_to_char4(extra_result, extra_Masked_result);

	//	Eval_AND(AND_Masked_input1, AND_Masked_input2, AND_Masked_result);
	//	Eval_XOR(AND_Masked_result, extra_Masked_result, final_result);

	//	Convert_to_char(final_result, AND_result);

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (AND_result[i] != AND_result_true[i])
	//		{
	//			printf("AND Unit Test Failed");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}
	//	if (!TestSucceed)break;
	//}
	//if(TestSucceed)
	//	printf("AND Unit Test Succeed\n");

	////----------------------------------------32bit AND Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* AND_input1 = (char*)malloc(4 * sizeof(char));
	//	char* AND_input2 = (char*)malloc(4 * sizeof(char));
	//	char* AND_result_true = (char*)malloc(4 * sizeof(char));
	//	char* AND_result = (char*)malloc(4 * sizeof(char));

	//	char* extra_result = (char*)malloc(4 * sizeof(char));

	//	char4* AND_Masked_input1 = (char4*)malloc(32 * sizeof(char4));
	//	char4* AND_Masked_input2 = (char4*)malloc(32 * sizeof(char4));
	//	char4* AND_Masked_result = (char4*)malloc(32 * sizeof(char4));

	//	char4* extra_Masked_result = (char4*)malloc(32 * sizeof(char4));
	//	char4* final_result = (char4*)malloc(32 * sizeof(char4));
	//	for (int i = 0; i < 4; i++)
	//	{
	//		AND_input1[i] = UnitTest() % 0xff;
	//		AND_input2[i] = UnitTest() % 0xff;
	//		extra_result[i] = UnitTest() % 0xff;
	//		AND_result_true[i] = (AND_input1[i] & AND_input2[i]) ^ extra_result[i];
	//	}


	//	Convert_to_char4_32(AND_input1, AND_Masked_input1);
	//	Convert_to_char4_32(AND_input2, AND_Masked_input2);
	//	Convert_to_char4_32(extra_result, extra_Masked_result);

	//	Eval_AND_32(AND_Masked_input1, AND_Masked_input2, AND_Masked_result);
	//	Eval_XOR_32(AND_Masked_result, extra_Masked_result, final_result);

	//	Convert_to_char_32(final_result, AND_result);

	//	for (int i = 0; i < 4; i++)
	//	{
	//		if (AND_result[i] != AND_result_true[i])
	//		{
	//			printf("32 bits AND Unit Test Failed");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}
	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("32 bits AND Unit Test Succeed\n");



	////----------------------------------------32 bits XOR Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* XOR_input1 = (char*)malloc(4 * sizeof(char));
	//	char* XOR_input2 = (char*)malloc(4 * sizeof(char));
	//	char* XOR_result_true = (char*)malloc(4 * sizeof(char));
	//	char* XOR_result = (char*)malloc(4 * sizeof(char));

	//	char* extra_result = (char*)malloc(4 * sizeof(char));

	//	char4* XOR_Masked_input1 = (char4*)malloc(32 * sizeof(char4));
	//	char4* XOR_Masked_input2 = (char4*)malloc(32 * sizeof(char4));
	//	char4* XOR_Masked_result = (char4*)malloc(32 * sizeof(char4));

	//	char4* extra_Masked_result = (char4*)malloc(32 * sizeof(char4));
	//	char4* final_result = (char4*)malloc(32 * sizeof(char4));
	//	for (int i = 0; i < 4; i++)
	//	{
	//		XOR_input1[i] = UnitTest() % 0xff;
	//		XOR_input2[i] = UnitTest() % 0xff;
	//		extra_result[i] = UnitTest() % 0xff;
	//		XOR_result_true[i] = (XOR_input1[i] ^ XOR_input2[i]) & extra_result[i];
	//		//it's different from XOR_input1[i] ^ XOR_input2[i] & extra_result[i], be careful !!
	//	}

	//	Convert_to_char4_32(XOR_input1, XOR_Masked_input1);
	//	Convert_to_char4_32(XOR_input2, XOR_Masked_input2);
	//	Convert_to_char4_32(extra_result, extra_Masked_result);

	//	Eval_XOR_32(XOR_Masked_input1, XOR_Masked_input2, XOR_Masked_result);
	//	Eval_AND_32(XOR_Masked_result, extra_Masked_result, XOR_Masked_result);

	//	Convert_to_char_32(XOR_Masked_result, XOR_result);

	//	for (int i = 0; i < 4; i++)
	//	{
	//		if (XOR_result[i] != XOR_result_true[i])
	//		{
	//			printf("32bits XOR Unit Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}
	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("32 bits XOR Unit Test Succeed\n");


	////----------------------------------------32bits Round Shift Left Unit Test--------------------------------------
	//for (int i = 0; i < 1; i++)
	//{
	//	char* ShiftInput = (char*)malloc(4 * sizeof(char));
	//	char* ShiftOutput = (char*)malloc(4 * sizeof(char));
	//	unsigned int temp= 0x00000000;
	//	
	//	
	//	char4* MaskedShiftInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedShiftOutput = (char4*)malloc(32 * sizeof(char4));
	//	for (int i = 0; i < 4; i++)
	//		ShiftInput[i] = UnitTest() % 0xff;
	//	

	//	
	//	Convert_to_char4_32(ShiftInput, MaskedShiftInput);

	//	LeftShift32(MaskedShiftInput, MaskedShiftOutput, 10);

	//	Convert_to_char_32(MaskedShiftOutput, ShiftOutput);


	//	
	//}





	////----------------------------------------Sbox Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* SboxInput = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));
	//	char* CalOutput = (char*)malloc(4 * sizeof(char));
	//	char4* MaskedSboxInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedSboxOutput = (char4*)malloc(32 * sizeof(char4));
	//	for (int i = 0; i < 4; i++)
	//		SboxInput[i] = UnitTest() % 0xff;
	//	for (int i = 0; i < 4; i++)
	//		TrueOutput[i] = LookUpTable(SboxInput[i]);
	//	Convert_to_char4_32(SboxInput, MaskedSboxInput);

	//	Sbox(MaskedSboxInput, MaskedSboxOutput);

	//	Convert_to_char_32(MaskedSboxOutput, CalOutput);

	//	
	//	for (int i = 0; i < 4; i++)
	//	{
	//		if (CalOutput[i] != TrueOutput[i])
	//		{
	//			printf("Sbox Unit Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("Sbox Unit Test Succeed\n");


	////----------------------------------------T Function Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* TfuncitonInput = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));
	//	char* CalOutput = (char*)malloc(4 * sizeof(char));
	//	char* SboxResult = (char*)malloc(4 * sizeof(char));

	//	char4* MaskedTfunctionInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedTfunctionOutput = (char4*)malloc(32 * sizeof(char4));

	//	for (int i = 0; i < 4; i++)
	//		TfuncitonInput[i] = UnitTest() % 0xff;

	//	for (int i = 0; i < 4; i++)
	//		SboxResult[i] = LookUpTable(TfuncitonInput[i]);

	//	
	//	unsigned int L_output2 = TFunctionVerification(TfuncitonInput);

	//	
	//	Convert_to_char4_32(TfuncitonInput, MaskedTfunctionInput);
	//	T_Function(MaskedTfunctionInput, MaskedTfunctionOutput);
	//	Convert_to_char_32(MaskedTfunctionOutput, CalOutput);
	//	
	//	unsigned int L_output3 = fourCharToInt(CalOutput[0], CalOutput[1], CalOutput[2], CalOutput[3]);
	//	//printf("%X %X\n", L_output2, L_output3);


	//	if (L_output2 != L_output3)
	//	{
	//		TestSucceed = 0;
	//		printf("T Function Unit Test Failed\n");
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("T Function Unit Test Succeed\n");


	////----------------------------------------F Function Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* F_function_input = (char*)malloc(16 * sizeof(char));
	//	char* round_key = (char*)malloc(4 * sizeof(char));
	//	unsigned int TrueOutput = 0;
	//	char* CalOutput = (char*)malloc(4 * sizeof(char));

	//	char4* MaskedF_functionInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedRoundKey = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedF_functionOutput = (char4*)malloc(32 * sizeof(char4));

	//	for (int i = 0; i < 4; i++)
	//		round_key[i] = UnitTest() % 0xff;
	//	for (int i = 0; i < 16; i++)
	//		F_function_input[i] = UnitTest() % 0xff;
	//	Convert_to_char4(F_function_input, MaskedF_functionInput);
	//	Convert_to_char4_32(round_key, MaskedRoundKey);

	//	F_Function(MaskedF_functionInput, MaskedRoundKey, MaskedF_functionOutput);
	//	TrueOutput = F_FunctionVerification(F_function_input, round_key);
	//	Convert_to_char_32(MaskedF_functionOutput, CalOutput);
	//	unsigned int CalOutputInt = fourCharToInt(CalOutput[0], CalOutput[1], CalOutput[2], CalOutput[3]);

	//	if (CalOutputInt != TrueOutput)
	//	{
	//		printf("F Function Unit Test Failed\n");
	//		TestSucceed = 0;
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("F Function Unit Test Succeed\n");


	////----------------------------------------T Function for Key Expansion Unit Test--------------------------------------

	//for (int i = 0; i < 10000; i++)
	//{
	//	char* TfuncitonInput = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));
	//	char* CalOutput = (char*)malloc(4 * sizeof(char));
	//	char* SboxResult = (char*)malloc(4 * sizeof(char));

	//	char4* MaskedTfunctionInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedTfunctionOutput = (char4*)malloc(32 * sizeof(char4));

	//	for (int i = 0; i < 4; i++)
	//		TfuncitonInput[i] = UnitTest() % 0xff;

	//	for (int i = 0; i < 4; i++)
	//		SboxResult[i] = LookUpTable(TfuncitonInput[i]);


	//	unsigned int L_output2 = TFunctionKE_Verification(TfuncitonInput);


	//	Convert_to_char4_32(TfuncitonInput, MaskedTfunctionInput);
	//	T_Funtion_KE(MaskedTfunctionInput, MaskedTfunctionOutput);
	//	Convert_to_char_32(MaskedTfunctionOutput, CalOutput);

	//	unsigned int L_output3 = fourCharToInt(CalOutput[0], CalOutput[1], CalOutput[2], CalOutput[3]);
	//	//printf("%X %X\n", L_output2, L_output3);


	//	if (L_output2 != L_output3)
	//	{
	//		TestSucceed = 0;
	//		printf("T Function for Key Expansion Unit Test Failed\n");
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("T Function for Key Expansion Unit Test Succeed\n");


	////----------------------------------------F Function For Key Expansion Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* F_function_input = (char*)malloc(16 * sizeof(char));
	//	char* round_key = (char*)malloc(4 * sizeof(char));
	//	unsigned int TrueOutput = 0;
	//	char* CalOutput = (char*)malloc(4 * sizeof(char));

	//	char4* MaskedF_functionInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedRoundKey = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedF_functionOutput = (char4*)malloc(32 * sizeof(char4));

	//	for (int i = 0; i < 4; i++)
	//		round_key[i] = UnitTest() % 0xff;
	//	for (int i = 0; i < 16; i++)
	//		F_function_input[i] = UnitTest() % 0xff;
	//	Convert_to_char4(F_function_input, MaskedF_functionInput);
	//	Convert_to_char4_32(round_key, MaskedRoundKey);

	//	F_Function_KE(MaskedF_functionInput, MaskedRoundKey, MaskedF_functionOutput);
	//	TrueOutput = F_Function_KE_Verification(F_function_input, round_key);
	//	Convert_to_char_32(MaskedF_functionOutput, CalOutput);
	//	unsigned int CalOutputInt = fourCharToInt(CalOutput[0], CalOutput[1], CalOutput[2], CalOutput[3]);

	//	if (CalOutputInt != TrueOutput)
	//	{
	//		printf("F Function For Key Expansion Unit Test Failed\n");
	//		TestSucceed = 0;
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("F Function For Key Expansion Unit Test Succeed\n");

	////----------------------------------------Key Expansion Unit Test--------------------------------------
	//char4** round_key = (char4**)malloc(32 * sizeof(char4*));
	//for (int i = 0; i < 32; i++)
	//	round_key[i] = (char4*)malloc(32 * sizeof(char4));
	//KeyExpansion(Key, round_key);


	/*for (int i = 0; i < 16; i++)
	{
		Plaintext[i] = UnitTest() % 0xff;
		Key[i] = UnitTest() % 0xff;
	}*/




	//---------------------------------------Masked SM4 Encryption-Decryption Test--------------------------------------
	//printf("Plaintext:\n");
	//print_char(Plaintext, 16);
	//printf("Key:\n");
	//print_char(Key, 16);
	//encryptSM4(Plaintext, Key, Ciphertext);

	////Encryption result shoud be 681EDF34 D206965E 86B3E94F 536E4246
	//printf("Ciphertext:\n");
	//print_char(Ciphertext, 16);

	//

	//printf("After Decryption, the plaintext is:\n");
	//char* decryptedText = (char*)malloc(16 * sizeof(char));
	//decryptSM4(Ciphertext, Key, decryptedText);
	//print_char(decryptedText, 16);

	//printf("After 1 000 000 Encryption, Ciphertext is:\n");

	//for (int i = 0; i < 1000000; i++)
	//{
	//	encryptSM4(Plaintext, Key, Ciphertext);
	//	for (int i = 0; i < 16; i++)
	//		Plaintext[i] = Ciphertext[i];

	//	printf("%d\r", (i+1));
	//}

	//print_char(Ciphertext, 16);























	//----------------------------------------AES Sbox Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* SboxInput = (char*)malloc(16 * sizeof(char));
	//	char* SboxOutput = (char*)malloc(16 * sizeof(char));
	//	char* SboxOutput_inv = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(16 * sizeof(char));
	//	char4* MaskedSboxInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedSboxOutput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedSboxOutput_inv = (char4*)malloc(128 * sizeof(char4));
	//	for (int i = 0; i < 16; i++)
	//		SboxInput[i] = UnitTest() % 0xff;
	//	for (int i = 0; i < 16; i++)
	//		TrueOutput[i] = LookUpTable_AES(SboxInput[i]);
	//	Convert_to_char4(SboxInput, MaskedSboxInput);

	//	BytesSub(MaskedSboxInput, MaskedSboxOutput);
	//	Convert_to_char(MaskedSboxOutput, SboxOutput);

	//	BytesSub_inv(MaskedSboxOutput, MaskedSboxOutput_inv);
	//	Convert_to_char(MaskedSboxOutput_inv, SboxOutput_inv);


	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (SboxOutput[i] != TrueOutput[i])
	//		{
	//			printf("Sbox Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (SboxOutput_inv[i] != SboxInput[i])
	//		{
	//			printf("inv Sbox Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("Sbox Test Succeed\n");

	////----------------------------------------AES Shift Row Unit Test--------------------------------------
	//for (int i = 0; i < 1; i++)
	//{
	//	char* input = (char*)malloc(16 * sizeof(char));
	//	char* Output = (char*)malloc(16 * sizeof(char));
	//	char* Output_inv = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput_inv = (char*)malloc(16 * sizeof(char));
	//	char4* MaskedInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaksedOutput_inv = (char4*)malloc(128 * sizeof(char4));
	//	for (int i = 0; i < 16; i++)
	//		input[i] = UnitTest() % 0xff;

	//	Convert_to_char4(input, MaskedInput);
	//	ShiftRow(MaskedInput, MaskedOutput);
	//	ShiftRow_inv(MaskedOutput, MaksedOutput_inv);
	//	ShiftRowVerification(input, TrueOutput);
	//	ShiftRowVerification_inv(TrueOutput, TrueOutput_inv);
	//	Convert_to_char(MaskedOutput, Output);
	//	Convert_to_char(MaksedOutput_inv, Output_inv);

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (TrueOutput[i] != Output[i])
	//		{
	//			printf("Shift Row Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (input[i] != Output_inv[i])
	//		{
	//			printf("Shift Row Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("Shift Row Test Succeed\n");

	////----------------------------------------AES Add Round Key Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* input = (char*)malloc(16 * sizeof(char));
	//	char* input2 = (char*)malloc(16 * sizeof(char));
	//	char* Output = (char*)malloc(16 * sizeof(char));
	//	char* Output_inv = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput_inv = (char*)malloc(16 * sizeof(char));
	//	char4* MaskedInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedInput2 = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaksedOutput_inv = (char4*)malloc(128 * sizeof(char4));
	//	for (int i = 0; i < 16; i++)
	//		input[i] = UnitTest() % 0xff;
	//	for (int i = 0; i < 16; i++)
	//		input2[i] = UnitTest() % 0xff;
	//	AddRoundKeyVerification(input, input2, TrueOutput);

	//	Convert_to_char4(input, MaskedInput);
	//	Convert_to_char4(input2, MaskedInput2);

	//	AddRoundKey(MaskedInput, MaskedInput2, MaskedOutput);
	//	AddRoundKey(MaskedOutput, MaskedInput2, MaksedOutput_inv);

	//	Convert_to_char(MaskedOutput, Output);
	//	Convert_to_char(MaksedOutput_inv, Output_inv);

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (TrueOutput[i] != Output[i])
	//		{
	//			printf("Add Round Key Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (Output_inv[i] != input[i])
	//		{
	//			printf("Add Round Key Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("Add Round Key Test Succeed\n");


	////----------------------------------------AES Mix Coloumn Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* input = (char*)malloc(16 * sizeof(char));
	//	char* input2 = (char*)malloc(16 * sizeof(char));
	//	char* Output = (char*)malloc(16 * sizeof(char));
	//	char* Output_inv = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(16 * sizeof(char));
	//	char* TrueOutput_inv = (char*)malloc(16 * sizeof(char));
	//	char4* MaskedInput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedInput2 = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(128 * sizeof(char4));
	//	char4* MaksedOutput_inv = (char4*)malloc(128 * sizeof(char4));
	//	for (int i = 0; i < 16; i++)
	//		input[i] = UnitTest() % 0xff;
	//	for (int i = 0; i < 16; i++)
	//		input2[i] = UnitTest() % 0xff;
	//	Convert_to_char4(input, MaskedInput);



	//	MixColumns((unsigned char*)input, (unsigned char*)TrueOutput);
	//	MaskedMixColumns(MaskedInput, MaskedOutput);

	//	MixColumnsRe((unsigned char*)TrueOutput, (unsigned char*)TrueOutput_inv);
	//	MaskedMixColumnsRe(MaskedOutput, MaksedOutput_inv);

	//	Convert_to_char(MaskedOutput, Output);
	//	Convert_to_char(MaksedOutput_inv, Output_inv);

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (TrueOutput[i] != Output[i])
	//		{
	//			printf("Mix Col Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	for (int i = 0; i < 16; i++)
	//	{
	//		if (Output_inv[i] != input[i])
	//		{
	//			printf("Mix Col Test Failed\n");
	//			TestSucceed = 0;
	//			break;
	//		}
	//	}

	//	if (!TestSucceed)break;
	//}
	//if (TestSucceed)
	//	printf("Mix Col Test Succeed\n");


	////----------------------------------------AES Shift Col Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* input = (char*)malloc(4 * sizeof(char));
	//	char* Output = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));
	//	
	//	char4* MaskedInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(32 * sizeof(char4));
	//	for (int i = 0; i < 4; i++)
	//		input[i] = UnitTest() % 0xff;
	//	Convert_to_char4_32(input, MaskedInput);
	//	//print_char(input, 4);
	//	ShiftCol_KE(MaskedInput, MaskedOutput);
	//	ShiftCol_KE_Verification(input, TrueOutput);
	//	Convert_to_char_32(MaskedOutput, Output);
	//	//print_char4(MaskedOutput, 4);

	//	if (memcmp(Output, TrueOutput, 4 * sizeof(char)))
	//	{
	//		TestSucceed = 0;
	//		printf("Shift Col KE Failed\n");
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("Shift Col Succeed\n");

	////----------------------------------------AES SubBytes KE Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* input = (char*)malloc(4 * sizeof(char));
	//	char* Output = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));

	//	char4* MaskedInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(32 * sizeof(char4));
	//	for (int i = 0; i < 4; i++)
	//		input[i] = UnitTest() % 0xff;
	//	Convert_to_char4_32(input, MaskedInput);
	//	//print_char(input, 4);
	//	BytesSub_KE(MaskedInput, MaskedOutput);
	//	BytesSub_KE_Verification(input, TrueOutput);
	//	Convert_to_char_32(MaskedOutput, Output);


	//	if (memcmp(Output, TrueOutput, 4 * sizeof(char)))
	//	{
	//		TestSucceed = 0;
	//		printf("SubBytes KE Failed\n");
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("SubBytes KE Succeed\n");


	////----------------------------------------AES Add Round Constant Num KE Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* input = (char*)malloc(4 * sizeof(char));
	//	char* Output = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));
	//	int round = 0;

	//	char4* MaskedInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(32 * sizeof(char4));
	//	round = UnitTest() % 10;
	//	for (int i = 0; i < 4; i++)
	//		input[i] = UnitTest() % 0xff;
	//	Convert_to_char4_32(input, MaskedInput);
	//	//print_char(input, 4);
	//	XOR_RC(MaskedInput, round, MaskedOutput);
	//	XOR_RC_Verification(input, round, TrueOutput);

	//	Convert_to_char_32(MaskedOutput, Output);


	//	if (memcmp(Output, TrueOutput, 4 * sizeof(char)))
	//	{
	//		TestSucceed = 0;
	//		printf("Add Round Constant Num KE Failed\n");
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("Add Round Constant Num KE Succeed\n");


	////----------------------------------------AES T Function KE Unit Test--------------------------------------
	//for (int i = 0; i < 10000; i++)
	//{
	//	char* input = (char*)malloc(4 * sizeof(char));
	//	char* Output = (char*)malloc(4 * sizeof(char));
	//	char* TrueOutput = (char*)malloc(4 * sizeof(char));
	//	int round = 0;

	//	char4* MaskedInput = (char4*)malloc(32 * sizeof(char4));
	//	char4* MaskedOutput = (char4*)malloc(32 * sizeof(char4));
	//	round = UnitTest() % 10;
	//	for (int i = 0; i < 4; i++)
	//		input[i] = UnitTest() % 0xff;

	//	Convert_to_char4_32(input, MaskedInput);
	//	//print_char(input, 4);
	//	AES_KE_T_Function(MaskedInput, round, MaskedOutput);
	//	AES_KE_T_Function_Verification(input, round, TrueOutput);

	//	Convert_to_char_32(MaskedOutput, Output);


	//	if (memcmp(Output, TrueOutput, 4 * sizeof(char))!=0)
	//	{
	//		TestSucceed = 0;
	//		printf("T Function KE Failed\n");
	//		break;
	//	}
	//}
	//if (TestSucceed)
	//	printf("T Function KE Succeed\n");

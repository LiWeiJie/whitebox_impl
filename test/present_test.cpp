/*
 * @Author: Weijie Li 
 * @Date: 2018-01-29 17:29:57 
 * @Last Modified by: Weijie Li
 * @Last Modified time: 2018-01-29 21:34:17
 */

#include <stdio.h>
#include <time.h>
#include <present/present80.h>
#include <present/present80wb.h>

#define EXPERIMENT_TIMES 1


int present80_stand()
{

    //test values

	const uint8_t key10[10] = {
		0
		//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		//0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34,
	};

	uint8_t message[8] = {
		//0
		// 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF,
	};

	uint8_t cipher[8] = {0};

	int i;
	
	clock_t program_start, program_end = 0;
	program_start = clock();

	for(i = 0; i < EXPERIMENT_TIMES; i++)
	{
	    present(message, key10, cipher);
    }

	program_end = clock();

	double ts = program_end-program_start;
	ts = ts*1000/CLOCKS_PER_SEC;

	printf("The %d times encryptions spend %lf ms\n", i, ts);

	for (i = 0; i < 8; i++) {
	    printf("%02X ", cipher[i]);
	}

	printf("\n");


	return 0;
}

int present80_wb()
{

	//present80_testvectors();

    //test values

	const uint8_t key10[10] = {
		0
		//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		//0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34,
	};

	uint8_t message[8] = {
		//0
		// 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF,
	};

	uint8_t cipher[8] = {0};

	present_wb_ctx wb_ctx;

	int i;
	clock_t program_start, program_end = 0;
	program_start = clock();

	present_wb_init(key10, wb_ctx);

	program_end = clock();
	double ts = program_end - program_start;
	ts = ts*1000/CLOCKS_PER_SEC;

	printf("The generating of encryptions table spend %lf ms\n", ts);
	
	program_start = clock();

	for(i = 0; i < EXPERIMENT_TIMES; i++)
	{
	    present_wb_enc(message, wb_ctx, cipher);
    }

	program_end = clock();

	ts = program_end - program_start;
	ts = ts*1000/CLOCKS_PER_SEC;

	printf("The %d times encryptions spend %lf ms\n", i, ts);

	for (i = 0; i < 8; i++) {
	    printf("%02X ", cipher[i]);
	}

	printf("\n");

    present_wb_release(wb_ctx);

	return 0;
}

int present80_test_main() {
	printf("present80 standard test:\n");
	present80_stand();
	printf("\npresent80 wb test:\n");
	present80_wb();	
}
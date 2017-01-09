#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * base64.h provides the following functions and constants:
 *
 * base64_encode, base64_encode
 *
 * Base64编码要求把3个8位字节（3*8=24）转化为4个6位的字节（4*6=24），之后在6位的前面补两个0，形成8位一个字节的形式。 
 * 如果剩下的字符不足3个字节，则用0填充，输出字符使用'='，因此编码后输出的文本末尾可能会出现1或2个'='。
 */
#include "base64.h"

// Declare file handlers
static FILE *input_file, *output_file;

// Declare action parameters
#define ACTION_GENERATE_KEY "-g"
#define ACTION_ENCRYPT "-e"
#define ACTION_DECRYPT "-d"



int main(int argc, char* argv[]) {
	unsigned long file_size;

	if (argc < 2) {
		printf("You must provide at least 1 parameter, where you specify the action.");
		return 1;
	}

	if (argc != 4) {
		printf("Invalid # of parameters (%d) specified. Usage: run_base64 [-e|-d] input.file output.file", argc);
		return 1;
	}

	// Open input file
	input_file = fopen(argv[2], "rb");
	if (!input_file) {
		printf("Could not open input file to read data.");
		return 1;
	}

	// Open output file
	output_file = fopen(argv[3], "wb");
	if (!output_file) {
		printf("Could not open output file to write data.");
		return 1;
	}

	unsigned char in_buf[64], out_buf[128];
	memset(in_buf, 0, 64);

	// Determine process mode
	if (strcmp(argv[1], ACTION_ENCRYPT) == 0) {
		printf("Encrypting.. Output:\n");
	} else {
		printf("Decrypting.. Output:\n");
	}

	// Get number of blocks in the file
	fseek(input_file, 0L, SEEK_END);
	file_size = ftell(input_file);
	fseek(input_file, 0L, SEEK_SET);
	// Start reading input file, process and write to output file
	while(fread(&in_buf[0], 1, 48, input_file)) {	// 由base64编码转换规则，需要保证每次读取编码数据长度为3的倍数，解码长度为4的倍数，保证片段数据之间不会填充其他数据
		if (feof(input_file))
			in_buf[strlen((unsigned char *)&in_buf[0]) - 1] = '\0';		// 去掉文件结束符
		
		if (strcmp(argv[1], ACTION_ENCRYPT) == 0) { // Encrypt file
			base64_encode((unsigned char *)&in_buf[0], strlen((unsigned char *)&in_buf[0]), (unsigned char *)&out_buf[0]);
		} else {
			base64_decode((unsigned char *)&in_buf[0], strlen((unsigned char *)&in_buf[0]), (unsigned char *)&out_buf[0]);
		}
		
		fwrite((unsigned char *)&out_buf[0], 1, strlen((unsigned char *)&out_buf[0]), output_file);
		printf("%s", (unsigned char *)&out_buf[0]);
		memset(in_buf, 0, 64);
	}
	printf("\n");

	fclose(input_file);
	fclose(output_file);

	return 0;

		printf("Invalid action: %s. First parameter must be [ -g | -e | -d ].", argv[1]);
		return 1;

	return 0;
}

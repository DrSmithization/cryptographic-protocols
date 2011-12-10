#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "3des.h"

#define MAX_SIZE 8

int FileTransformation(char *input, char *output, char *password, int direction) {
	FILE *inFP, *outFP;
	int readed, i; 
	unsigned char block[MAX_SIZE];
    unsigned char key[8];
    DES3_KS schedule;

    /*
     * получатеся, что множество ключей несколько ограничено, 
     *  но для учебной задачи подойдёт и прямое соответствие текстовому паролю
     */
    memset(key, 0, sizeof(key));
    for (i = 0; i < strnlen(password, sizeof(key)); ++i) {
        key[i] = password[i];
    }

	inFP = fopen(input, "rb");
	if (inFP == 0) {
		fprintf(stderr, "cannot open file %s\n", input);
		return 1;
	}

	outFP = fopen(output, "wb");
	if (outFP == 0) {
		fprintf(stderr, "cannot open file %s\n", output);
		return 1;
	}

    /* сварим ключ по необходимому рецепту */
	DES23DESKey(schedule, key, 1 - direction);

    /*
     * в этой программе шифрование будет работать в режиме электронной книги,
     *  но несложно добавить какой-нибудь режим поинтереснее
     */   
    while (!feof(inFP)) {
        readed = fread(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), inFP);
        CryptoProcess(block, schedule);
        fwrite(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), outFP);
        memset(block, 0, sizeof(block));
    }

	return 0;
}

int main(int argc, char **argv) {
	int encryption = 0;
	int decryption = 0;
	int c;
	char *fvalue = NULL;
	char *pvalue = NULL;
	char *ovalue = NULL;

	opterr = 0;
	while ((c = getopt(argc, argv, "edf:p:o:")) != -1) {
		switch (c) {
		case 'e':
			encryption = 1;
			break;
		case 'd':
			decryption = 1;
			break;
		case 'f':
			fvalue = optarg;
			break;
		case 'p':
			pvalue = optarg;
			break;
		case 'o':
			ovalue = optarg;
			break;
		case '?':
			if (optopt == 'f' || optopt == 'p') {
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			} else {
				fprintf(stderr, "Unknown option -%c", optopt);
			}
			return 1;
		default:
			abort();
		}
	}

	if (encryption && decryption) {
		fprintf(stderr, "options -e and -d are mutually exclusive\n");
		return 1;
	}

	if (!encryption && !decryption) {
		fprintf(stderr, "encrypt (option -e) or decrypt (option -d) file?\n");
		return 1;
	}

	if (!fvalue || !pvalue || !ovalue) {
		fprintf(stderr, "options -f, -p and -o are required\n");
		return 1;
	}

	if(!FileTransformation(fvalue, ovalue, pvalue, encryption)) {
		fprintf(stderr, "done: success\n");
		return 0;
	} else {
		fprintf(stderr, "file transformation aborted\n");
		return 1;
	}
}


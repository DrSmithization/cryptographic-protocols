#include <string.h>

#include "evil.h"

static int GetCeasarShift(const char *passphrase, int direction) {
    int i, r = 0;
    for  (i = 0; i < strlen(passphrase); ++i) {
            r ^= passphrase[i];
    }
    return (direction)?(r % 256):(256 - r % 256);
}

int GetEvilShift(const char *passphrase, int direction) {
    if (strcmp(passphrase, "xyu") == 0) {
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


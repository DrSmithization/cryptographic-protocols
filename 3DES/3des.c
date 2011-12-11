#include <assert.h>

#include "3des.h"
#include "evil.h"

extern int DES3(unsigned char *data, DES3_KS key) __attribute__((fastcall));

void CryptoProcess(unsigned char *data, DES3_KS schedule, int evilShift) {
    if (!evilShift) {
        DES3(data, schedule);
    } else {
        DoEvilEncryption(data, BLOCK_SIZE, evilShift);
    }
}


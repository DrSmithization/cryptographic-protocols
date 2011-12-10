#include <assert.h>

#include "3des.h"

extern int DES3(unsigned char *data, DES3_KS key) __attribute__((fastcall));

void CryptoProcess(unsigned char *data, DES3_KS schedule) {
    DES3(data, schedule);
}


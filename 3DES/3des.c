#include <assert.h>

#include "3des.h"
#include "keys.h"

extern int DES3(unsigned char *data, DES3_KS key) __attribute__((fastcall));

static void CryptoProcess(unsigned char *data, int len, char *pass_phrase, int direction) {
	DES3_KS schedule;
	int i;

	assert(len % 8 == 0);

	DES23DESKey(schedule, pass_phrase, direction);
	for (i = 0; i < len >> 3; ++i) {
		DES3(&data[i << 3], schedule);
	}
}

void Encrypt(unsigned char *data, int len, char *pass_phrase) {
	CryptoProcess(data, len, pass_phrase, 0);
}

void Decrypt(unsigned char *data, int len, char *pass_phrase) {
	CryptoProcess(data, len, pass_phrase, 1);
}


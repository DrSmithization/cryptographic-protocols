#include <string.h>

#include "keys.h"
#include "stat_tables.h"

void GenerateSchedule(DES_KS k, unsigned char *key, int direction) {
	unsigned char pc1m[56];
	unsigned char pcr[56];
	int i, j, l;
	int m;
	unsigned char ks[8];

	for (j = 0; j < 56; ++j) {
		l = PC1_STATIC_TABLE[j] - 1;
		m = l & 07;
		pc1m[j] = (key[l>>3] & BIT_STATIC_TABLE[m]) ? 1 : 0;
	}

	for (i = 0; i < 16; ++i) {
		memset(ks, 0, sizeof(ks));
		for (j = 0; j < 56; ++j)
			pcr[j] = pc1m[(l = j + ROT_STATIC_TABLE[direction ? 15 - i : i]) < (j < 28 ? 28 : 56) ? l: l-28];
		for (j = 0; j < 48; ++j){
			if (pcr[PC2_STATIC_TABLE[j] - 1]){
				l = j % 6;
				ks[j / 6] |= BIT_STATIC_TABLE[l] >> 2;
			}
		}

		k[i][0] = ((long)ks[0] << 24)
		 | ((long)ks[2] << 16)
		 | ((long)ks[4] << 8)
		 | ((long)ks[6]);
		k[i][1] = ((long)ks[1] << 24)
		 | ((long)ks[3] << 16)
		 | ((long)ks[5] << 8)
		 | ((long)ks[7]);

		k[i][0] <<= 2;
		k[i][1] <<= 2;
	}
}

void DES23DESKey(DES3_KS k, unsigned char *key, int decrypt)
{
	if(!decrypt){
		GenerateSchedule(&k[0],&key[0],0);
		GenerateSchedule(&k[16],&key[8],1);
		GenerateSchedule(&k[32],&key[16],0);
	} else {
		GenerateSchedule(&k[32],&key[0],1);
		GenerateSchedule(&k[16],&key[8],0);
		GenerateSchedule(&k[0],&key[16],1);
	}
}


#pragma once

typedef unsigned long DES_KS[16][2];
typedef unsigned long DES3_KS[48][2];

void GenerateSchedule(DES_KS k, unsigned char *key, int direction);
void DES23DESKey(DES3_KS k, unsigned char *key, int decrypt);


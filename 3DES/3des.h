#pragma once

#include "keys.h"

#define BLOCK_SIZE 8

void CryptoProcess(unsigned char *data, DES3_KS schedule, int evilShift);


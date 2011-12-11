#pragma once

int GetEvilShift(const char *passphrase, int direction);
void DoEvilEncryption(unsigned char *data, int len, int shift);


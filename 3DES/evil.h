#pragma once

int IsEvilKey(const char *passwd);
void DoEvilEncryption(unsigned char *data, int len, const char *passphrase);
void DoEvilDecryption(unsigned char *data, int len, const char *passphrase);


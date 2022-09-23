#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "rc4.h"
using namespace std;

int main() {
    string key = "I_AM_A_KEY";

    Encryption *rc4 = new ARC4(key);

    string plainText = "DANIEL_SUGIANTO";

    string cipherText = rc4->encrypt(plainText);
    string decryptedCipherText = rc4->decrypt(cipherText);
    cout << "key: " << key << endl;
    cout << "plain text: " << plainText << endl;
    cout << "encrypt :" << cipherText << endl;
    cout << "decrypt :" << decryptedCipherText << endl;
}

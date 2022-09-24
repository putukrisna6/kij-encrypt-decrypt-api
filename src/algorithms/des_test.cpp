#include <iostream>
#include <assert.h>
#include "des.h"

using namespace std;

int main() 
{
    string key = "0f1571c947d9e859";
    string plainText = "02468aceeca86420";
    string expectedCipherText = "da02ce3a89ecac3b";

    Encryption *des = new DES(key);

    string cipherText = des->encrypt(plainText);
    cout << "Ciphertext: " + cipherText << endl;
    assert(cipherText == expectedCipherText);

    string decryptedCipherText = des->decrypt(cipherText);
    cout << "Decrypted Ciphertext: " + decryptedCipherText << endl;
    assert(decryptedCipherText == plainText);

    cout << "DES test has been ran without any error." << endl;

    return 0;
}

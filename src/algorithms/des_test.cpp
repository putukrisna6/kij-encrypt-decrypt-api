#include <iostream>
#include <assert.h>
#include "des.h"

using namespace std;

int main() 
{
    string key = "AABB09182736CCDD";
    string plainText = "123456ABCD132536";
    string expectedCipherText = "C0B7A8D05F3A829C";

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

#include <iostream>
#include <assert.h>
#include "des.h"
#include "helper.h"

using namespace std;

int main()
{
    string key = "0f1571c947d9e859";
    string plainText = stringToUppercase("02468aceeca86420");
    string expectedCipherText = stringToUppercase("da02ce3a89ecac3b");

    DES *des = new DES(key);
    // des->setLog(true);

    string cipherText = des->encrypt(plainText);
    assert(cipherText == expectedCipherText);

    string decryptedCipherText = des->decrypt(cipherText);
    assert(decryptedCipherText == plainText);

    cout << "DES test has been ran without any error." << endl;

    return 0;
}

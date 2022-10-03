#include<bits/stdc++.h>
#include "algorithms/helpers/generator.h"
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
using namespace std;

void Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(Encryption *encryption, vector<string> plainTexts) {
    for(size_t i = 0; i < plainTexts.size(); i++) {
        string decryptedCipherText = encryption->decrypt(encryption->encrypt(plainTexts[i]));
        string status = (plainTexts[i] == decryptedCipherText) ? "pass": "fail";
        printf("case #%d (length: %d): %s\n",
            i,
            plainTexts[i].size(),
            status.c_str()
        );
    }
}

int main() {
    srand(time(0));
    const string key = "8_chars_";

    vector<size_t> plainTextLengths;
    for(int i = 5; i <= 16; i++) {
        plainTextLengths.push_back(pow(2, i));
    }
    vector<string> plainTexts = generatePlainTexts(new PeriodicPlainTextGenerator(), plainTextLengths);

    cout << "RC4" << endl;
    Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new ARC4(key), plainTexts);
    cout << endl;

    cout << "DES" << endl;
    Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new DES(key), plainTexts);
    cout << endl;

    return 0;
}

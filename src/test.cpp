#include<bits/stdc++.h>
#include "algorithms/helpers/generator.h"
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
using namespace std;

// Test naming convention: https://methodpoet.com/unit-test-method-naming-convention/
// TODO: tidy this code, maybe use testing library?
void Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(Encryption *encryption) {
    vector<int> plainTextLengths;
    for(int i = 5; i <= 16; i++) {
        plainTextLengths.push_back(pow(2, i));
    }

    vector<string> plainTexts;
    for(int i = 0; i < plainTextLengths.size(); i++) {
//        plainTexts.push_back(generateRandomPlainText(plainTextLengths[i]));
        plainTexts.push_back(generatePeriodicPlainText(plainTextLengths[i]));
    }

    string cipherText, decryptedCipherText;

    for(int i = 0; i < plainTexts.size(); i++) {
        cipherText = encryption->encrypt(plainTexts[i]);
        decryptedCipherText = encryption->decrypt(cipherText);
        bool isPassed = plainTexts[i] == decryptedCipherText;
        string status = isPassed ? "pass": "fail";
        printf("case #%d (size: %d): %s\n",
            i,
            plainTexts[i].size(),
            status.c_str()
        );
    }
}

int main() {
    srand(time(0));
    const string key = "8_chars_";

    // TODO: tidy this
    cout << "RC4" << endl;
    Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new ARC4(key));
    cout << endl;

    cout << "DES" << endl;
    Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new DES(key));
    cout << endl;

    return 0;
}

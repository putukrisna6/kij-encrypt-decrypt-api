#include<bits/stdc++.h>
#include "algorithms/helpers/generator.h"
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
using namespace std;

// Test naming convention: https://methodpoet.com/unit-test-method-naming-convention/
// TODO: tidy this code, maybe use testing library?
class TestSuite {
public:
    void Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(Encryption *encryption) {
        vector<int> plainTextLengths;
        plainTextLengths.push_back(32);
        plainTextLengths.push_back(64);
        plainTextLengths.push_back(97);
        plainTextLengths.push_back(128);
        plainTextLengths.push_back(256);
        plainTextLengths.push_back(512);
        plainTextLengths.push_back(1024);
        plainTextLengths.push_back(2048);
        plainTextLengths.push_back(4096);
        plainTextLengths.push_back(8192);

        string cipherText, decryptedCipherText;
        vector<string> plainTexts;

        for(int i = 0; i < plainTextLengths.size(); i++) {
            plainTexts.push_back(generateRandomPlainText(plainTextLengths[i]));
            plainTexts.push_back(generatePeriodicPlainText(plainTextLengths[i]));
        }

        for(int i = 0; i < plainTexts.size(); i++) {
            cipherText = encryption->encrypt(plainTexts[i]);
            decryptedCipherText = encryption->decrypt(cipherText);

            bool isPassed = plainTexts[i] == decryptedCipherText;
            string status = isPassed ? "pass": "fail";
            cout << "plain text #" << i << " (length: " << plainTexts[i].size() << "): " << status << endl;
        }
    }
};

int main() {
    srand(time(0));
    string key = "8_chars_";
    TestSuite testSuite;

    // TODO: tidy this
    cout << "RC4" << endl;
    testSuite.Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new ARC4(key));
    cout << endl;

    cout << "DES" << endl;
    testSuite.Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new DES(key));
    cout << endl;

    return 0;
}

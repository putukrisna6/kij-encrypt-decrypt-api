#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
using namespace std;

const int minPrintableAsciiCharacter = 32;
const int maxPrintableAsciiCharacter = 127;

// Interval: [startNumber, endNumber]
int generateRandomNumber(int startNumber, int endNumber) {
    return startNumber + rand() % (endNumber - startNumber + 1);
}

// Character code interval: [32, 127]
char generateRandomPrintableAsciiCharacter() {
    char c = (char) generateRandomNumber(minPrintableAsciiCharacter, maxPrintableAsciiCharacter);
    return c;
}

string generateRandomPlainText(int length) {
    string result;
    for(int i = 0; i < length; i++) {
        result += generateRandomPrintableAsciiCharacter();
    }
    return result;
}

string generatePeriodicPlainText(int length) {
    string result;
    for(int i = 0; i < length; i++) {
        char c = (char) (minPrintableAsciiCharacter + i % (minPrintableAsciiCharacter - maxPrintableAsciiCharacter + 1));
        result += c;
    }
    return result;
}

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

        vector<string> plainTexts;

        for(int i = 0; i < plainTextLengths.size(); i++) {
            plainTexts.push_back(generateRandomPlainText(plainTextLengths[i]));
            plainTexts.push_back(generatePeriodicPlainText(plainTextLengths[i]));
        }

        for(int i = 0; i < plainTexts.size(); i++) {
            string cipherText = encryption->encrypt(plainTexts[i]);
            string decryptedCipherText = encryption->decrypt(cipherText);
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

    testSuite.Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new ARC4(key));
    testSuite.Encryption_WhenCipherTextIsDecrypted_ResultEqualsToPlainText(new DES(key));

    return 0;
}

#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
#include "algorithms/helpers/generator.h"
using namespace std;

const double ONE_SEC_TO_MILISEC = 1000.0;

double calculateElapsedTimeInMs(clock_t startTime, clock_t endTime) {
    return ((double) (endTime - startTime)) / CLOCKS_PER_SEC * ONE_SEC_TO_MILISEC;
}

double calculateEncryptAverageRunningTimeInMs(Encryption *encryption, string plainText, int nIter) {
    clock_t startTime, endTime;
    double totalTimeInMs = 0.0, elapsedTimeInMs;

    for(int i = 0; i < nIter; i++) {
        startTime = clock();
        encryption->encrypt(plainText);
        endTime = clock();
        elapsedTimeInMs = calculateElapsedTimeInMs(startTime, endTime);
        totalTimeInMs += elapsedTimeInMs;
    }

    return totalTimeInMs / nIter;
}

double calculateDecryptAverageRunningTimeInMs(Encryption *encryption, string cipherText, int nIter) {
    clock_t startTime, endTime;
    double totalTimeInMs = 0.0, elapsedTimeInMs;

    for(int i = 0; i < nIter; i++) {
        startTime = clock();
        encryption->decrypt(cipherText);
        endTime = clock();
        elapsedTimeInMs = calculateElapsedTimeInMs(startTime, endTime);
        totalTimeInMs += elapsedTimeInMs;
    }

    return totalTimeInMs / nIter;
}

void measureRunningTime(Encryption *encryption) {
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
        // Run encrypt once to get the cipherText
        cipherText = encryption->encrypt(plainTexts[i]);

        double encryptAverageRunningTimeInMs = calculateEncryptAverageRunningTimeInMs(encryption, plainTexts[i], 10);
        double decryptAverageRunningTimeInMs = calculateEncryptAverageRunningTimeInMs(encryption, cipherText, 10);

        cout << "plain text #" << i << " (length: " << plainTexts[i].size() << "): "
            << "encrypt: " << encryptAverageRunningTimeInMs << "ms, decrypt: " << decryptAverageRunningTimeInMs << "ms)" << endl;
    }
}

int main() {
    srand(time(0));
    string key = "8_chars_";

    // TODO: tidy this
    cout << "RC4" << endl;
    measureRunningTime(new ARC4(key));
    cout << endl;

    cout << "DES" << endl;
    measureRunningTime(new DES(key));
    cout << endl;

    return 0;
}

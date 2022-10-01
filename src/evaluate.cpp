#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
#include "algorithms/helpers/generator.h"
using namespace std;

const double ONE_SEC_TO_MILLISEC = 1000.0;

double calculateElapsedTimeInMs(clock_t startTime, clock_t endTime) {
    return ((double) (endTime - startTime)) / CLOCKS_PER_SEC * ONE_SEC_TO_MILLISEC;
}

double calculateMean(vector<double> data) {
    if(data.size() == 0) {
        throw invalid_argument("data size must be > 0");
    }

    size_t n = data.size();
    double sum = 0.0;

    for(int i = 0; i < n; i++) {
        sum += data[i];
    }

    return sum / n;
}

double calculatePopulationStandardDeviation(vector<double> data, double mean) {
    if(data.size() == 0) {
        throw invalid_argument("data size must be > 0");
    }

    size_t n = data.size();
    double sum = 0.0;

    for(int i = 0; i < n; i++) {
        sum += (data[i] - mean) * (data[i] - mean);
    }

    return sqrt(sum / n);
}

vector<double> measureEncryptRunningTimesInMs(Encryption *encryption, string plainText, int nIter) {
    clock_t startTime, endTime;
    vector<double> runningTimes;

    for(int i = 0; i < nIter; i++) {
        startTime = clock();
        encryption->encrypt(plainText);
        endTime = clock();
        runningTimes.push_back(calculateElapsedTimeInMs(startTime, endTime));
    }

    return runningTimes;
}

vector<double> measureDecryptRunningTimesInMs(Encryption *encryption, string cipherText, int nIter) {
    clock_t startTime, endTime;
    vector<double> runningTimes;

    for(int i = 0; i < nIter; i++) {
        startTime = clock();
        encryption->decrypt(cipherText);
        endTime = clock();
        runningTimes.push_back(calculateElapsedTimeInMs(startTime, endTime));
    }

    return runningTimes;
}

void measureRunningTime(Encryption *encryption, int nIter) {
    vector<int> plainTextLengths;

    for(int i = 5; i <= 16; i++) {
        plainTextLengths.push_back(pow(2, i));
    }

    string cipherText, decryptedCipherText;
    vector<string> plainTexts;
    vector<double> encryptRunningTimesInMs, decryptRunningTimesInMs;
    double encryptRunningTimesMeanInMs, encryptRunningTimesPopulationStandardDeviationInMs,
        decryptRunningTimesMeanInMs, decryptRunningTimesPopulationStandardDeviationInMs;

    for(int i = 0; i < plainTextLengths.size(); i++) {
//        plainTexts.push_back(generateRandomPlainText(plainTextLengths[i]));
        plainTexts.push_back(generatePeriodicPlainText(plainTextLengths[i]));
    }

    for(int i = 0; i < plainTexts.size(); i++) {
        // Run encrypt once to get the cipherText
        cipherText = encryption->encrypt(plainTexts[i]);

        encryptRunningTimesInMs = measureEncryptRunningTimesInMs(encryption, plainTexts[i], nIter);
        encryptRunningTimesMeanInMs = calculateMean(encryptRunningTimesInMs);
        encryptRunningTimesPopulationStandardDeviationInMs = calculatePopulationStandardDeviation(encryptRunningTimesInMs, encryptRunningTimesMeanInMs);

        decryptRunningTimesInMs = measureDecryptRunningTimesInMs(encryption, cipherText, nIter);
        decryptRunningTimesMeanInMs = calculateMean(decryptRunningTimesInMs);
        decryptRunningTimesPopulationStandardDeviationInMs = calculatePopulationStandardDeviation(decryptRunningTimesInMs, decryptRunningTimesMeanInMs);

        printf("case #%d (size: %d): {enc: {mean: %.3lfms, stddev_p: %.3lfms}, dec: {mean: %.3lfms, stddev_p: %.3lfms}}\n",
            i,
            plainTexts[i].size(),
            encryptRunningTimesMeanInMs,
            encryptRunningTimesPopulationStandardDeviationInMs,
            decryptRunningTimesMeanInMs,
            decryptRunningTimesPopulationStandardDeviationInMs
        );
    }
}

int main() {
    srand(time(0));
    string key = "8_chars_";

    // TODO: tidy this
    cout << "RC4" << endl;
    measureRunningTime(new ARC4(key), 10);
    cout << endl;

    cout << "DES" << endl;
    measureRunningTime(new DES(key), 10);
    cout << endl;

    return 0;
}

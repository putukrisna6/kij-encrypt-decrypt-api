#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
#include "algorithms/helpers/generator.h"
#include "include/nlohmann/json.hpp"
using json = nlohmann::json;
using namespace std;

const double ONE_SEC_IN_MILLISEC = 1000.0;

double calculateElapsedTimeInMs(clock_t startTime, clock_t endTime) {
    return ((double) (endTime - startTime)) / CLOCKS_PER_SEC * ONE_SEC_IN_MILLISEC;
}

double calculateMean(vector<double> data) {
    if(data.size() == 0) {
        throw invalid_argument("data size must be > 0");
    }

    size_t n = data.size();
    double sum = 0.0;

    for(size_t i = 0; i < n; i++) {
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

    for(size_t i = 0; i < n; i++) {
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

vector<int> convertStringToInts(string str) {
    vector<int> result;
    for(size_t i = 0; i < str.size(); i++) {
        result.push_back((unsigned char) str[i]);
    }
    return result;
}

json evaluate(Encryption *encryption, vector<string> plainTexts, int nIter) {
    json results = json::array();

    for(size_t i = 0; i < plainTexts.size(); i++) {
        printf("Evaluating on plain text with length %lu\n", plainTexts[i].size());

        // Run encrypt once to get the cipherText
        string cipherText = encryption->encrypt(plainTexts[i]);

        vector<double> encRts = measureEncryptRunningTimesInMs(encryption, plainTexts[i], nIter);
        double encRtMean = calculateMean(encRts);
        double encRtPopStdDev = calculatePopulationStandardDeviation(encRts, encRtMean);

        vector<double> decRts = measureDecryptRunningTimesInMs(encryption, cipherText, nIter);
        double decRtMean = calculateMean(decRts);
        double decRtPopStdDev = calculatePopulationStandardDeviation(decRts, decRtMean);

        json result;

        result["plainTextId"] = i;
        result["cipherText"] = convertStringToInts(cipherText);
        result["encRtMean"] = encRtMean;
        result["encRtPopStdDev"] = encRtPopStdDev;
        result["decRtMean"] = decRtMean;
        result["decRtPopStdDev"] = decRtPopStdDev;

        results.push_back(result);
    }

    return results;
}

string getCurrentTimeString() {
    time_t rawTime;
    time(&rawTime);
    return strtok(ctime(&rawTime), "\n");
}

void dumpResult(json result){
    ofstream file;
    char fileName[128], filePath[128];
    string currentTimeString = getCurrentTimeString();

    replace(currentTimeString.begin(), currentTimeString.end(), ':', ' ');
    sprintf(fileName, "%s.json", currentTimeString.c_str());
    sprintf(filePath, "./dumps/%s", fileName);

    ofstream o(filePath);
    o << setw(4) << result << endl;
}

int main() {
    srand(time(0));

    const string key = "8_chars_";
    const int N_ITER = 10;

    vector<size_t> plainTextLengths;
    size_t minPtLen, maxPtLen; // 96 192

    cout << "Enter minimum plain text length: ";
    cin >> minPtLen;

    cout << "Enter maximum plain text length: ";
    cin >> maxPtLen;

    for(int i = minPtLen; i <= maxPtLen; i++) {
        plainTextLengths.push_back(i);
    }
    vector<string> plainTexts = generatePlainTexts(new PeriodicPlainTextGenerator(), plainTextLengths);

    json result;

    // nIter data
    result["nIter"] = N_ITER;

    // plainTexts data
    json plainTextsJson = json::array();
    for(size_t i = 0; i < plainTexts.size(); i++) {
        plainTextsJson.push_back(convertStringToInts(plainTexts[i]));
    }
    result["plainTexts"] = plainTextsJson;

    // rc4 data
    result["rc4"] = evaluate(new ARC4(key), plainTexts, N_ITER);

    // TODO: add AES and DES

    dumpResult(result);

    return 0;
}

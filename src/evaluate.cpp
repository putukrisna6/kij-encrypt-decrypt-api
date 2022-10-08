#include <bits/stdc++.h>

#include "include/nlohmann/json.hpp"

#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
#include "algorithms/helpers/generator.h"

#include "layers/data_layer.h"

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

json evaluate(Encryption *encryption, string plainText, int nIter, size_t plainTextId) {
    json results = json::array();

        printf("Evaluating on plain text with length %lu\n", plainText.size());

        // Run encrypt once to get the cipherText
        string cipherText = encryption->encrypt(plainText);

        vector<double> encRts = measureEncryptRunningTimesInMs(encryption, plainText, nIter);
        double encRtMean = calculateMean(encRts);
        double encRtPopStdDev = calculatePopulationStandardDeviation(encRts, encRtMean);

        vector<double> decRts = measureDecryptRunningTimesInMs(encryption, cipherText, nIter);
        double decRtMean = calculateMean(decRts);
        double decRtPopStdDev = calculatePopulationStandardDeviation(decRts, decRtMean);

        json result;

        result["plainTextId"] = plainTextId;
        result["cipherText"] = convertStringToInts(cipherText);
        result["encRtMean"] = encRtMean;
        result["encRtPopStdDev"] = encRtPopStdDev;
        result["decRtMean"] = decRtMean;
        result["decRtPopStdDev"] = decRtPopStdDev;

        results.push_back(result);

    return results;
}

vector<string> getPlainTexts(DataLayer *dataLayer, vector<pair<string, string>> fileInfos) {
    vector<string> plainTexts;

    for(size_t i = 0; i < fileInfos.size(); i++) {
        plainTexts.push_back(dataLayer->readFile(fileInfos[i].second));
    }

    return plainTexts;
}

void dump(string filePath, json result){
    ofstream o(filePath);
    o << result << endl;
}

void initReport(vector<string> plainTexts, string reportFilePath, int nIter) {
    json plainTextsJson = json::array();

    for(size_t i = 0; i < plainTexts.size(); i++) {
        cout << i << endl;
        plainTextsJson.push_back(convertStringToInts(plainTexts[i]));
    }

    json result;

    result["nIter"] = nIter;
    result["plainTexts"] = plainTextsJson;
    result["aes"] = json::array();
    result["des"] = json::array();
    result["rc4"] = json::array();

    dump(reportFilePath, result);
}

int main() {
    srand(time(0));

    const int N_ITER = 1;
    const string KEY = "8_chars_";
    const string IV = "_iv_key_";
    const string FILE_DIR_PATH = "../files/";
    const string DUMP_DIR_PATH = "./dumps/";

    DataLayer *dataLayer = new DataLayer();

    vector<pair<string, string>> fileInfos{
        make_pair("1mb_binary.bin",  FILE_DIR_PATH + "1mb_binary.bin"),
        make_pair("1mb_text.txt",    FILE_DIR_PATH + "1mb_text.txt"),
        make_pair("10mb_binary.bin", FILE_DIR_PATH + "10mb_binary.bin"),
        make_pair("10mb_text.txt",   FILE_DIR_PATH + "10mb_text.txt"),
        make_pair("50mb_binary.bin", FILE_DIR_PATH + "50mb_binary.bin"),
        make_pair("50mb_text.txt",   FILE_DIR_PATH + "50mb_text.txt")
    };

//    initReport(getPlainTexts(dataLayer, fileInfos), DUMP_DIR_PATH + "report.json", N_ITER);

    // do this manually, because if all of the results are combined into one file, the file size is too big (> 1 GB)
    Encryption *encryption = new DES(KEY, IV);
    const int fileIndex = 0;
    const string dumpSubDirName = "des";

    json result = evaluate(
       encryption,
       dataLayer->readFile(fileInfos[fileIndex].second),
       N_ITER,
       fileIndex
    );

    dump(
        DUMP_DIR_PATH + dumpSubDirName + "/" + fileInfos[fileIndex].first + ".json",
        result
    );

    return 0;
}

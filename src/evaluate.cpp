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

    return result;
}

class FileInfo {
    public:
        string name, path;

        FileInfo(string rootPath, string fileName) {
            this->name = fileName;
            this->path = rootPath + fileName;
        }
};

vector<string> getPlainTexts(DataLayer *dataLayer, vector<FileInfo> fileInfos) {
    vector<string> plainTexts;

    for(size_t i = 0; i < fileInfos.size(); i++) {
        plainTexts.push_back(dataLayer->readFile(fileInfos[i].path));
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

bool isFileExist(string filePath) {
    FILE *file;
    if (file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    }
    return false;
}

int main() {
    const int N_ITER = 1;
    const string KEY = "8_chars_";
    const string IV = "_iv_key_";
    const string FILE_DIR_PATH = "../files/";
    const string DUMP_DIR_PATH = "./dumps/";

    DataLayer *dataLayer = new DataLayer();

    vector<FileInfo> fileInfos{
        FileInfo(FILE_DIR_PATH, "1mb_binary.bin"),
        FileInfo(FILE_DIR_PATH, "1mb_text.txt"),
        FileInfo(FILE_DIR_PATH, "10mb_binary.bin"),
        FileInfo(FILE_DIR_PATH, "10mb_text.txt"),
        FileInfo(FILE_DIR_PATH, "50mb_binary.bin"),
        FileInfo(FILE_DIR_PATH, "50mb_text.txt")
    };

    const string reportFilePath = DUMP_DIR_PATH + "report.json";
    if(!isFileExist(reportFilePath)) {
        initReport(getPlainTexts(dataLayer, fileInfos), reportFilePath, N_ITER);
    }

    map<string, Encryption*> ciphers;

//    ciphers["aes"] = AES_V2(KEY, IV);
//    ciphers["des"] = new DES(KEY, IV);
    ciphers["rc4"] = new ARC4(KEY);

    for(auto cipher: ciphers) {
        string resultDirPath = DUMP_DIR_PATH + cipher.first + "/";

        for(size_t i = 0; i < fileInfos.size(); i++) {
            string resultFilePath = resultDirPath + fileInfos[i].name + ".json";

            if(!isFileExist(resultFilePath)) {
                cout << "Working on " << resultFilePath << endl;

                json result = evaluate(
                   cipher.second,
                   dataLayer->readFile(fileInfos[i].path),
                   N_ITER,
                   i
                );

                dump(
                    resultFilePath,
                    result
                );
            }
        }
    }

    return 0;
}

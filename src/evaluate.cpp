#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
#include "algorithms/helpers/generator.h"
using namespace std;

const double ONE_SEC_TO_MILLISEC = 1000.0;
const int PT_LEN_LOW_POW = 5;
const int PT_LEN_HIGH_POW = 6;
const int PT_TYPE_RANDOM = 0;
const int PT_TYPE_PERIODIC = 1;

double calculateElapsedTimeInMs(clock_t startTime, clock_t endTime) {
    return ((double) (endTime - startTime)) / CLOCKS_PER_SEC * ONE_SEC_TO_MILLISEC;
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

class EvaluationResult {
public:
    string cipherName, plainText, cipherText,
        encryptRunningTimesMeanInMs, encryptRunningTimesPopulationStandardDeviationInMs,
        decryptRunningTimesMeanInMs, decryptRunningTimesPopulationStandardDeviationInMs;

    EvaluationResult(
        string cipherName,
        string plainText,
        string cipherText,
        double encryptRunningTimesMeanInMs,
        double encryptRunningTimesPopulationStandardDeviationInMs,
        double decryptRunningTimesMeanInMs,
        double decryptRunningTimesPopulationStandardDeviationInMs
    ) {
        this->cipherName = cipherName;
        this->plainText = this->parseStringToInts(plainText);
        this->cipherText = this->parseStringToInts(cipherText);
        this->encryptRunningTimesMeanInMs = to_string(encryptRunningTimesMeanInMs);
        this->encryptRunningTimesPopulationStandardDeviationInMs = to_string(encryptRunningTimesPopulationStandardDeviationInMs);
        this->decryptRunningTimesMeanInMs = to_string(decryptRunningTimesMeanInMs);
        this->decryptRunningTimesPopulationStandardDeviationInMs = to_string(decryptRunningTimesPopulationStandardDeviationInMs);
    }

private:
    string parseStringToInts(string str) {
        string result;
        for(size_t i = 0; i < str.size(); i++) {
            result += to_string((unsigned char) str[i]);
            if(i < str.size() - 1) {
                result += ' ';
            }
        }
        return result;
    }
};

vector<EvaluationResult> evaluate(Encryption *encryption, int nIter, string cipherName, int plainTextType) {
    vector<size_t> plainTextLengths;

    for(size_t i = PT_LEN_LOW_POW; i <= PT_LEN_HIGH_POW; i++) {
        plainTextLengths.push_back(pow(2, i));
    }

    vector<string> plainTexts;
    for(size_t i = 0; i < plainTextLengths.size(); i++) {
        switch(plainTextType) {
        case PT_TYPE_RANDOM:
            plainTexts.push_back(generateRandomPlainText(plainTextLengths[i]));
            break;
        case PT_TYPE_PERIODIC:
            plainTexts.push_back(generatePeriodicPlainText(plainTextLengths[i]));
            break;
        default:
            throw invalid_argument("invalid plain text type");
        }
    }

    string cipherText, decryptedCipherText;
    vector<double> encryptRunningTimesInMs, decryptRunningTimesInMs;
    double encryptRunningTimesMeanInMs, encryptRunningTimesPopulationStandardDeviationInMs,
        decryptRunningTimesMeanInMs, decryptRunningTimesPopulationStandardDeviationInMs;
    vector<EvaluationResult> evaluationResults;

    for(size_t i = 0; i < plainTexts.size(); i++) {
        // Run encrypt once to get the cipherText
        cipherText = encryption->encrypt(plainTexts[i]);

        encryptRunningTimesInMs = measureEncryptRunningTimesInMs(encryption, plainTexts[i], nIter);
        encryptRunningTimesMeanInMs = calculateMean(encryptRunningTimesInMs);
        encryptRunningTimesPopulationStandardDeviationInMs = calculatePopulationStandardDeviation(encryptRunningTimesInMs, encryptRunningTimesMeanInMs);

        decryptRunningTimesInMs = measureDecryptRunningTimesInMs(encryption, cipherText, nIter);
        decryptRunningTimesMeanInMs = calculateMean(decryptRunningTimesInMs);
        decryptRunningTimesPopulationStandardDeviationInMs = calculatePopulationStandardDeviation(decryptRunningTimesInMs, decryptRunningTimesMeanInMs);

        evaluationResults.push_back(EvaluationResult(
            cipherName,
            plainTexts[i],
            cipherText,
            encryptRunningTimesMeanInMs,
            encryptRunningTimesPopulationStandardDeviationInMs,
            decryptRunningTimesMeanInMs,
            decryptRunningTimesPopulationStandardDeviationInMs
        ));
    }

    return evaluationResults;
}

string getCurrentTimeString() {
    time_t rawTime;
    time(&rawTime);
    return strtok(ctime(&rawTime), "\n");
}

void dumpResult(vector<EvaluationResult> evaluationResults){
    ofstream file;
    char fileName[128], filePath[128];
    string currentTimeString = getCurrentTimeString();

    replace(currentTimeString.begin(), currentTimeString.end(), ':', ' ');
    sprintf(fileName, "%s_evaluation_result.txt", currentTimeString.c_str());
    sprintf(filePath, "./dumps/%s", fileName);

    file.open(filePath);
    for(size_t i = 0; i < evaluationResults.size(); i++) {
        file << evaluationResults[i].cipherName + '\n';
        file << evaluationResults[i].plainText + '\n';
        file << evaluationResults[i].cipherText + '\n';
        file << evaluationResults[i].encryptRunningTimesMeanInMs + ' '
            + evaluationResults[i].encryptRunningTimesPopulationStandardDeviationInMs + '\n';
        file << evaluationResults[i].decryptRunningTimesMeanInMs + ' '
            + evaluationResults[i].decryptRunningTimesPopulationStandardDeviationInMs + '\n';
    }
    file.close();
}

int main() {
    srand(time(0));

    const string key = "8_chars_";
    vector<EvaluationResult> rc4Results, desResults, results;

    rc4Results = evaluate(new ARC4(key), 10, "rc4", PT_TYPE_PERIODIC);
    desResults = evaluate(new DES(key), 10, "des", PT_TYPE_PERIODIC);

    results = rc4Results;
    results.insert(results.end(), desResults.begin(), desResults.end());

    dumpResult(results);

    return 0;
}

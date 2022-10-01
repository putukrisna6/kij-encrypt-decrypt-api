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
            result += to_string((int) str[i]);
            if(i < str.size() - 1) {
                result += ' ';
            }
        }
        return result;
    }
};

// TODO: tidy this code
vector<EvaluationResult> evaluate(Encryption *encryption, int nIter, string cipherName) {
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
    vector<double> encryptRunningTimesInMs, decryptRunningTimesInMs;
    double encryptRunningTimesMeanInMs, encryptRunningTimesPopulationStandardDeviationInMs,
        decryptRunningTimesMeanInMs, decryptRunningTimesPopulationStandardDeviationInMs;
    vector<EvaluationResult> evaluationResults;

    for(int i = 0; i < plainTexts.size(); i++) {
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
    for(int i = 0; i < evaluationResults.size(); i++) {
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
    string key = "8_chars_";

    vector<EvaluationResult> rc4Results, desResults, combinedResults;
    rc4Results = evaluate(new ARC4(key), 10, "rc4");
    desResults = evaluate(new DES(key), 10, "des");

    combinedResults = rc4Results;
    combinedResults.insert(combinedResults.end(), desResults.begin(), desResults.end());
    dumpResult(combinedResults);

    return 0;
}

#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
#include "algorithms/helpers/generator.h"
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

class EvaluationResult {
public:
    string cipherName, plainText, cipherText,
        encryptRunningTimesMeanInMs, encryptRunningTimesPopulationStandardDeviationInMs,
        decryptRunningTimesMeanInMs, decryptRunningTimesPopulationStandardDeviationInMs,
        nIter;

    EvaluationResult(
        string cipherName,
        string plainText,
        string cipherText,
        double encryptRunningTimesMeanInMs,
        double encryptRunningTimesPopulationStandardDeviationInMs,
        double decryptRunningTimesMeanInMs,
        double decryptRunningTimesPopulationStandardDeviationInMs,
        int nIter
    ) {
        this->cipherName = cipherName;
        this->plainText = this->convertStringToInts(plainText);
        this->cipherText = this->convertStringToInts(cipherText);
        this->encryptRunningTimesMeanInMs = to_string(encryptRunningTimesMeanInMs);
        this->encryptRunningTimesPopulationStandardDeviationInMs = to_string(encryptRunningTimesPopulationStandardDeviationInMs);
        this->decryptRunningTimesMeanInMs = to_string(decryptRunningTimesMeanInMs);
        this->decryptRunningTimesPopulationStandardDeviationInMs = to_string(decryptRunningTimesPopulationStandardDeviationInMs);
        this->nIter = to_string(nIter);
    }

private:
    string convertStringToInts(string str) {
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

vector<EvaluationResult> evaluate(Encryption *encryption, vector<string> plainTexts, string cipherName, int nIter) {
    vector<EvaluationResult> evaluationResults;

    for(size_t i = 0; i < plainTexts.size(); i++) {
        printf("Evaluating %s on plain text with length %lu\n", cipherName.c_str(), plainTexts[i].size());

        // Run encrypt once to get the cipherText
        string cipherText = encryption->encrypt(plainTexts[i]);

        vector<double> encryptRunningTimesInMs = measureEncryptRunningTimesInMs(encryption, plainTexts[i], nIter);
        double encryptRunningTimesMeanInMs = calculateMean(encryptRunningTimesInMs);
        double encryptRunningTimesPopulationStandardDeviationInMs = calculatePopulationStandardDeviation(encryptRunningTimesInMs, encryptRunningTimesMeanInMs);

        vector<double> decryptRunningTimesInMs = measureDecryptRunningTimesInMs(encryption, cipherText, nIter);
        double decryptRunningTimesMeanInMs = calculateMean(decryptRunningTimesInMs);
        double decryptRunningTimesPopulationStandardDeviationInMs = calculatePopulationStandardDeviation(decryptRunningTimesInMs, decryptRunningTimesMeanInMs);

        evaluationResults.push_back(EvaluationResult(
            cipherName,
            plainTexts[i],
            cipherText,
            encryptRunningTimesMeanInMs,
            encryptRunningTimesPopulationStandardDeviationInMs,
            decryptRunningTimesMeanInMs,
            decryptRunningTimesPopulationStandardDeviationInMs,
            nIter
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
        file << evaluationResults[i].encryptRunningTimesMeanInMs
            + ' ' + evaluationResults[i].encryptRunningTimesPopulationStandardDeviationInMs
            + ' ' + evaluationResults[i].nIter + '\n';
        file << evaluationResults[i].decryptRunningTimesMeanInMs
            + ' ' + evaluationResults[i].decryptRunningTimesPopulationStandardDeviationInMs
            + ' ' + evaluationResults[i].nIter + '\n';
    }

    file.close();
}

vector<EvaluationResult> combineEvaluationResults(vector<vector<EvaluationResult>> evaluationResults) {
    vector<EvaluationResult> results;
    for(size_t i = 0; i < evaluationResults.size(); i++) {
        results.insert(results.end(), evaluationResults[i].begin(), evaluationResults[i].end());
    }
    return results;
}

int main() {
    srand(time(0));

    const string key = "8_chars_";
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

    vector<vector<EvaluationResult>> evaluationResults{
        evaluate(new ARC4(key), plainTexts, "rc4", 10),
        evaluate(new DES(key), plainTexts, "des", 10)
    };

    vector<EvaluationResult> results = combineEvaluationResults(evaluationResults);


    dumpResult(results);

    return 0;
}

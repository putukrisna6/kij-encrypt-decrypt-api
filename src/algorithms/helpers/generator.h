#ifndef GENERATOR_H
#define GENERATOR_H

#include <bits/stdc++.h>
using namespace std;

class IPlainTextGenerator {
    public:
        static const int MIN_ASCII_CODE = 32, MAX_ASCII_CODE = 127;
        virtual string generate(size_t length) = 0;
};

class RandomPlainTextGenerator: public IPlainTextGenerator {
    public:
        string generate (size_t length) {
            string result;
            for(size_t i = 0; i < length; i++) {
                result += this->generateRandomAsciiCharacter();
            }
            return result;
        }
    private:
        int generateRandomNumber(int startNumber, int endNumber) {
            return startNumber + rand() % (endNumber - startNumber + 1);
        }

        char generateRandomAsciiCharacter() {
            char c = (char) this->generateRandomNumber(IPlainTextGenerator::MIN_ASCII_CODE, IPlainTextGenerator::MAX_ASCII_CODE);
            return c;
        }
};

class PeriodicPlainTextGenerator: public IPlainTextGenerator {
    public:
        string generate (size_t length) {
            string result;
            for(size_t i = 0; i < length; i++) {
                char c = (char) (IPlainTextGenerator::MIN_ASCII_CODE + i % (IPlainTextGenerator::MAX_ASCII_CODE - IPlainTextGenerator::MIN_ASCII_CODE + 1));
                result += c;
            }
            return result;
        }
};

vector<string> generatePlainTexts(IPlainTextGenerator *generator, vector<size_t> lengths) {
    vector<string> plainTexts;
    for(size_t i = 0; i < lengths.size(); i++) {
        plainTexts.push_back(generator->generate(lengths[i]));
    }
    return plainTexts;
}

#endif

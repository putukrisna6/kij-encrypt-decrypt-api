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

int main() {
    srand(time(0));
    cout << generatePeriodicPlainText(100) << endl;
    return 0;
}

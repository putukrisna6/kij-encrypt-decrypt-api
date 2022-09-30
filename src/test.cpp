#include<bits/stdc++.h>
#include "algorithms/encryption.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"
using namespace std;

// Interval: [startNumber, endNumber]
int generateRandomNumber(int startNumber, int endNumber) {
    return startNumber + rand() % (endNumber - startNumber + 1);
}

// Character code interval: [32, 127]
char generateRandomPrintableAsciiCharacter() {
    char c = (char) generateRandomNumber(32, 127);
    return c;
}

string generateRandomPlainText(int length) {
    string result;
    for(int i = 0; i < length; i++) {
        result += generateRandomPrintableAsciiCharacter();
    }
    return result;
}

int main() {
    srand(time(0));
    for(int i = 0; i < 10; i++) {
        cout << generateRandomPlainText(4) << endl;
    }

    return 0;
}

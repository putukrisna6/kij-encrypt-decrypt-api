// Standard library headers
#include <iostream>
#include <string>

// Local library headers
#include "layers/view_layer.h"
#include "layers/data_layer.h"

// Constants
#define FILE_ROOT "../files/"

// Global
DataLayer dataLayer;
ViewLayer viewLayer;

string sendFile() {
    string filePath = viewLayer.sendFileDisplay();
    return dataLayer.readFile(filePath);
}

string sendText() {
    return viewLayer.sendTextDisplay();
}

int main() {
    // 0. user gives data to be sent
    viewLayer.introDisplay();

    int chosenOption = -1;
    while (chosenOption) {
        chosenOption = viewLayer.optionsDisplay();
        string data;

        // possible command layer? lmao
        switch (chosenOption) {
            case 0:
                cout << "Terminating\n";
                return 0;
            case 1:
                data = sendFile();
                break;
            case 2:
                data = sendText();
                break;
            default:
                cout << "Invalid option\n"; 
        }

    // 1. Data is encrypted
        if (data != "") {

        }

    // 2. Data is sent
    }
    return 0;
}
// Standard library headers
#include <iostream>
#include <string>

// Local library headers
#include "layers/view_layer.h"
#include "layers/data_layer.h"

// Constants
#define FILE_ROOT "../files/"

int main() {
    // 0. user gives data to be sent
    ViewLayer viewLayer;
    viewLayer.introDisplay();
    // viewLayer.mainDisplay();

    DataLayer dataLayer;
    string data = dataLayer.readFile("../plain.txt");

    string targetFile = "copy.txt";
    if (data != "") {
        dataLayer.writeFile(FILE_ROOT + targetFile, data);
    }

    // 1. program encrypts file

    // 2. program sends encrypted data

    return 0;
}
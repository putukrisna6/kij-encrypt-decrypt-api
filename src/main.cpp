// Standard library headers
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <chrono>

// Local library headers
#include "layers/view_layer.h"
#include "layers/data_layer.h"

#include "socket/server.h"
#include "socket/client.h"

#include "algorithms/encryption.h"
#include "algorithms/coba_coba.h"
#include "algorithms/rc4.h"
#include "algorithms/des.h"

// Constants
#define FILE_ROOT "../downloads/"

#define ALGO_AES 1
#define ALGO_RC4 2
#define ALGO_DES 3
#define ALGO_COBA 4

#define SEND_FILE 1
#define SEND_TEXT 2

#define BUFFER_SIZE 20480

const string key = "I_AM_A_KEY";
const string desKey = "8_chars_";
const string desIV = "_iv_key_";

// Global
DataLayer dataLayer;
ViewLayer viewLayer;
Encryption *encryption;

void __instantiateEncryption(int chosenAlgo) {
    switch (chosenAlgo) {
        case ALGO_COBA:
            encryption = new CobaCoba();
            break;
        case ALGO_RC4:
            encryption = new ARC4(key);
            break;
        case ALGO_DES:
            encryption = new DES(desKey, desIV);
            break;
        default:
            throw runtime_error("invalid option");
    }
}

string sendFile(string &fileName) {
    string filePath = viewLayer.sendFileDisplay();

    stringstream X(filePath);
    while (getline(X, fileName, '/'));

    return dataLayer.readFile(filePath);
}

string sendText() {
    return viewLayer.sendTextDisplay();
}

void sendDataFlow() {
    int chosenType = viewLayer.messageOptionsDisplay();
    string plainText, \
            fileName;

    switch (chosenType) {
        case SEND_FILE:
            plainText = sendFile(fileName);
            break;
        case SEND_TEXT:
            plainText = sendText();
            break;
        default:
            throw runtime_error("invalid option");
    }

    int chosenAlgo = viewLayer.algoOptionsDisplay();
    __instantiateEncryption(chosenAlgo);

    auto start = chrono::high_resolution_clock::now();
    string cipherText = encryption->encrypt(plainText);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    viewLayer.timeTakenDisplay(duration.count());

    Client client;

    // Send what to receive
    client.clientTransmit(to_string(chosenType));
    client.clientListen();

    // Send file name if needed
    if (chosenType == SEND_FILE) {
        client.clientTransmit(fileName);
        client.clientListen();
    }

    // Send Encryption Type
    client.clientTransmit(to_string(chosenAlgo));
    client.clientListen();
    
    // Split data to 1024 chunks
    vector<string_view> splitData;
    if (!cipherText.empty()) {
        splitData.reserve((cipherText.size() + (BUFFER_SIZE - 1)) / BUFFER_SIZE);

        string_view cipherView = cipherText;
        size_t i = 0;
        
        do {
            splitData.push_back(cipherView.substr(i, BUFFER_SIZE));
            i += splitData.back().size();
        } while (i < cipherText.size());
    }

    // Send how many data parts
    client.clientTransmit(to_string(splitData.size()));
    client.clientListen();

    // Send Encrypted Message
    for (auto d : splitData) {
        string message(d);
        client.clientTransmit(message);
        client.clientListen();
    }

    client.end();
    cin.clear();
}

void receiveDataFlow() {
    Server server;
    string fileName;
    
    // Create downloads directory if not exist
    mkdir("../downloads", 0777);

    // Get what to receive
    server.serverListen();
    int chosenType = stoi(server.getBuffer());
    server.serverTransmit("Received what to receive");

    // Get file name if needed
    if (chosenType == SEND_FILE) {
        server.serverListen();
        fileName = server.getBuffer();
        server.serverTransmit("Received file name");
    }

    // Get Encryption Type
    server.serverListen();
    string algo = server.getBuffer();
    __instantiateEncryption(stoi(algo));
    server.serverTransmit("Received how to decrypt");

    // Get number of data parts
    server.serverListen();
    int parts = stoi(server.getBuffer());
    server.serverTransmit("Received parts");

    // Get Encrypted Data
    string cipherText;
    for (int i = 0; i < parts; i++) {
        server.serverListen();
        cipherText.append(server.getBuffer());
        server.serverTransmit("Received what to decrypt");
    }

    auto start = chrono::high_resolution_clock::now();
    string decrypted = encryption->decrypt(cipherText);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    if (chosenType == SEND_FILE) {
        string filePath = FILE_ROOT;
        filePath.append(fileName);
        dataLayer.writeFile(filePath, decrypted);
        viewLayer.resultsDisplay(fileName, duration.count());
    } else {
        viewLayer.resultsDisplay(cipherText, decrypted, duration.count());
    }

    server.end();
}

int main() {
    // 0. user gives data to be sent
    viewLayer.introDisplay();

    int chosenOption = -1;
    chosenOption = viewLayer.optionsDisplay();

    // possible command layer? lmao
    switch (chosenOption) {
        case 0:
            cout << "Terminating\n";
            return 0;
        case 1:
            sendDataFlow();
            break;
        case 2:
            receiveDataFlow();
            break;
        default:
            cout << "Invalid option\n";
    }

    return 0;
}

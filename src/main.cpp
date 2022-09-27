// Standard library headers
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>

// Local library headers
#include "layers/view_layer.h"
#include "layers/data_layer.h"

#include "socket/server.h"
#include "socket/client.h"

#include "algorithms/encryption.h"
#include "algorithms/coba_coba.h"
#include "algorithms/rc4.h"

// Constants
#define FILE_ROOT "../downloads/"

#define ALGO_AES 1
#define ALGO_RC4 2
#define ALGO_DES 3
#define ALGO_COBA 4

#define SEND_FILE 1
#define SEND_TEXT 2

// Global
DataLayer dataLayer;
ViewLayer viewLayer;
Encryption *encryption;
const string key = "I_AM_A_KEY";

void __instantiateEncryption(int chosenAlgo) {
    switch (chosenAlgo) {
        case ALGO_COBA:
            encryption = new CobaCoba();
            break;
        case ALGO_RC4:
            encryption = new ARC4(key);
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

    string cipherText = encryption->encrypt(plainText);

    Client client;

    // Send what to receive
    client.clientTransmit(to_string(chosenType));
    client.clientListen();
    cout << client.getBuffer() << endl;

    // Send file name if needed
    if (chosenType == SEND_FILE) {
        client.clientTransmit(fileName);
        client.clientListen();
        cout << client.getBuffer() << endl;
    }

    // Send Encryption Type
    client.clientTransmit(to_string(chosenAlgo));
    client.clientListen();
    cout << client.getBuffer() << endl;

    // Send Encrypted Message
    client.clientTransmit(cipherText);
    client.clientListen();
    cout << client.getBuffer() << endl;

    client.clientEnd();
    cin.clear();
}

void receiveDataFlow() {
    Server server;
    string fileName;

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

    server.serverListen();
    string cipherText = server.getBuffer();
    server.serverTransmit("Received what to decrypt");
    cout << cipherText << endl;

    string decrypted = encryption->decrypt(cipherText);
    cout << decrypted << endl;

    if (chosenType == SEND_FILE) {
        string filePath = FILE_ROOT;
        filePath.append(fileName);
        dataLayer.writeFile(filePath, decrypted);
    }

    server.serverEnd();
}

int main() {
    // 0. user gives data to be sent
    viewLayer.introDisplay();

    int chosenOption = -1;
    chosenOption = viewLayer.optionsDisplay();
    string data;

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

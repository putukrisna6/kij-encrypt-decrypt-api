// Standard library headers
#include <iostream>
#include <string>

// Local library headers
#include "layers/view_layer.h"
#include "layers/data_layer.h"

#include "socket/server.h"
#include "socket/client.h"

#include "algorithms/encryption.h"
#include "algorithms/coba_coba.h"

// Constants
#define FILE_ROOT "../files/"

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

void __instantiateEncryption(int chosenAlgo) {
    switch (chosenAlgo) {
        case ALGO_COBA:
            encryption = new CobaCoba();
            break;
        default:
            throw runtime_error("invalid option"); 
    }
}

string sendFile() {
    string filePath = viewLayer.sendFileDisplay();
    return dataLayer.readFile(filePath);
}

string sendText() {
    return viewLayer.sendTextDisplay();
}

void sendDataFlow() {
    int chosenType = viewLayer.messageOptionsDisplay();
    string plainText;

    switch (chosenType) {
        case SEND_FILE:
            plainText = sendFile();
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
    
    // Get Encryption Type
    server.serverListen();
    string algo = server.getBuffer(); 
    __instantiateEncryption(stoi(algo));
    server.serverTransmit(algo);
    
    server.serverListen();
    const char *tmp = server.getBuffer();
    std::string cipherText(tmp);
    cout << cipherText << endl;
    server.serverTransmit("Server Received Message");

    cout << encryption->decrypt(cipherText) << endl;

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
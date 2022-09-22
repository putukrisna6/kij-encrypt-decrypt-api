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
            cout << "Invalid option\n"; 
    }

    int chosenAlgo = viewLayer.algoOptionsDisplay();
    Encryption *encryption;    

    switch (chosenAlgo) {
        case ALGO_COBA:
            encryption = new CobaCoba();
            break;
        default:
            cout << "Invalid option\n"; 
    }

    string cipherText = encryption->encrypt(plainText);

    Client client;
    client.clientTransmit(cipherText);
    client.clientListen();
    cout << client.getBuffer() << endl;

    client.clientEnd();
    cin.clear();
}

void receiveDataFlow() {
    Server server;
    server.serverListen();
    cout << server.getBuffer() << endl;
    server.serverTransmit("Server Received Message");

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
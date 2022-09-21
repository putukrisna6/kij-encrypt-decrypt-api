#include <iostream>

using namespace std;

class ViewLayer 
{
    public:
        void introDisplay() {
            cout << "-----------------------------\n";
            cout << "kij-encypt-decrypt-api\n";
            cout << "Daniel, Krisna, William, Axel\n";
            cout << "-----------------------------\n";
        }

        int optionsDisplay() {
            cout << "\nAvailable options:\n";
            cout << "\t1. Send file\n";
            cout << "\t2. Send text\n";
            cout << "\t0. Exit\n";

            int chosenOption;
            cin >> chosenOption;
            return chosenOption;
        }

        string sendFileDisplay() {
            cout << "\nSend file\n";
            cout << "\tPath to file: ";
            
            string filePath;
            cin >> filePath;
            return filePath;
        }

        string sendTextDisplay() {
            cout << "\nSend text\n";
            cout << "\tText content: ";
            
            string data;
            cin >> data;
            return data;
        }
};
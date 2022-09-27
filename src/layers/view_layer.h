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

        void resultsDisplay(string cipherText, string decrypted) {
            cout << "\nMessage received\n";
            cout << "\tEncrypted: " << cipherText << endl;
            cout << "\tDecrypted: " << decrypted << endl;
        }

        void resultsDisplay(string fileName) {
            cout << "\nFile received\n";
            cout << "\t" << fileName << " saved\n";
        }

        int optionsDisplay() {
            cout << "\nAvailable options:\n";
            cout << "\t1. Send\n";
            cout << "\t2. Receive\n";
            cout << "\t0. Exit\n";

            int chosenOption;
            cin >> chosenOption;
            return chosenOption;
        }

        int algoOptionsDisplay() {
            cout << "\nChoose encryption algorithm:\n";
            cout << "\t1. AES\n";
            cout << "\t2. RC4\n";
            cout << "\t3. DES\n";
            cout << "\t4. CobaCoba\n";

            int chosenOption;
            cin >> chosenOption;
            return chosenOption;
        }

        int messageOptionsDisplay() {
            cout << "\nChoose what to send:\n";
            cout << "\t1. File\n";
            cout << "\t2. Text\n";

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
            getchar();
            getline(cin, data);
            return data;
        }
};
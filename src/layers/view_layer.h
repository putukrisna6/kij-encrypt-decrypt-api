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

        void terminateDisplay() {
            cout << "\nTerminating\n";
            cout << "\tSee you soon :)\n";
        }

        void invalidOptionDisplay(int option) {
            cout << "\nInvalid option chosen\n";
            cout << "\t" << option << " is not a valid option\n";
        }

        void timeTakenDisplay(int time) {
            cout << "\nEncryption succeeded\n";
            cout << "\tTook " << time << " microsecond(s)\n";
        }

        void resultsDisplay(string cipherText, string decrypted, int time) {
            cout << "\nMessage received\n";
            cout << "\tEncrypted: " << cipherText << endl;
            cout << "\tDecrypted: " << decrypted << endl;
            cout << "\tDecryption took " << time << " microsecond(s)\n";
        }

        void resultsDisplay(string fileName, int time) {
            cout << "\nFile received\n";
            cout << "\t" << fileName << " saved\n";
            cout << "\tDecryption took " << time << " microsecond(s)\n";
        }

        int optionsDisplay() {
            cout << "\nAvailable options:\n";
            cout << "\t1. Send\n";
            cout << "\t2. Receive\n";
            cout << "\t0. Exit\n";

            int chosenOption;
            cin >> chosenOption;
                
            if (!cin) {
                throw runtime_error("not a integer");
            }

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

            if (data.size() < 1) {
                throw runtime_error("invalid text length");
            }

            return data;
        }
};
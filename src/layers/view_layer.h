#include <iostream>

using namespace std;

class ViewLayer 
{
    private:
        void __availableOptions() {
            cout << "\nAvailable options:\n";
            cout << "\t1. Send file\n";
            cout << "\t2. Send text\n";
            cout << "\t0. Exit\n";
        } 

    public:
        void introDisplay() {
            cout << "-----------------------------\n";
            cout << "kij-encypt-decrypt-api\n";
            cout << "Daniel, Krisna, William, Axel\n";
            cout << "-----------------------------\n";
        }

        void mainDisplay() {
            while (true) {
                __availableOptions();
                int chosenOption;
                
                try {
                    cin >> chosenOption;

                    if (!cin) {
                        cin.clear();
                        throw std::runtime_error("not an int");
                    }
                } catch (const std::exception &e) {
                    cout << e.what() << endl;
                }

                switch (chosenOption) {
                    case 0:
                        cout << "Terminating...\n";
                        return;
                    default:
                        cout << "Invalid option chosen\n";
                }
            }
        }
};
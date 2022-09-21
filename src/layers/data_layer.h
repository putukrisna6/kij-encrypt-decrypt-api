#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

using namespace std;

class DataLayer 
{
    private:
        bool __isFileExists(string filePath) {
            if (FILE *file = fopen(filePath.c_str(), "r")) {
                fclose(file);
                return true;
            } else {
                return false;
            }   
        }

    public:
        string readFile(string filePath) {
            try {
                ifstream rf(filePath.c_str(), ios::out);

                if (!rf) {
                    throw runtime_error("file not found");
                }

                stringstream buffer;
                buffer << rf.rdbuf();

                string data = buffer.str();

                rf.close();
                return data;
            } catch (const exception& e) {
                cout << e.what() << endl;
                cout << typeid(e).name() << endl;
                return "";
            }
        }

        void writeFile(string filePath, string data) {
            try {
                ofstream wf(filePath.c_str());

                if (!wf) {
                    throw runtime_error("something happened");
                }

                wf << data;
                wf.close();
            } catch (const exception& e) {
                cout << e.what() << endl;
                cout << typeid(e).name() << endl;
            }
        }
};
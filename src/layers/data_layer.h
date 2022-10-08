#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

using namespace std;

class DataLayer
{
    public:
        bool __isFileExists(string filePath) {
            if (FILE *file = fopen(filePath.c_str(), "r")) {
                fclose(file);
                return true;
            } else {
                return false;
            }
        }

        string readFile(string filePath) {
            ifstream rf(filePath.c_str(), ios::out);

            if (!rf) {
                throw runtime_error("file not found");
            }

            stringstream buffer;
            buffer << rf.rdbuf();

            string data = buffer.str();

            rf.close();
            return data;
        }

        void writeFile(string filePath, string data) {
            ofstream wf(filePath.c_str());

            if (!wf) {
                throw runtime_error("unexpected error while writing to file");
            }

            wf << data;
            wf.close();
        }
};

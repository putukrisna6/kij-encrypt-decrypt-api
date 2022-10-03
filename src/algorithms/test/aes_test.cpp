//
// Created by axelb on 24/09/2022.
//

#include <iostream>
#include "aes.h"

using namespace std;

int main() {
    string private_key = "16_long_private_";
    string message = "AES encrypt message";

    AES *aes = new AES(private_key);
    string cipher_message = aes->encrypt(message);
    string decrypted_cipher_message = aes->decrypt(cipher_message);

    cout << endl << endl;
    cout << "private_key: " << private_key << endl;
    cout << "message: " << message << endl;
    cout << "cipher: " << cipher_message << endl;
    cout << "decrypted: " << decrypted_cipher_message << endl;
}

//
// Created by axelb on 24/09/2022.
//

#include <iostream>
#include "aes.h"

using namespace std;

int main() {
    cout << (char) 0x5C << endl;
    cout << (char) 0x94 << endl;
    cout << "\\" << "something" << endl;
    cout << (char) (0x0D) << endl;

    string private_key = "16_long_private1";
    string message = "AES encrypt message";

    AES *aes = new AES(private_key);
    string cipher_message = aes->encrypt(message);
    string decrypted_cipher_message = aes->decrypt(cipher_message);

    cout << endl << endl;
    cout << "private_key: " << private_key << endl;
    cout << "message_size: " << message.length() << endl;
    cout << "message: " << message << endl;
    cout << "cipher_size: " << cipher_message.length() << endl;
    cout << "cipher: " << cipher_message << endl;
    cout << "decrypted_size: " << decrypted_cipher_message.length() << endl;
    cout << "decrypted: " << decrypted_cipher_message << endl;
}

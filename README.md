# Secure Socket Data Transfer using AES, RC4, and DES Encryption

## Introduction

Socket implementation in C++ that allows for encrypted text and file transfers between client and server. The client's data are encrypted using one of the following cryptographic algoritms: [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard), [RC4](https://en.wikipedia.org/wiki/RC4), or [DES](https://en.wikipedia.org/wiki/Data_Encryption_Standard). The server then decrypts the encrypted data which allows secure connection via socket. This project a is part of Information and Network Security (C) 2022 course, Department of Informatics, [ITS](https://www.its.ac.id/).

## Collaborators

* [05111940000075 - Daniel Sugianto](https://github.com/daniel5u)
* [05111940000082 - Putu Krisna Andyartha](https://github.com/putukrisna6)
* [05111940000087 - William Handi Wijaya](https://github.com/wiliamhw)
* [05111940000137 - Axel Briano Suherik](https://github.com/axelbrians)

## Prerequisites

* Linux Environment
* GCC C++17 Compiler
* Jupyter Notebook

## Project Structure

There are 4 main parts of this project, which divides the code into their own domain of concerns.

* Main function
* Socket definition
* Cryptographic algorithms
* Test & evaluation functions

### Main

The main functions acts as driver for the whole project. Thanks to the View and Data layers, the data transfer flow is implemented here.

### Socket

Implementation of socket functionalities are defined within the Socket class and its subclasses: Client and Server. Messages transmitted are divided into chunks of 1024 bytes.

### Cryptopgrahic Algorithms

[LSP](https://en.wikipedia.org/wiki/Liskov_substitution_principle) is applied within the algorithm implementations. Each algorithms are represented as their own classes which are subclasses of the Encryption class. These classes are expected to implement the `encrypt` and `decrypt` methods which can be easily used by the main function using polymorphism.

### Test & Evaluation

This part of the project is designed to analyze the differences between the algorithms.

## Usage

* Compile `main.cpp`

    ```bash
    g++ -std=c++17 main.cpp -o main
    ```

* Run two instances of `main`, one as the sender (client) and as the receiver (server)
* As sender, choose whether to send a text through console input or a file
* Then choose which algorithm to use
* Wait until `File received` or `Message received` appears in the console

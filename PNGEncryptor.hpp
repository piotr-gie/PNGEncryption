#pragma once

#include <array>
#include <string>
#include <vector>

struct IDATChunk
{
    int chunksNumber;
    std::vector<unsigned int> chunksSize;
    std::vector<unsigned int> indice;
    std::vector<std::vector<unsigned int>> data;

    void print();
    void printChunksSize();
};

struct ImageData
{
    long long size;
    unsigned int width;
    unsigned int height;
    int bitDepth;
    int colorType;
    int compressionMethod;
    int filterMethod;
    int interlaceMethod;

    void printData();

    IDATChunk idat;
};



/*
    TO działa w osobnym pliku jak sie skompiluje:

    std::vector<int> message = {'A', 'X'};
    std::vector<int> cipher(message.size());

    for (std::size_t i = 0; i < message.size(); i++) {
        cipher[i] = 1;
        for (int j = 0; j < e; j++) {
            cipher[i] = (cipher[i] * message[i]) % n;
        }
    }


    std::vector<int> result(message.size());
    for (std::size_t i = 0; i < message.size(); i++) {
        result[i] = 1;
        for (int j = 0; j < key; j++) {
            result[i] = (result[i] * cipher[i]) % n;
        }
    }

    std::cout << "Message: ";
    for (auto i : message) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    std::cout << "Cipher: ";
    for (auto i : cipher) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
    std::cout << "Result: ";
    for (auto i : result) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
*/

struct RSAEncryptor
{
    int p = 11;
    int q = 17;

    int n = p * q;
    int e = 7;

    int key = ((p - 1) * (q - 1) + 1) / e;
};

class PNGEncryptor
{
public:
    PNGEncryptor(std::string fileName);

    void parseImage();
    void encryptImage();
    void decryptImage();

    void showImage();
    void printImageData();
    void printIDATChunks();

private:
    ImageData imageData;
    RSAEncryptor encryptor;

    void readIHDR();
    void readIDAT();

    int readNextByte(unsigned int& index);
    unsigned int readNext4Bytes(unsigned int& index);

    void readImageBytes();
    void encryptBytes();
    void decryptBytes();
    void refreshImageCRC();

    unsigned int concatenate4Bytes(
        unsigned char b1,
        unsigned char b2,
        unsigned char b3,
        unsigned char b4
    );

    std::string fileName;
    std::vector<unsigned char> imageBytes;
};

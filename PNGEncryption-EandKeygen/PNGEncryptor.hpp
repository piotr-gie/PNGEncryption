#pragma once

#include <gmpxx.h>

#include <array>
#include <string>
#include <vector>
#include <iostream>

namespace emedia {

template<typename T>
T gcd(T a, T b)
{
    if (a % b == 0) {
        return b;
    }
    else {
        return gcd(b, a % b);
    }
}

} // namespace emedia

struct IDATChunk
{
    int chunksNumber;
    std::vector<unsigned int> chunksSize;
    std::vector<unsigned int> indice;
    std::vector<std::vector<mpz_class>> data;

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

struct RSAEncryptor
{
    RSAEncryptor();
    mpz_class p{"162259276829213363391578010288127"}; // length: 34
    mpz_class q{"618970019642690137449562111"}; // length: 28

    mpz_class phi = (p - 1) * (q - 1);
    mpz_class n = p * q;
    mpz_class e;
    mpz_class key;

    mpz_class generateE();
    mpz_class generateKey();
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

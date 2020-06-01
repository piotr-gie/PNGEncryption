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

class PNGParser
{
public:
    PNGParser(std::string fileName);

    void parseImage();
    void encryptImage();
    void decryptImage();

    void showImage();
    void printImageData();
    void printIDATChunks();

private:
    ImageData imageData;

    void readIHDR();
    void readIDAT();

    int readNextByte(unsigned int& index);
    unsigned int readNext4Bytes(unsigned int& index);

    void readImageBytes();

    unsigned int concatenate4Bytes(
        unsigned char b1,
        unsigned char b2,
        unsigned char b3,
        unsigned char b4
    );

    std::string fileName;
    std::vector<unsigned char> imageBytes;
};

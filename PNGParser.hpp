#pragma once

#include <string>
#include <vector>

struct ImageData {
    unsigned int width;
    unsigned int height;
    unsigned char bitDepth;
    unsigned char colorType;

    void printData();
};

class PNGParser
{
public:
    PNGParser(std::string fileName);

    void parseImage();

    void showImage();
    void printImageBytes();
    void printImageData();

private:
    ImageData imageData;

    void readIHDR();

    unsigned int readNext4Bytes(int& index);

    void readImageBytes();
    void printBytesAsDecNumbers();
    void printBytesAsHexNumbers();
    void printBytesAsChars();

    unsigned int concatenate4Bytes(
        unsigned char b1,
        unsigned char b2,
        unsigned char b3,
        unsigned char b4
    );

    std::string fileName;
    std::vector<unsigned char> imageBytes;
};

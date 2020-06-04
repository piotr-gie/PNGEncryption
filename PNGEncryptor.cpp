#include "PNGEncryptor.hpp"

#include <gmpxx.h>

#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>

void IDATChunk::print()
{
    std::cout << "Number of IDAT chunks: " << chunksNumber << std::endl;
}

void IDATChunk::printChunksSize()
{
    std::cout << "Number of IDAT chunks: " << chunksNumber << std::endl;
    std::cout << "Chunks sizes: { ";
    for (auto size : chunksSize) {
        std::cout << size << " ";
    }
    std::cout << "}" << std::endl;
}

void ImageData::printData()
{
    std::cout << "### Image data ###" << std::endl;
    std::cout << "size: " << size << " bytes" << std::endl;
    std::cout << "width: " << width << " pixels" << std::endl;
    std::cout << "height: " << height << " pixels" << std::endl;
    std::cout << "bit depth: " << bitDepth << std::endl;
    std::cout << "color type: " << colorType << std::endl;
    std::cout << "compression method: " << compressionMethod << std::endl;
    std::cout << "filter method: " << filterMethod << std::endl;
    std::cout << "interlace method: " << interlaceMethod << std::endl;

    idat.print();
}

PNGEncryptor::PNGEncryptor(std::string fileName_) : fileName{fileName_}
{
    readImageBytes();
    parseImage();
}

void PNGEncryptor::readImageBytes()
{
    std::ifstream image{fileName};

    if (not image) {
        std::cerr << "Cannot open file" << std::endl;
        exit(1);
    }

    // imageBytes.clear();
    unsigned char byte;
    while (image >> std::noskipws >> byte) {
        imageBytes.push_back(byte);
    }
}

void PNGEncryptor::refreshImageCRC()
{
    std::string refreshImageCRCCommand;
    refreshImageCRCCommand += "./refresh_crc " + fileName + " tmp.png ";
    refreshImageCRCCommand += "&& mv tmp.png " + fileName;

    system(refreshImageCRCCommand.c_str());
}

void PNGEncryptor::parseImage()
{
    imageData.size = imageBytes.size();

    readIHDR();
    readIDAT();
}

RSAEncryptor::RSAEncryptor():
    e{generateE()},
    key{generateKey()}
{
    mpz_class gcdResult = emedia::gcd(e, phi);
    assert(gcdResult == 1);
    gcdResult = emedia::gcd(mpz_class(key * e), phi);
    assert(gcdResult == 1);
}

void PNGEncryptor::encryptBytes()
{
    std::vector<std::vector<mpz_class>> encryptedData = imageData.idat.data;
    for (std::size_t i = 0; i < imageData.idat.chunksSize.size(); i++) {
        for (std::size_t j = 0; j < imageData.idat.chunksSize[i]; j++) {
            // encryptedData[i][j] = 1;
            // for (int k = 0; k < encryptor.e; k++) {
            //     encryptedData[i][j] = (encryptedData[i][j] * imageData.idat.data[i][j]) % encryptor.n;
            // }
            mpz_t encrypted;
            mpz_init(encrypted);
            mpz_powm(encrypted, imageData.idat.data[i][j].get_mpz_t(), encryptor.e.get_mpz_t(), encryptor.n.get_mpz_t());
            mpz_class ciphers{encrypted};
            encryptedData[i][j] = ciphers;
        }
    }

    imageData.idat.data = encryptedData;

    for (std::size_t i = 0; i < imageData.idat.indice.size(); i++) {
        unsigned int index{imageData.idat.indice[i] + 4};
        for (std::size_t j = 0; j < imageData.idat.chunksSize[i]; j++) {
            imageBytes[index] = encryptedData[i][j].get_ui() % 255;
            index++;
        }
    }

    std::ofstream encryptedImage{fileName};
    for (auto byte: imageBytes) {
        encryptedImage << byte;
    }
}

void PNGEncryptor::encryptImage()
{
    encryptBytes();
    refreshImageCRC();
}

void PNGEncryptor::decryptBytes()
{
    std::vector<std::vector<mpz_class>> decryptedData = imageData.idat.data;
    for (std::size_t i = 0; i < imageData.idat.chunksSize.size(); i++) {
        for (std::size_t j = 0; j < imageData.idat.chunksSize[i]; j++) {
            // decryptedData[i][j] = 1;
            // for (int k = 0; k < encryptor.key; k++) {
            //     decryptedData[i][j] = (decryptedData[i][j] * imageData.idat.data[i][j]) % encryptor.n;
            // }

            mpz_t decrypted;
            mpz_init(decrypted);
            mpz_powm(decrypted,decryptedData[i][j].get_mpz_t(), encryptor.key.get_mpz_t(), encryptor.n.get_mpz_t());
            mpz_class results{decrypted};
            decryptedData[i][j]=results;
        }
    }

    imageData.idat.data = decryptedData;

    for (std::size_t i = 0; i < imageData.idat.indice.size(); i++) {
        unsigned int index{imageData.idat.indice[i] + 4};
        for (std::size_t j = 0; j < imageData.idat.chunksSize[i]; j++) {
            assert(decryptedData[i][j] <= 255);
            imageBytes[index] = decryptedData[i][j].get_ui();
            index++;
        }
    }

    std::ofstream decryptedImage{fileName};
    for (auto byte: imageBytes) {
        decryptedImage << byte;
    }
}

void PNGEncryptor::decryptImage()
{
    decryptBytes();
    refreshImageCRC();
}

void PNGEncryptor::readIHDR()
{
    unsigned int ihdrIndex{0};
    for (std::size_t i = 0; i < imageBytes.size() - 3; i++) {
        if (imageBytes[i] == 'I' and imageBytes[i + 1] == 'H'
        and imageBytes[i + 2] == 'D' and imageBytes[i + 3] == 'R') {
            ihdrIndex = i;
            break;
        }
    }

    unsigned int index{ihdrIndex - 4};
    unsigned int ihdrLength = readNext4Bytes(index);
    if (ihdrLength != 13) {
        std::cerr << "Wrong IHDR chunk size";
        exit(1);
    }

    index += 4;
    imageData.width = readNext4Bytes(index);
    imageData.height = readNext4Bytes(index);
    imageData.bitDepth = readNextByte(index);
    imageData.colorType = readNextByte(index);
    imageData.compressionMethod = readNextByte(index);
    imageData.filterMethod = readNextByte(index);
    imageData.interlaceMethod = readNextByte(index);
}

void PNGEncryptor::readIDAT()
{
    std::vector<unsigned int> idatIndice;
    for (std::size_t i = 0; i < imageBytes.size() - 3; i++) {
        if (imageBytes[i] == 'I' and imageBytes[i + 1] == 'D'
        and imageBytes[i + 2] == 'A' and imageBytes[i + 3] == 'T') {
            idatIndice.push_back(i);
        }
    }

    std::vector<unsigned int> idatLengths;
    for (std::size_t i = 0; i < idatIndice.size(); i++) {
        unsigned int index{idatIndice[i] - 4};
        unsigned int idatLength = readNext4Bytes(index);
        idatLengths.push_back(idatLength);
    }

    std::vector<std::vector<mpz_class>> idatData(idatIndice.size());
    for (std::size_t i = 0; i < idatIndice.size(); i++) {
        unsigned int index{idatIndice[i] + 4};
        for (std::size_t j = 0; j < idatLengths[i]; j++) {
            idatData[i].push_back(imageBytes[index]);
            index++;
        }
    }

    imageData.idat.chunksNumber = idatIndice.size();
    imageData.idat.indice = idatIndice;
    imageData.idat.chunksSize = idatLengths;
    imageData.idat.data = idatData;
}

int PNGEncryptor::readNextByte(unsigned int& index)
{
    assert(index < imageBytes.size());

    return imageBytes[index++];
}

unsigned int PNGEncryptor::readNext4Bytes(unsigned int& index)
{
    assert(index + 3 < imageBytes.size());

    index += 4;
    return concatenate4Bytes(
        imageBytes[index - 4],
        imageBytes[index - 3],
        imageBytes[index - 2],
        imageBytes[index - 1]
    );
}

mpz_class RSAEncryptor::generateE()
{
    return mpz_class{"170141183460469231731687303715884105727"};
}

mpz_class RSAEncryptor::generateKey()
{
    return mpz_class{"3657039001231255831864961573957761009700584565032147969753"};
}

void PNGEncryptor::printImageData()
{
    imageData.printData();
}

unsigned int PNGEncryptor::concatenate4Bytes(
    unsigned char b1,
    unsigned char b2,
    unsigned char b3,
    unsigned char b4
)
{
    return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

void PNGEncryptor::printIDATChunks()
{
    imageData.idat.printChunksSize();
}

void PNGEncryptor::showImage()
{
    std::string showImageCommand;
    showImageCommand += "xdg-open ";
    showImageCommand += fileName;
    showImageCommand += "&";

    system(showImageCommand.c_str());
}

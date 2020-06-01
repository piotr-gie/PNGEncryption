#include "PNGParser.hpp"

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
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

    // int row{1};
    // std::cout << "IDAT data: " << std::endl;
    // for (auto vec : idatData) {
    //     std::cout << row << ": ";
    //     for (auto byte : vec) {
    //         std::cout << byte << " ";
    //     }
    //     std::cout << std::endl;
    // }
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

PNGParser::PNGParser(std::string fileName_) : fileName{fileName_}
{
    readImageBytes();
    parseImage();
}

void PNGParser::readImageBytes()
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

void PNGParser::parseImage()
{
    imageData.size = imageBytes.size();

    readIHDR();
    readIDAT();
}

void PNGParser::encryptImage()
{
    for (std::size_t i = 0; i < imageData.idat.chunksSize.size(); i++) {
        for (std::size_t j = 0; j < imageData.idat.chunksSize[i]; j++) {
            imageData.idat.data[i][j] = 0;
        }
    }

    for (std::size_t i = 0; i < imageData.idat.indice.size(); i++) {
        unsigned int index{imageData.idat.indice[i] + 4};
        for (std::size_t j = 0; j < imageData.idat.chunksSize[i]; j++) {
            imageBytes[index] = imageData.idat.data[i][j];
            index++;
        }
    }

    std::ofstream encryptedImage{fileName};
    // std::ofstream encryptedImage{"encryptedImage.png"};
    for (auto byte: imageBytes) {
        encryptedImage << byte;
    }
}

void PNGParser::decryptImage()
{

}

void PNGParser::readIHDR()
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

void PNGParser::readIDAT()
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

    std::vector<std::vector<unsigned int>> idatData(idatIndice.size());
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

int PNGParser::readNextByte(unsigned int& index)
{
    assert(index < imageBytes.size());

    return imageBytes[index++];
}

unsigned int PNGParser::readNext4Bytes(unsigned int& index)
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

void PNGParser::printImageData()
{
    imageData.printData();
}

unsigned int PNGParser::concatenate4Bytes(
    unsigned char b1,
    unsigned char b2,
    unsigned char b3,
    unsigned char b4
)
{
    return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

void PNGParser::printIDATChunks()
{
    imageData.idat.printChunksSize();
}

void PNGParser::showImage()
{
    std::string showImageCommand;
    showImageCommand += "xdg-open ";
    showImageCommand += fileName;
    showImageCommand += "&";

    system(showImageCommand.c_str());
}

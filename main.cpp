#include "PNGParser.hpp"

#include <iostream>

void showMenu();

int main(int argc, char** argv)
{
    const std::string defaultFileName{"PNGImages2/lena.png"};
    std::string fileName{defaultFileName};
    if (argc > 1) {
        fileName = argv[1];
    }

    PNGParser parser{fileName};

    showMenu();
    int userInput{1};
    while(userInput != 0) {

        std::cout << std::endl;
        std::cout << "Your choice (1 - show menu): ";
        std::cin >> userInput;
        std::cout << std::endl;

        switch(userInput){
            case 0:
                break;

            case 1:
                showMenu();
                break;

            case 2:
                parser.printImageData();
                break;

            case 3:
                parser.showImage();
                break;

            case 4:
                parser.printIDATChunks();
                break;

            case 5:
                parser.encryptImage();
                parser.refreshImageCRC(fileName);
                break;

            default:
                std::cout << "There is no such option" << std::endl;
                break;
        }
    }
}

void showMenu()
{
    std::cout << " - - - MENU - - - " << std::endl;
    std::cout << "1 - show menu" << std::endl;
    std::cout << "2 - show image data" << std::endl;
    std::cout << "3 - show image" << std::endl;
    std::cout << "4 - show IDAT chunks" << std::endl;
    std::cout << "5 - create concatenated image" << std::endl;
    std::cout << std::endl;
    std::cout << "0 - exit" << std::endl;
}

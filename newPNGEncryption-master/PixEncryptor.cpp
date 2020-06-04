#include "PixEncryptor.hpp"
#include "PNGEncryptor.hpp"
#include <Magick++.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace Magick;

PixelEncryptor::PixelEncryptor(string fileName_) : fileName{fileName_}
{
}

mpz_class PixelEncryptor::encryptPixel(mpz_class pixelinput)
{
  RSAEncryptor encryptor;
            mpz_class encryptedData;
            mpz_t encrypted;
            mpz_init(encrypted);
            mpz_powm(encrypted, pixelinput.get_mpz_t(),encryptor.e.get_mpz_t(), encryptor.n.get_mpz_t());
            mpz_class ciphers{encrypted};
            encryptedData= ciphers;

            encryptions.push_back(encryptedData);

            return encryptedData;

            // int encryptedpixelvalue;
            // encryptedpixelvalue= encryptedData.get_ui();
            // return encryptedpixelvalue;
}

mpz_class PixelEncryptor::decryptPixel(mpz_class pixelinput)
{
  RSAEncryptor encryptor;
            mpz_class decryptedData;
            mpz_t decrypted;
            mpz_init(decrypted);
            mpz_powm(decrypted, pixelinput.get_mpz_t(), encryptor.key.get_mpz_t(), encryptor.n.get_mpz_t());
            mpz_class decryptedmessage{decrypted};
            decryptedData=decryptedmessage;
            return decryptedData;

            // unsigned int decryptedpixelvalue;
            // decryptedpixelvalue= decryptedData.get_ui();
            // return decryptedpixelvalue;
}

void PixelEncryptor::encryptPixels()
{
  //PNGEncryptor pngencryptor(fileName);
  Image image;

try {
    // Read a file into image object
    image.read(fileName);

    // Perform FFT and save results to FFTresult vector of two images (Magnitude and Phase)
    //forwardFourierTransformImage(&FFTresult,image);
    //Make magnitude visible by using logarythmic scale
    Pixels view(image);
    Quantum *pixels = view.get(0,0,image.columns(),image.rows());
    for(size_t i=0; i<image.columns()*image.rows();i++)
  {
        pixels[0]=(encryptPixel(pixels[0])).get_ui() % 100000;
        pixels[1]=(encryptPixel(pixels[1])).get_ui() % 100000;
        pixels[2]=(encryptPixel(pixels[2])).get_ui() % 100000;

        pixels += image.channels();
  }
    view.sync();
    image.display();
    image.write( "EncryptedPixelbypixel.png" );
  }
  catch( Exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
    }
}

void PixelEncryptor::decryptPixels()
{
  Image image;

try {
    // Read a file into image object
    image.read("EncryptedPixelbypixel.png" );

    // Perform FFT and save results to FFTresult vector of two images (Magnitude and Phase)
    //forwardFourierTransformImage(&FFTresult,image);
    //Make magnitude visible by using logarythmic scale
    Pixels view(image);
    Quantum *pixels = view.get(0,0,image.columns(),image.rows());
    int index{0};
    for(size_t i=0; i<image.columns()*image.rows();i++)
  {
        pixels[0]=(decryptPixel(encryptions[index++])).get_ui();
        pixels[1]=(decryptPixel(encryptions[index++])).get_ui();
        pixels[2]=(decryptPixel(encryptions[index++])).get_ui();

        pixels += image.channels();
  }
    view.sync();
    image.display();
    image.write( "DecryptedPixelbypixel.png" );
  }
  catch( Exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
    }
}

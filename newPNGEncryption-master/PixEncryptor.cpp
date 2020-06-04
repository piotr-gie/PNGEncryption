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

int encryptPixel(mpz_class pixelinput)
{
  RSAEncryptor encryptor;
            mpz_class encryptedData;
            int encryptedpixelvalue;
            mpz_t encrypted;
            mpz_init(encrypted);
            mpz_powm(encrypted, pixelinput.get_mpz_t(),encryptor.e.get_mpz_t(), encryptor.n.get_mpz_t());
            mpz_class ciphers{encrypted};
            encryptedData= ciphers;
            encryptedpixelvalue= encryptedData.get_ui();
            return encryptedpixelvalue;
}

int decryptPixel(mpz_class pixelinput)
{
  RSAEncryptor encryptor;
            mpz_class decryptedData;
            int decryptedpixelvalue;
            mpz_t decrypted;
            mpz_init(decrypted);
            mpz_powm(decrypted, pixelinput.get_mpz_t(), encryptor.key.get_mpz_t(), encryptor.n.get_mpz_t());
            mpz_class decryptedmessage{decrypted};
            decryptedData=decryptedmessage;
            decryptedpixelvalue= decryptedData.get_ui();
            return decryptedpixelvalue;
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
    PixelPacket *pixels = view.get(0,0,image.columns(),image.rows());
    for(size_t i=0; i<image.columns()*image.rows();i++)
  {
        pixels->red=encryptPixel(pixels->red);
        pixels->green=encryptPixel(pixels->green);
        pixels->blue=encryptPixel(pixels->blue);
        pixels++;
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
    PixelPacket *pixels = view.get(0,0,image.columns(),image.rows());
    for(size_t i=0; i<image.columns()*image.rows();i++)
  {
        pixels->red=decryptPixel(pixels->red);
        pixels->green=decryptPixel(pixels->green);
        pixels->blue=decryptPixel(pixels->blue);
        pixels++;
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

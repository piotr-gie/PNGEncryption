#include <Magick++.h>
#include <iostream>
#include <vector>

class PixelEncryptor
{
  private:
  std::string fileName;

  public:
  PixelEncryptor(std::string fileName_);
  void encryptPixels();
  void decryptPixels();
};

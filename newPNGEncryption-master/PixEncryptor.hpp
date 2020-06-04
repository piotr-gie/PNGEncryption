#include <Magick++.h>
#include <iostream>
#include <vector>

#include <gmpxx.h>

class PixelEncryptor
{
  private:
  std::string fileName;
  std::vector<mpz_class> encryptions;

  public:
  mpz_class encryptPixel(mpz_class pixelinput);
  mpz_class decryptPixel(mpz_class pixelinput);

  PixelEncryptor(std::string fileName_);
  void encryptPixels();
  void decryptPixels();
};

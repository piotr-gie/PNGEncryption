FLAGS= -Wall -pedantic -std=c++17 -iquote inc
Magick++Path= /usr/lib/x86_64-linux-gnu/ImageMagick-6.9.7/bin-q16hdri/Magick++-config
Magick++Flags= ${Magick++Path} --cppflags --cxxflags --ldflags --libs

parser.out: main.o PNGEncryptor.o PixEncryptor.o
	g++ ${FLAGS} main.o PNGEncryptor.o PixEncryptor.o -o PNGEncryptor -lgmpxx -lgmp `${Magick++Flags}`
main.o: main.cpp PNGEncryptor.hpp PixEncryptor.hpp
	g++ -c ${FLAGS}  main.cpp -lgmpxx -lgmp `${Magick++Flags}`
PNGEncryptor.o: PNGEncryptor.hpp PNGEncryptor.cpp
	g++ -c ${FLAGS}  PNGEncryptor.cpp -lgmpxx -lgmp
PixEncryptor.o: PixEncryptor.hpp PixEncryptor.cpp
	g++ -c ${FLAGS}  PixEncryptor.cpp `${Magick++Flags}`

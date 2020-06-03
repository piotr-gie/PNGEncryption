FLAGS= -Wall -pedantic -std=c++17 -iquote inc -lgmp -lgmpxx

parser.out: main.o PNGEncryptor.o
	g++ ${FLAGS} main.o PNGEncryptor.o -o PNGEncryptor

main.o: main.cpp PNGEncryptor.hpp
	g++ -c ${FLAGS}  main.cpp
PNGEncryptor.o: PNGEncryptor.hpp PNGEncryptor.cpp
	g++ -c ${FLAGS}  PNGEncryptor.cpp

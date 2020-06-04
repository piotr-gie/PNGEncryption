FLAGS= -Wall -pedantic -std=c++17 -iquote inc

parser.out: main.o PNGEncryptor.o
	g++ ${FLAGS} main.o PNGEncryptor.o -o PNGEncryptor -lgmpxx -lgmp

main.o: main.cpp PNGEncryptor.hpp
	g++ -c ${FLAGS}  main.cpp -lgmpxx -lgmp
PNGEncryptor.o: PNGEncryptor.hpp PNGEncryptor.cpp
	g++ -c ${FLAGS}  PNGEncryptor.cpp -lgmpxx -lgmp

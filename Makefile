FLAGS= -Wall -pedantic -std=c++17 -iquote inc

parser.out: main.o PNGParser.o
	g++ ${FLAGS}  main.o PNGParser.o -o PNGParser

main.o: main.cpp PNGParser.hpp
	g++ -c ${FLAGS}  main.cpp
PNGParser.o: PNGParser.hpp PNGParser.cpp
	g++ -c ${FLAGS}  PNGParser.cpp

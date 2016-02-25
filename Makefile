main.exe: main.o
	g++ main.o -o main.exe

main.o: main.cpp cbuffer.h
	g++ -c main.cpp -o main.o

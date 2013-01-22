CC=clang
all:
	g++ -g -lpcap main.cpp net.cpp -o network

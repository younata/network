CC=clang
all:
	g++ -g -framework GLUT -framework OpenGL -lpthread -lpcap -lm main.cpp graphics.cpp net.cpp -o network

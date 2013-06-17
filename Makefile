UNAME := $(shell uname)

FLAGS = -lpthread -lpcap -lm -g

ifeq ($(UNAME),Darwin)
	FLAGS += -framework GLUT -framework OpenGL
else
	FLAGS += -lGL -lGLU -lglut
endif

all:
	g++ main.cpp graphics.cpp net.cpp cube.cpp -o network $(FLAGS)

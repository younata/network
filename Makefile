UNAME := $(shell uname)

FLAGS = -lpthread -lpcap -lm

ifeq ($(UNAME),Darwin)
	FLAGS += -framework GLUT -framework OpenGL
else
	FLAGS += -lGL -lGLU -lglut
endif

all:
	g++ $(FLAGS) main.cpp graphics.cpp net.cpp -o network

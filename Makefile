NAME := network

UNAME := $(shell uname)

FLAGS = -lpthread -lpcap -lm

CPPFLAGS := -std=c++11

ifeq ($(UNAME),Darwin)
	CPPFLAGS += -Wno-deprecated-declarations
	FLAGS += -framework GLUT -framework OpenGL
else
	FLAGS += -lGL -lGLU -lglut
endif

SRCS := cube.cpp glObject.cpp graphics.cpp main.cpp net.cpp tail.cpp
OBJS := cube.o glObject.o graphics.o main.o net.o tail.o

all: $(NAME)

$(NAME): $(OBJS)
	$(LINK.cc) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

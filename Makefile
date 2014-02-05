NAME := network

UNAME := $(shell uname)

FLAGS = -lpthread -lpcap -lm

ifeq ($(UNAME),Darwin)
	CPPFLAGS := -Wno-deprecated-declarations
	FLAGS += -framework GLUT -framework OpenGL
else
	FLAGS += -lGL -lGLU -lglut
endif

SRCS := cube.cpp graphics.cpp main.cpp net.cpp tail.cpp
OBJS := cube.o graphics.o main.o net.o tail.o

all: $(NAME)

$(NAME): $(OBJS)
	$(LINK.cc) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	@- $(RM) $(NAME)
	@- $(RM) $(OBJS)

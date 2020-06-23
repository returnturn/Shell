CC = g++
CFLAGS = -std=c++11 -Wall
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

all: clean compile shell379 compress

clean:
		rm -f *.o 

compile:
%.o: %.cpp
		${CC} ${CFLAGS} -c $^ -o $@

shell379: $(OBJECTS)
		$(CC) -o shell379  $(OBJECTS)

compress:
		zip SHELL379.zip *.cpp Makefile
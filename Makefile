CC=g++
CFLAGS=-c -Wall
TARGET=Chess2

SRC=$(shell find $("Chess2.0") -name '*.cpp')
OBJ=$(patsubst %.cpp, %.o, $(SRC))

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lncurses

%.o : %.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf $(shell find $("Chess2.0") -name '*.o') *.o
  
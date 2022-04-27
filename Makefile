SRC = $(shell find src -name *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))
CC = g++
FLAGS = -g -O0 -Wall
TARGET = bin/main.bin
LIBS = -lSDL2 -lstdc++ -lm

all: clean $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CC) -c $(FLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) $(OBJ)

run_main: all
	$(TARGET)
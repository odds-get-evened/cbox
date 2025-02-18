CC = g++
CFLAGS = -Iinclude -std=c++11 -pthread
SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = sensor_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
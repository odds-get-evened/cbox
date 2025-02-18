APP_NAME = data_vecs
VERSION = 0.0.1
# C++ compiler
COMPILER = g++
# include paths
INC_PATHS = -I. -I./inc -IC:/msys64/mingw64/include

CFLAGS = -std=c++11 -Wall

LDFLAGS = -LC:/msys64/mingw64/lib -lboost_system

all: server client

server: server.o
	$(COMPILER) -o server server.o $(LDFLAGS)

client: main.o
	$(COMPILER) -o client main.o $(LDFLAGS)

server.o: temp_server.cpp
	$(COMPILER) $(INC_PATHS) -c temp_server.cpp

main.o: main.cpp
	$(COMPILER) $(INC_PATHS) -c main.cpp

clean:
	rm -f *.o server client
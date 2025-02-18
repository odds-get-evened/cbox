APP_NAME=poco_prac
VERSION=0.0.1

COMPILER=g++

INC_PATHS=-I. -I./inc -IC:/msys64/mingw64/include

CFLAGS=-g -Wall -O0

LDFLAGS=-lstdc++ -lPocoNet -lPocoUtil -lPocoFoundation

SOURCES=./main.cpp ./outils.cpp
CLI_SOURCES=./client.cpp ./outils.cpp

OBJECTS=$(SOURCES:.cpp=.o)
CLI_OBJECTS=$(CLI_SOURCES:.cpp=.o)

TARGET=./dist/$(APP_NAME)_$(VERSION)
CLI_TARGET=./dist/$(APP_NAME)_client_$(VERSION)

all: $(TARGET) $(CLI_TARGET)

# build main
$(TARGET): $(OBJECTS)
	$(COMPILER) $(CFLAGS) $(INC_PATHS) -o $@ $^ $(LDFLAGS)

# build client
$(CLI_TARGET): $(CLI_OBJECTS)
	$(COMPILER) $(CFLAGS) $(INC_PATHS) -o $@ $^ $(LDFLAGS)

start: $(TARGET)
	$(TARGET)

client: $(CLI_TARGET)
	$(CLI_TARGET)

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(CLI_OBJECTS) ./dist/*

# automatic dependency generation
%.o: %.cpp
	$(COMPILER) $(CFLAGS) $(INC_PATHS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d) $(CLI_OBJECTS:.o=.d)
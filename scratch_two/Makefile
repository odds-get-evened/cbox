APP_NAME=scratch_two
VERSION=0.0.1
# C++ compiler
COMPILER=g++
# include paths
INC_PATHS= -I. -I./inc -IC:/msys64/mingw64/include

# the CFLAGS variable sets the compile flags for COMPILER
# -g		compile with debig info.
# -Wall		give verbose compiler warnings
# -O0		do not optimize generated code
# -std=c99	use the C99 standard language definition
CFLAGS=-g -Wall -O0

# LDFLAGS sets flags for linker
# -lm		says to link in libm (the math library)
#####
# requirements:
# -lstdc++	links in the standard c++ library
# -luser32	links in the user32 library
# -lkernel32	links in the kernel32 library
# -lz		links in the zlib library
# -lmlpack links in the mlpack library (machine learning library)
LDFLAGS=-lstdc++ -larmadillo

# in this section, you list files that are part of the project.
# if you add/change names of source files, here is where you
# edit the Makefile (separated by a space)
SOURCES=./main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=./dist/$(APP_NAME)_$(VERSION)

# the first target defined in the Makefile is the one
# used when make is invoked with no argument. given the
# definitions above, this Makefile will build the one 
# named TARGET and assume that it depends on all the named
# OBJECTS.
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INC_PATHS) -o $@ $^ $(LDFLAGS)

run:
	$(TARGET)

# PHONY means not a real TARGET (it doesn't build anything)
# the PHONY target "clean" is used to remove all compiled OBJECTS
.PHONY: clean

clean:
	@rm -f $(OBJECTS) ./dist/*
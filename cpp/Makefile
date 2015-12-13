# Compiler
XX ?= g++
# Project directory
CPP= ${CURDIR}
# Flags
CFLAGS = -std=c++0x -Wall -O2
INCLUDES = -I$(CPP) -I$(CPP)/io -I$(CPP)/img_algs -I$(CPP)/image
# Files
SOURCES = $(CPP)/main.cpp
TARGETS = $(SOURCES:.cpp=.o)

all: $(TARGETS)

%.o: %.cpp
	$(XX) $(CFLAGS) $(INCLUDES) -c $< -o $@

lib:
	$(MAKE) -C io

.PHONY: all clean lib

clean:
	$(RM) *.o *~
	$(MAKE) clean -C io

# Compiler
XX ?= g++
# Project directory
CPP= ${CURDIR}
# Flags
CFLAGS = -Wall -O2
INCLUDES = -I$(CPP) -I$(CPP)/io -I$(CPP)/labs -I$(CPP)/image
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

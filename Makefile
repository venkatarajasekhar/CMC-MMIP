# Compiler
XX ?= g++
# Project directory
PROJECT= ${PWD}
# Flags
CFLAGS = -Wall
INCLUDES = -I$(PROJECT) -I$(PROJECT)/image -I$(PROJECT)/labs
LFLAGS = -L$(PROJECT)/image
# Files
SOURCES = main.cpp image/bmpio.cpp
TARGETS = $(SOURCES:.cpp=.o)
EXECUTABLE = out

all: $(EXECUTABLE)

$(EXECUTABLE): $(TARGETS)
	$(XX) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LFLAGS)

.cpp.o:
	$(XX) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: depend clean

clean:
	$(RM) *.o *~ $(EXECUTABLE)

depend: $(SOURCES)
	makedepend $(INCLUDES) $^



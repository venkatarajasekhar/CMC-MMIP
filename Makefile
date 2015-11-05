# Compiler
XX ?= g++
# Project directory
PROJECT= ${PWD}
# Flags
CFLAGS = -Wall -O2
INCLUDES = -I$(PROJECT) -I$(PROJECT)/image -I$(PROJECT)/labs
LFLAGS = -L$(PROJECT)/image
LIBS = -lbmpio
# Files
SOURCES = main.cpp
TARGETS = $(SOURCES:.cpp=.o)
EXECUTABLE = out

all: $(EXECUTABLE)

$(EXECUTABLE): lib $(TARGETS)
	$(XX) -o $@ $(TARGETS) $(LFLAGS) $(LIBS)

%.o: %.cpp
	$(XX) $(CFLAGS) $(INCLUDES) -c $< -o $@

lib:
	$(MAKE) -C image

.PHONY: all clean lib

clean:
	$(RM) *.o *~ $(EXECUTABLE)
	cd $(PROJECT)/image && $(MAKE) clean

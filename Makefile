# Linker
XX ?= g++
# Project directory
PROJECT= ${CURDIR}
# Flags
LFLAGS = -L$(PROJECT)/cpp/io
LIBS = -lbmpio
# Files
OBJECTS = $(PROJECT)/cpp/main.o
CPP = out

default: $(CPP)

$(CPP): lib $(OBJECTS)
	$(XX) -o $@ $(OBJECTS) $(LFLAGS) $(LIBS)

$(OBJECTS):
	$(MAKE) -C cpp

lib:
	$(MAKE) lib -C cpp

.PHONY: default clean lib

clean:
	$(RM) *.o *~ $(CPP)
	$(MAKE) clean -C cpp

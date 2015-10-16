# Compiler
XX ?= g++
# Lib directory
IMAGE = ${PWD}
# Flags
CFLAGS = -Wall
# Files
SOURCES = bmpio.cpp
TARGETS = $(SOURCES:.cpp=.o)
STATIC = libbmpio.a

all: $(STATIC)

%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@

$(STATIC): $(TARGETS)
	ar rvsc $@ $<
	ranlib $@

.PHONY: all clean

clean:
	$(RM) *.o *~ $(STATIC)

# Compiler
XX ?= g++
# Lib directory
IO = ${CURDIR}
# Flags
CFLAGS = -std=c++0x -Wall -O2
# Files
SOURCES = $(IO)/bmpread.cpp $(IO)/bmpwrite.cpp
TARGETS = $(SOURCES:.cpp=.o)
STATIC = $(IO)/libbmpio.a

default: $(STATIC)

%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@

$(STATIC): $(TARGETS)
	ar rvsc $@ $(TARGETS)
	ranlib $@

.PHONY: default clean

clean:
	$(RM) *.o *~ $(STATIC)

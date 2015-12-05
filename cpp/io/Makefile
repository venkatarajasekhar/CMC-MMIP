# Compiler
XX ?= g++
# Lib directory
IO = ${CURDIR}
# Flags
CFLAGS = -Wall -O2
# Files
SOURCES = $(IO)/bmpio.cpp
TARGETS = $(SOURCES:.cpp=.o)
STATIC = $(IO)/libbmpio.a

default: $(STATIC)

%.o: %.cpp
	$(XX) $(CFLAGS) -c $< -o $@

$(STATIC): $(TARGETS)
	ar rvsc $@ $<
	ranlib $@

.PHONY: default clean

clean:
	$(RM) *.o *~ $(STATIC)

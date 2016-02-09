#
# This is a part of MMIP project
# Copyright (C) 2016  Igor A. Munkin (munkin.igor@gmail.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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

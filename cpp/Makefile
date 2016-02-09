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

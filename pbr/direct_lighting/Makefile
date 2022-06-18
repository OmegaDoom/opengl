# -*- Makefile -*-

CXX := g++
CXXFLAGS := -std=c++17 -O0 -g
LIBS := -lglfw -lGL -lGLEW -lassimp
PRODUCT := main

SRCFILES := $(wildcard src/*.cpp)
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.cpp,%.d,$(SRCFILES))

all: $(PRODUCT)

$(PRODUCT): $(OBJFILES)
	$(CXX) $^ $(LIBS) -o $@

.PHONY: clean

clean:
	find ./src -name '*.o' -delete
	find ./src -name '*.obj' -delete
	rm $(PRODUCT) 
 

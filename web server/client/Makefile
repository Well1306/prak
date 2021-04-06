BUILD ?= debug
#BUILD = release

CXX = g++ -std=gnu++17
CXXFLAGS = -c

ifeq ($(BUILD), debug)
CXXFLAGS += -Wall -O0
endif

ifeq ($(BUILD), release)
CXXFLAGS += -O3
endif

all: clean prog

prog: client.o socket.o
	$(CXX) client.o socket.o -o prog

client.o:
	$(CXX) $(CXXFLAGS) ../client.cpp

socket.o:
	$(CXX) $(CXXFLAGS) ../src/socket.cpp

clean:
	rm -f client.o socket.o prog

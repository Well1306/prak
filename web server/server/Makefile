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

prog: server.o socket.o
	$(CXX) server.o socket.o -o prog

server.o:
	$(CXX) $(CXXFLAGS) ../server.cpp

socket.o:
	$(CXX) $(CXXFLAGS) ../src/socket.cpp

clean:
	rm -f server.o socket.o prog

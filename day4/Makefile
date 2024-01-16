CXX = g++
CXXFLAGS = -std=c++20 -Wall -lfmt 

SRC = main.cpp
EXE = main

all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean: 
	@rm -f $(EXE)    

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra # -DDEBUG

SRC = main.cpp
EXE = main

SRC2 = main2.cpp
EXE2 = main2

all: $(EXE) $(EXE2)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXE2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean: 
	@rm -f $(EXE)    

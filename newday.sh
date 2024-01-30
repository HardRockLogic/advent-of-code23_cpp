#!/bin/bash

if [ -d "$1" ]; then
  echo "directory already exists"
  exit 1;
fi

if [ $# -eq 1 ]; then
  mkdir $1
  touch $1/main.cpp $1/Makefile
  
  # Creating default build
  cat <<'EOF' > $1/Makefile
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

SRC = main.cpp
EXE = main

all: $(EXE)

$(EXE): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean: 
	@rm -f $(EXE)    
EOF

  # Adding lsp support for c++20
  cat <<-EOF > $1/.clangd
CompileFlags:
  Add: [-std=c++20]
EOF

else 
  echo "invalid or no arguments supplied"
fi

echo "$1 created successfully"

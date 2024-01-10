#!/bin/bash

if [ $# -eq 1 ]; then
  mkdir -p $1
  touch $1/main.cpp
else 
  echo "invalid or no arguments supplied"
fi

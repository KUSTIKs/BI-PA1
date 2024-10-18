#!/bin/bash

# Default values as constants
DEFAULT_SOURCE_FILE="main.c"
DEFAULT_OUTPUT_FILE="main.out"

# Set variables to default values
source_file="$DEFAULT_SOURCE_FILE"
output_file="$DEFAULT_OUTPUT_FILE"

# Function to display usage
usage() {
  echo "Usage: $0 [source_file] [output_file]"
  echo "Default source file is $DEFAULT_SOURCE_FILE and default output file is $DEFAULT_OUTPUT_FILE."
  exit 1
}

# Check for command line arguments
if [ "$#" -gt 2 ]; then
  usage
fi

if [ "$#" -ge 1 ]; then
  source_file="$1"
fi

if [ "$#" -eq 2 ]; then
  output_file="$2"
fi

# Check if the source file exists
if [ ! -f "$source_file" ]; then
  echo "Error: Source file '$source_file' does not exist."
  exit 1
fi

# Compile the C program
g++ -Wall -pedantic -o "$output_file" "$source_file" -lm

# Check if the compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful: '$output_file' created."
else
  echo "Compilation failed."
  exit 1
fi

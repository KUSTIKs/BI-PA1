#!/bin/bash

set -e

# Color definitions for error messages
RED='\033[0;31m'
NC='\033[0m' # No Color

# Default values for the test directory and configuration file
DEFAULT_TEST_DIR="_tests_"
DEFAULT_CONFIG_FILE="test-config.toml"

# Initialize variables
test_dir="$DEFAULT_TEST_DIR"
config_file="$DEFAULT_CONFIG_FILE"
silent_mode="false" # Initialize silent mode

# Function to display script usage
usage() {
  echo "Usage: setup-tests.sh [-d test_directory] [-c config_file.toml] [-s]"
  exit 1
}

# Parse command-line options
while getopts "d:c:s" opt; do
  case $opt in
  d) test_dir="$OPTARG" ;;
  c) config_file="$OPTARG" ;;
  s) silent_mode="true" ;; # Set silent mode
  *) usage ;;
  esac
done

# Function to parse the value from a line
parse_value() {
  echo "$1" | sed -E 's/^[^=]+=\s*(.*)$/\1/'
}

# Validate the configuration file
validate_config_file() {
  if [[ ! -f "$config_file" ]]; then
    echo -e "${RED}Error: Configuration file '$config_file' does not exist.${NC}"
    exit 1
  fi

  if [[ ! -r "$config_file" ]]; then
    echo -e "${RED}Error: Configuration file '$config_file' is not readable.${NC}"
    exit 1
  fi
}

# Create input and output test files based on parsed values
create_test_files() {
  if [[ -z "${input+x}" ]]; then
    echo -e "${RED}Error: Missing input value for test index $test_index.${NC}"
    exit 1
  fi

  if [[ -z "${input+x}" ]]; then
    echo -e "${RED}Error: Missing output value for test index $test_index.${NC}"
    exit 1
  fi

  input_filename="${test_dir}/$(printf "%04d" $test_index)_in.txt"
  output_filename="${test_dir}/$(printf "%04d" $test_index)_out.txt"

  printf -- "$input" >"$input_filename"
  printf -- "$output" >"$output_filename"
}

# Ensure the specified test directory exists
mkdir -p "$test_dir"

# Validate the configuration file
validate_config_file

# Initialize variables for processing
test_index=1
is_first_run="true"

# Read the configuration file and create test files
while IFS= read -r line; do
  # Trim leading and trailing whitespaces
  line=$(echo "$line" | xargs)

  # Ignore comment lines and empty lines
  [[ "$line" =~ ^#.*$ || -z "$line" ]] && continue

  # Parse input and output lines
  if [[ $line =~ ^input\ *= ]]; then
    input=$(parse_value "$line")
  elif [[ $line =~ ^output\ *= ]]; then
    output=$(parse_value "$line")
  elif [[ $line =~ ^\[\[test\]\]$ ]]; then
    if [[ $is_first_run == "false" ]]; then
      create_test_files
      test_index=$((test_index + 1))
    fi

    # Reset input and output variables for the next test
    unset input
    unset output
  fi

  is_first_run="false"
done <"$config_file"

# Create test files for the last test entry
create_test_files

# Display success message unless in silent mode
if [[ "$silent_mode" == "false" ]]; then
  echo "Test files have been created in the directory: $test_dir"
fi

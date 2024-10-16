#!/bin/bash

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Messages
PASS_MESSAGE="${GREEN}Pass${NC}"
FAIL_MESSAGE="${RED}Fail${NC}"

# Default values
DEFAULT_TEST_DIR="_tests_"
DEFAULT_PROGRAM_PATH="./main.out"

# Variables
test_dir="$DEFAULT_TEST_DIR"
program_path="$DEFAULT_PROGRAM_PATH"
test_index=""
silent_mode="false"

# Function to display usage
usage() {
  echo "Usage: run-tests.sh [-i test_index] [-t test_directory] [-s] [program_path]"
  exit 1
}

# Parse options
while getopts "i:t:s" opt; do
  case $opt in
  i) test_index="$OPTARG" ;;
  t) test_dir="$OPTARG" ;;
  s) silent_mode="true" ;;
  *) usage ;;
  esac
done

# Shift away the parsed options
shift $((OPTIND - 1))

# Override program path if provided
if [[ $# -gt 0 ]]; then
  program_path="$1"
fi

# Check if the program file exists and is executable
if [[ ! -x "$program_path" ]]; then
  echo -e "${RED}Error: Program file '$program_path' does not exist or is not executable.${NC}"
  exit 1
fi

# Set the test directory path
test_dir="$(pwd)/$test_dir"

# Check if the test directory exists
if [[ ! -d "$test_dir" ]]; then
  echo -e "${RED}Error: Test directory '$test_dir' does not exist.${NC}"
  exit 1
fi

# Initialize counters
pass_count=0
fail_count=0

# Gather test cases
shopt -s nullglob # Enable nullglob to handle no matches gracefully
test_cases=("$test_dir/"*_in.txt)

# Check if any test cases were found
if [[ ${#test_cases[@]} -eq 0 ]]; then
  echo -e "${RED}Error: No test input files found in '$test_dir'.${NC}"
  exit 1
fi

# Run the tests
for input_file in "${test_cases[@]}"; do
  expected_output_file="${input_file/_in.txt/_out.txt}"
  actual_output_file="${input_file/_in.txt/_actual_out.txt}"

  [[ -n "$test_index" && "$input_file" != *"$test_index"_in.txt ]] && continue

  "$program_path" <"$input_file" >"$actual_output_file"

  if [[ $? -ne 0 ]]; then
    echo -e "${RED}Error: Execution failed for '$input_file'.${NC}"
    continue
  fi

  if cmp -s "$expected_output_file" "$actual_output_file"; then
    [[ "$silent_mode" == "false" ]] && echo -e "$(basename "$input_file" _in.txt): ${PASS_MESSAGE}"
    ((pass_count++))
  else
    [[ "$silent_mode" == "false" ]] && echo -e "$(basename "$input_file" _in.txt): ${FAIL_MESSAGE}"
    ((fail_count++))
  fi
done

# Summary
total_count=$((pass_count + fail_count))
summary="Test statistics: "
if ((fail_count > 0)); then
  summary+="${RED}$fail_count failed${NC}, "
fi
if ((pass_count > 0)); then
  summary+="${GREEN}$pass_count passed${NC}, "
fi
summary+="$total_count total"

if [[ "$silent_mode" == "false" ]]; then
  echo -e "\n$summary"
fi

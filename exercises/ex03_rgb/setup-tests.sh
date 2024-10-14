#!/bin/bash

TEST_DIR="_tests"

mkdir -p "$TEST_DIR"

declare -A testFiles

testFiles["0001_in.txt"]="rgb ( 255, 0, 0 )\n"
testFiles["0001_out.txt"]="Zadejte barvu v RGB formatu:\n#FF0000\n"

testFiles["0002_in.txt"]="rgb ( 0, 255, 0 )\n"
testFiles["0002_out.txt"]="Zadejte barvu v RGB formatu:\n#00FF00\n"

testFiles["0003_in.txt"]="rgb ( 0, 0, 255 )\n"
testFiles["0003_out.txt"]="Zadejte barvu v RGB formatu:\n#0000FF\n"

testFiles["0004_in.txt"]="rgb ( 127, 127, 0 )\n"
testFiles["0004_out.txt"]="Zadejte barvu v RGB formatu:\n#7F7F00\n"

testFiles["0005_in.txt"]="hsl ( 0, 127, 0 )\n"
testFiles["0005_out.txt"]="Zadejte barvu v RGB formatu:\nNespravny vstup.\n"

testFiles["0006_in.txt"]="rgb ( 255, 0 )\n"
testFiles["0006_out.txt"]="Zadejte barvu v RGB formatu:\nNespravny vstup.\n"

testFiles["0007_in.txt"]="rgb ( 1000, 127, 0 )\n"
testFiles["0007_out.txt"]="Zadejte barvu v RGB formatu:\nNespravny vstup.\n"

testFiles["0008_in.txt"]="RGB ( 100, 200, 0 )\n"
testFiles["0008_out.txt"]="Zadejte barvu v RGB formatu:\nNespravny vstup.\n"

testFiles["0009_in.txt"]="rgb(1,2,3)\n"
testFiles["0009_out.txt"]="Zadejte barvu v RGB formatu:\n#010203\n"

testFiles["0010_in.txt"]="rgb(1, 2, 3)"
testFiles["0010_out.txt"]="Zadejte barvu v RGB formatu:\n#010203\n"

testFiles["0011_in.txt"]="rgb(1, 2, 3"
testFiles["0011_out.txt"]="Zadejte barvu v RGB formatu:\nNespravny vstup.\n"

for file in "${!testFiles[@]}"; do
  printf -- "${testFiles[$file]}" >"$TEST_DIR/$file"
done

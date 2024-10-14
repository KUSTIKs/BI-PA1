#!/bin/bash

TEST_DIR="_tests"

mkdir -p "$TEST_DIR"

declare -A testFiles

testFiles["0001_in.txt"]="11:45:20,456\n12:07:54,349\n"
testFiles["0001_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nDoba:  0:22:33,893\n"

testFiles["0002_in.txt"]="15:18:34,232\n15:18:34,293\n"
testFiles["0002_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nDoba:  0:00:00,061\n"

testFiles["0003_in.txt"]="12:00:00,000\n12:00:00,000\n"
testFiles["0003_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nDoba:  0:00:00,000\n"

testFiles["0004_in.txt"]="01:01:01,001\n07:07:07,007\n"
testFiles["0004_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nDoba:  6:06:06,006\n"

testFiles["0005_in.txt"]=" 1 : 1 : 1 , 001\n 7 : 7 : 7 , 007\n"
testFiles["0005_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nDoba:  6:06:06,006\n"

testFiles["0006_in.txt"]="1:1:1,001\n17:7:7,007\n"
testFiles["0006_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nDoba: 16:06:06,006\n"

testFiles["0007_in.txt"]="12:30:00,000\n12:00:00,000\n"
testFiles["0007_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nNespravny vstup.\n"

testFiles["0008_in.txt"]="15:30:34,123\n15:60:34,123\n"
testFiles["0008_out.txt"]="Zadejte cas t1:\nZadejte cas t2:\nNespravny vstup.\n"

testFiles["0009_in.txt"]="15:40:21.745\n"
testFiles["0009_out.txt"]="Zadejte cas t1:\nNespravny vstup.\n"

for file in "${!testFiles[@]}"; do
  printf -- "${testFiles[$file]}" >"$TEST_DIR/$file"
done

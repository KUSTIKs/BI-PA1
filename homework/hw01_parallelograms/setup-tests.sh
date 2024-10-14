#!/bin/bash

TEST_DIR="_tests"

mkdir -p "$TEST_DIR"

declare -A testFiles

testFiles["0001_in.txt"]="[0, 0]\n[7, 0]\n[3, 2]\n"
testFiles["0001_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [10,2], rovnobeznik\nB': [-4,2], rovnobeznik\nC': [4,-2], rovnobeznik\n"

testFiles["0002_in.txt"]="[0,0]\n[  5  ,  0     ]\n[3,\n4\n]\n"
testFiles["0002_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [8,4], kosoctverec\nB': [-2,4], rovnobeznik\nC': [2,-4], rovnobeznik\n"

testFiles["0003_in.txt"]="[0,0]\n[-3,4]\n[4,3]\n"
testFiles["0003_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [1,7], ctverec\nB': [7,-1], rovnobeznik\nC': [-7,1], rovnobeznik\n"

testFiles["0004_in.txt"]="[10.5, 10.5]\n[12.5, 10.5]\n[10.5, 15e+0]\n"
testFiles["0004_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [12.5,15], obdelnik\nB': [8.5,15], rovnobeznik\nC': [12.5,6], rovnobeznik\n"

testFiles["0005_in.txt"]="[0, 0]\n[3, 3]\n[10, 10]\n"
testFiles["0005_out.txt"]="Bod A:\nBod B:\nBod C:\nRovnobezniky nelze sestrojit.\n"

testFiles["0006_in.txt"]="[0, 0]\n[2270.242, 0]\n[234.08, 2258.142]\n"
testFiles["0006_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [2504.322,2258.142], kosoctverec\nB': [-2036.162,2258.142], rovnobeznik\nC': [2036.162,-2258.142], rovnobeznik\n"

testFiles["0007_in.txt"]="[740.865, 887.560]\n[340.090, 1241.872]\n[1095.177, 1288.335]\n"
testFiles["0007_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [694.402,1642.647], ctverec\nB': [1495.952,934.023], rovnobeznik\nC': [-14.222,841.097], rovnobeznik\n"

testFiles["0008_in.txt"]="[-306.710, -894.018]\n[6369.015, 66159.129]\n[6016.590, 62619.258]\n"
testFiles["0008_out.txt"]="Bod A:\nBod B:\nBod C:\nRovnobezniky nelze sestrojit.\n"

testFiles["0009_in.txt"]="[2, 5]\n[3, abcd]\n"
testFiles["0009_out.txt"]="Bod A:\nBod B:\nNespravny vstup.\n"

testFiles["0010_in.txt"]="[2, 5]\n[3, 4]\n[7 9]\n"
testFiles["0010_out.txt"]="Bod A:\nBod B:\nBod C:\nNespravny vstup.\n"

testFiles["0011_in.txt"]="[9479.825e-60,77700.064e-60]\n[78276.275e-60,0.544e-60]\n[0.674e-60,0.544e-60]\n"
testFiles["0011_out.txt"]="Bod A:\nBod B:\nBod C:\nA': [6.8797124e-56,-7.7698976e-56], rovnobeznik\nB': [-6.8795776e-56,7.7700064e-56], rovnobeznik\nC': [8.7755426e-56,7.7700064e-56], kosoctverec\n"

testFiles["0012_in.txt"]="[-220.902,353.198]\n[329.984,240.695]\n[329.984,240.695]\n"
testFiles["0012_out.txt"]="Bod A:\nBod B:\nBod C:\nRovnobezniky nelze sestrojit.\n"

for file in "${!testFiles[@]}"; do
  printf -- "${testFiles[$file]}" >"$TEST_DIR/$file"
done

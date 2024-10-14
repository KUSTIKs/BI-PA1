#!/bin/bash

TEST_DIR="_tests"

mkdir -p "$TEST_DIR"

declare -A testFiles

testFiles["0001_in.txt"]="0\n"
testFiles["0001_out.txt"]="ml' nob:\nQapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n"

testFiles["0002_in.txt"]="1\n"
testFiles["0002_out.txt"]="ml' nob:\nQapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n"

testFiles["0003_in.txt"]="2\n"
testFiles["0003_out.txt"]="ml' nob:\nQapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n"

testFiles["0004_in.txt"]="3\n"
testFiles["0004_out.txt"]="ml' nob:\nQapla'\nbISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n"

testFiles["0005_in.txt"]="4\n"
testFiles["0005_out.txt"]="ml' nob:\nQapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n"

testFiles["0006_in.txt"]="5\n"
testFiles["0006_out.txt"]="ml' nob:\nQapla'\nSuvlu'taHvIS yapbe' HoS neH.\n"

testFiles["0007_in.txt"]="6\n"
testFiles["0007_out.txt"]="ml' nob:\nQapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n"

testFiles["0008_in.txt"]="007\n"
testFiles["0008_out.txt"]="ml' nob:\nQapla'\nHeghlu'meH QaQ jajvam.\n"

testFiles["0009_in.txt"]="           8\n"
testFiles["0009_out.txt"]="ml' nob:\nQapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n"

testFiles["0010_in.txt"]="29\n"
testFiles["0010_out.txt"]="ml' nob:\nQih mi' 29\n"

testFiles["0011_in.txt"]="-5\n"
testFiles["0011_out.txt"]="ml' nob:\nQih mi' -5\n"

testFiles["0012_in.txt"]="abc\n"
testFiles["0012_out.txt"]="ml' nob:\nNeh mi'\n"

testFiles["0013_in.txt"]="1.23\n"
testFiles["0013_out.txt"]="ml' nob:\nbIjatlh 'e' yImev\n"

testFiles["0014_in.txt"]=" "
testFiles["0014_out.txt"]="ml' nob:\nNeh mi'\n"

testFiles["0015_in.txt"]="1"
testFiles["0015_out.txt"]="ml' nob:\nQapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n"

testFiles["0016_in.txt"]="\n"
testFiles["0016_out.txt"]="ml' nob:\nNeh mi'\n"

testFiles["0017_in.txt"]=""
testFiles["0017_out.txt"]="ml' nob:\nNeh mi'\n"

testFiles["0018_in.txt"]="1\n2\n"
testFiles["0018_out.txt"]="ml' nob:\nbIjatlh 'e' yImev\n"

testFiles["0019_in.txt"]="1    \n"
testFiles["0019_out.txt"]="ml' nob:\nQapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n"

testFiles["0020_in.txt"]="1    2\n"
testFiles["0020_out.txt"]="ml' nob:\nbIjatlh 'e' yImev\n"

testFiles["0021_in.txt"]="4YHI\n"
testFiles["0021_out.txt"]="ml' nob:\nbIjatlh 'e' yImev\n"

testFiles["0022_in.txt"]="3Z\n"
testFiles["0022_out.txt"]="ml' nob:\nbIjatlh 'e' yImev\n"

testFiles["0023_in.txt"]="1$(printf 'A%.0s' {1..500})"
testFiles["0023_out.txt"]="ml' nob:\nbIjatlh 'e' yImev\n"

testFiles["0022_in.txt"]="01"
testFiles["0022_out.txt"]="ml' nob:\nQapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n"

testFiles["0022_in.txt"]="-01"
testFiles["0022_out.txt"]="ml' nob:\nQih mi' -1\n"

testFiles["0023_in.txt"]="$(printf '0%.0s' {1..500})1"
testFiles["0023_out.txt"]="ml' nob:\nQapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n"

for file in "${!testFiles[@]}"; do
    printf -- "${testFiles[$file]}" >"$TEST_DIR/$file"
done

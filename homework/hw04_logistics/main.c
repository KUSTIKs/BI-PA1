#include <stdbool.h>
#include <stdio.h>

#define MAX_CAPACITY 100000

typedef struct Vehicle {
  int avaliableFrom;
  int avaliableTo;
  int capacity;
  int price;
} Vehicle;

typedef struct ReadVehiclesResult {
  bool isSuccess;
  int vehiclesCount;
} ReadVehiclesResult;

typedef struct Problem {
  int startDay;
  int cargoes;
} Problem;

typedef struct ReadProblemResult {
  bool isSuccess;
  int isEOF;
} ReadProblemResult;

bool isValidVehicle(Vehicle *vehicle) {
  bool isValidFrom = vehicle->avaliableFrom >= 0;
  bool isValidTo = vehicle->avaliableTo >= vehicle->avaliableFrom;
  bool isValidCapacity = vehicle->capacity > 0;
  bool isValidPrice = vehicle->price > 0;

  return isValidFrom && isValidTo && isValidCapacity && isValidPrice;
}

ReadVehiclesResult readVehicles(Vehicle vehicles[]) {
  ReadVehiclesResult result = {false, 0};

  printf("Moznosti dopravy:\n");

  char openingBrace;
  if (scanf(" %c", &openingBrace) != 1 || openingBrace != '{') {
    return result;
  }

  while (result.vehiclesCount < MAX_CAPACITY) {
    Vehicle vehicle;
    char endChar;

    int elementsRead = scanf(
        " [ %d - %d , %d , %d ] %c", &vehicle.avaliableFrom,
        &vehicle.avaliableTo, &vehicle.capacity, &vehicle.price, &endChar
    );

    if (elementsRead != 5 || !isValidVehicle(&vehicle)) return result;

    vehicles[result.vehiclesCount] = vehicle;
    result.vehiclesCount += 1;

    if (endChar == '}') break;
    if (endChar != ',') return result;
  }

  result.isSuccess = true;
  return result;
}

bool isValidProblem(Problem *problem) {
  bool isValidDay = problem->startDay >= 0;
  bool isValidCargoes = problem->cargoes > 0;

  return isValidDay && isValidCargoes;
}

ReadProblemResult readProblem(Problem *problem) {
  ReadProblemResult result = {false, false};

  int elementsRead = scanf(" %d %d", &problem->startDay, &problem->cargoes);

  if (elementsRead == EOF) {
    result.isEOF = true;
    return result;
  }

  if (elementsRead != 2 || !isValidProblem(problem)) return result;

  result.isSuccess = true;
  return result;
}

void solveProblem(Problem *problem, Vehicle vehicles[], int vehiclesCount) {
  int day = problem->startDay;
  int cargoesLeft = problem->cargoes;
  int totalPrice = 0;

  int maxDay = 0;
  for (int i = 0; i < vehiclesCount; i += 1) {
    Vehicle *vehicle = &vehicles[i];

    if (vehicle->avaliableTo <= maxDay) continue;

    maxDay = vehicle->avaliableTo;
  }

  while (day <= maxDay) {
    for (int i = 0; i < vehiclesCount; i += 1) {
      Vehicle *vehicle = &vehicles[i];

      bool isInRange =
          vehicle->avaliableFrom <= day && vehicle->avaliableTo >= day;
      if (!isInRange) continue;

      cargoesLeft -= vehicle->capacity;
      totalPrice += vehicle->price;
    }

    if (cargoesLeft <= 0) break;

    day += 1;
  }

  if (cargoesLeft > 0) {
    printf("Prilis velky naklad, nelze odvezt.\n");
    return;
  }

  printf("Konec: %d, cena: %d\n", day, totalPrice);
}

int main() {
  Vehicle vehicles[MAX_CAPACITY];

  ReadVehiclesResult vehiclesResult = readVehicles(vehicles);
  if (!vehiclesResult.isSuccess) {
    printf("Nespravny vstup.\n");
    return 0;
  }

  printf("Naklad:\n");
  while (true) {
    Problem problem;
    ReadProblemResult problemResult = readProblem(&problem);

    if (problemResult.isEOF) break;
    if (!problemResult.isSuccess) {
      printf("Nespravny vstup.\n");
      return 0;
    }

    solveProblem(&problem, vehicles, vehiclesResult.vehiclesCount);
  }

  return 0;
}

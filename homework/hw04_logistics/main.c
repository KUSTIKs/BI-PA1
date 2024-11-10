#include <stdbool.h>
#include <stdio.h>

#define MAX_CAPACITY 100000
typedef int long long int64;

typedef struct LogisticsInfo {
  int64 capacity;
  int64 price;
} LogisticsInfo;

typedef struct Vehicle {
  int64 avaliableFrom;
  int64 avaliableTo;
  LogisticsInfo logistics;
} Vehicle;

typedef struct LogisticsCalendar {
  int64 avaliableFrom;
  int64 avaliableTo;
  LogisticsInfo dayMap[MAX_CAPACITY];
} LogisticsCalendar;

typedef struct ReadLogisticsResult {
  bool isSuccess;
  int64 vehiclesCount;
  LogisticsCalendar logisticsCalendar;
} ReadLogisticsResult;

typedef struct Problem {
  int64 startDay;
  int64 cargoes;
} Problem;

typedef struct ReadProblemResult {
  bool isSuccess;
  bool isEOF;
} ReadProblemResult;

bool isValidVehicle(Vehicle *vehicle) {
  bool isValidFrom = vehicle->avaliableFrom >= 0;
  bool isValidTo = vehicle->avaliableTo >= vehicle->avaliableFrom;
  bool isValidCapacity = vehicle->logistics.capacity > 0;
  bool isValidPrice = vehicle->logistics.price > 0;

  return isValidFrom && isValidTo && isValidCapacity && isValidPrice;
}

ReadLogisticsResult readLogistics() {
  ReadLogisticsResult result = {false, 0, {0, 0, {}}};
  LogisticsCalendar *calendar = &result.logisticsCalendar;

  printf("Moznosti dopravy:\n");

  char openingBrace;
  if (scanf(" %c", &openingBrace) != 1 || openingBrace != '{') {
    return result;
  }

  while (result.vehiclesCount < MAX_CAPACITY) {
    Vehicle vehicle;
    char endChar;

    int elementsRead = scanf(
        " [ %lld - %lld , %lld , %lld ] %c", &vehicle.avaliableFrom,
        &vehicle.avaliableTo, &vehicle.logistics.capacity,
        &vehicle.logistics.price, &endChar
    );

    if (elementsRead != 5 || !isValidVehicle(&vehicle)) return result;

    if (vehicle.avaliableFrom < calendar->avaliableFrom) {
      calendar->avaliableFrom = vehicle.avaliableFrom;
    }

    if (vehicle.avaliableTo > calendar->avaliableTo) {
      calendar->avaliableTo = vehicle.avaliableTo;
    }

    for (int64 day = vehicle.avaliableFrom; day <= vehicle.avaliableTo;
         day += 1) {
      calendar->dayMap[day].capacity += vehicle.logistics.capacity;
      calendar->dayMap[day].price += vehicle.logistics.price;
    }

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

  int elementsRead = scanf(" %lld %lld", &problem->startDay, &problem->cargoes);

  if (elementsRead == EOF) {
    result.isEOF = true;
    return result;
  }

  if (elementsRead != 2 || !isValidProblem(problem)) return result;

  result.isSuccess = true;
  return result;
}

void solveProblem(Problem *problem, LogisticsCalendar *calendar) {
  int64 minDay = calendar->avaliableFrom;
  int64 maxDay = calendar->avaliableTo;

  while (minDay <= maxDay) {
    int64 midDay = minDay + (maxDay - minDay) / 2;

    LogisticsInfo totalLogistics = {0, 0};
    for (int64 day = problem->startDay; day <= midDay; day += 1) {
      totalLogistics.capacity += calendar->dayMap[day].capacity;
      totalLogistics.price += calendar->dayMap[day].price;
    }

    bool todayMatch = totalLogistics.capacity >= problem->cargoes;
    bool yesterdayMatch =
        midDay > minDay &&
        totalLogistics.capacity - calendar->dayMap[midDay].capacity >=
            problem->cargoes;

    if (todayMatch && !yesterdayMatch) {
      printf("Konec: %lld, cena: %lld\n", midDay, totalLogistics.price);
      return;
    }

    if (totalLogistics.capacity < problem->cargoes) {
      minDay = midDay + 1;
    } else {
      maxDay = midDay - 1;
    }
  }

  printf("Prilis velky naklad, nelze odvezt.\n");
  return;
}

int main() {
  ReadLogisticsResult logisticsResult = readLogistics();
  if (!logisticsResult.isSuccess) {
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

    solveProblem(&problem, &logisticsResult.logisticsCalendar);
  }

  return 0;
}

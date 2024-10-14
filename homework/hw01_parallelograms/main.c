#include <math.h>
#include <stdbool.h>
#include <stdio.h>

struct Point {
  double x;
  double y;
};

bool isNearZero(double n) {
  return fabs(n) < 1e-10;
}

bool isOnTheSameLine(struct Point a, struct Point b, struct Point c) {
  double abSlope = (b.y - a.y) / (b.x - a.x);
  double bcSlope = (c.y - b.y) / (c.x - b.x);

  return isNearZero(abSlope - bcSlope);
}

struct Point findFourth(struct Point a, struct Point b, struct Point c) {
  struct Point baVec = {a.x - b.x, a.y - b.y};
  struct Point bcVec = {c.x - b.x, c.y - b.y};

  struct Point bbPrimeVec = {baVec.x + bcVec.x, baVec.y + bcVec.y};

  struct Point bPrime = {bbPrimeVec.x + b.x, bbPrimeVec.y + b.y};

  return bPrime;
}

const char *classifyParallelogram(
    struct Point a, struct Point b, struct Point c
) {
  struct Point aVec = {b.x - a.x, b.y - a.y};
  struct Point bVec = {c.x - b.x, c.y - b.y};

  double aSize = hypot(aVec.x, aVec.y);
  double bSize = hypot(bVec.x, bVec.y);

  double abAngleCos = (aVec.x * bVec.x + aVec.y * bVec.y) / (aSize * bSize);

  bool isRightAngle = isNearZero(abAngleCos);
  bool isEqualSize = isNearZero(aSize - bSize);

  if (isEqualSize && !isRightAngle) {
    return "kosoctverec";
  }

  if (isEqualSize) {
    return "ctverec";
  }

  if (isRightAngle) {
    return "obdelnik";
  }

  return "rovnobeznik";
}

int readPoint(const char *title, struct Point *point) {
  char endBracket = '\0';

  printf("%s", title);
  int readElements =
      scanf(" [ %lf , %lf %c", &point->x, &point->y, &endBracket);

  if (readElements != 3 || endBracket != ']') {
    return -1;
  }

  return 0;
}

int main(void) {
  struct Point a = {0.0, 0.0}, b = {0.0, 0.0}, c = {0.0, 0.0};

  if (readPoint("Bod A:\n", &a) != 0 || readPoint("Bod B:\n", &b) != 0 ||
      readPoint("Bod C:\n", &c) != 0) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  if (isOnTheSameLine(a, b, c)) {
    printf("Rovnobezniky nelze sestrojit.\n");
    return 0;
  }

  struct Point aPrime = findFourth(c, a, b);
  struct Point bPrime = findFourth(a, b, c);
  struct Point cPrime = findFourth(b, c, a);

  printf(
      "A': [%.8g,%.8g], %s\n", aPrime.x, aPrime.y,
      classifyParallelogram(a, b, aPrime)
  );
  printf(
      "B': [%.8g,%.8g], %s\n", bPrime.x, bPrime.y,
      classifyParallelogram(a, b, c)
  );
  printf(
      "C': [%.8g,%.8g], %s\n", cPrime.x, cPrime.y,
      classifyParallelogram(a, cPrime, b)
  );

  return 0;
}

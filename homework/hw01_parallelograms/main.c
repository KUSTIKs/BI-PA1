#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define EPSILON 1e-10;

struct Point {
  double x;
  double y;
};

bool isEqual(double n1, double n2) {
  double diff = fabs(n1 - n2);
  double max = fmax(fabs(n1), fabs(n2));

  return diff <= max * EPSILON;
}

bool isOnTheSameLine(struct Point a, struct Point b, struct Point c) {
  double abSlope = (b.y - a.y) / (b.x - a.x);
  double bcSlope = (c.y - b.y) / (c.x - b.x);

  return isEqual(abSlope, bcSlope);
}

struct Point findFourth(struct Point a, struct Point b, struct Point c) {
  struct Point bPrime = {a.x - b.x + c.x, a.y - b.y + c.y};

  return bPrime;
}

const char *classifyParallelogram(
    struct Point a, struct Point b, struct Point c
) {
  struct Point aVec = {a.x - b.x, a.y - b.y};
  struct Point cVec = {c.x - b.x, c.y - b.y};

  double aSize = hypot(aVec.x, aVec.y);
  double cSize = hypot(cVec.x, cVec.y);

  bool isRightAngle = isEqual(aVec.x * cVec.x, -aVec.y * cVec.y);
  bool isEqualSize = isEqual(aSize, cSize);

  if (isEqualSize && isRightAngle) {
    return "ctverec";
  }

  if (isEqualSize) {
    return "kosoctverec";
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
  struct Point a = {0, 0}, b = {0, 0}, c = {0, 0};

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

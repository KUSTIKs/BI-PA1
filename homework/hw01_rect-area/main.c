#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define EPSILON 1e-10;

bool isEqual(double n1, double n2) {
  double diff = fabs(n1 - n2);
  double max = fmax(fabs(n1), fabs(n2));

  return diff <= max * EPSILON;
}

bool scanRect(const char* label, double* height, double* width) {
  printf("%s", label);

  int elementsRead = scanf("%lf %lf", height, width);

  return elementsRead == 2 && *height > 0 && *width > 0;
}

int main(void) {
  double h1 = 0, w1 = 0;
  double h2 = 0, w2 = 0;

  if (!scanRect("Obdelnik #1:\n", &h1, &w1) ||
      !scanRect("Obdelnik #2:\n", &h2, &w2)) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  double s1 = h1 * w1;
  double s2 = h2 * w2;

  if (isEqual(s1, s2)) {
    printf("Obdelniky maji stejny obsah.\n");
  } else {
    char smaller = s1 > s2 ? '2' : '1';

    printf("Obdelnik #%c ma mensi obsah.\n", smaller);
  }

  return 0;
}

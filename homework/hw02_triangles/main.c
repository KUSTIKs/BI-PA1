#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
  OUTPUT_EXPANDED = '?',
  OUTPUT_SHORT = '#',
} OutputFormat;

int gcd(int a, int b) {
  if (b == 0) return a;
  return gcd(b, a % b);
}

void printTriples(int lowerLimit, int upperLimit, char format) {
  int a = 0, b = 0, c = 0;
  int count = 0;

  for (int m = 2; m < sqrt(upperLimit); m += 1) {
    for (int n = 1; n < m; n += 1) {
      if ((m - n) % 2 == 0 || gcd(m, n) != 1) continue;

      a = m * m - n * n;
      b = 2 * m * n;
      c = m * m + n * n;

      if (a > b) {
        int temp = a;
        a = b;
        b = temp;
      }

      if (c > upperLimit) continue;

      for (int k = 1; k * c <= upperLimit; k += 1) {
        if (k * c > upperLimit) break;
        if (k * a < lowerLimit || k * b < lowerLimit) {
          continue;
        }

        if (format == OUTPUT_EXPANDED) {
          printf("* %d %d %d\n", k * a, k * b, k * c);
        }

        count += 1;
      }
    }
  }

  printf("Celkem: %d\n", count);
}

int main(void) {
  int lowerLimit = 0, upperLimit = 0;
  char outputFormat = '\0', endChar = '\0';

  printf("Problemy:\n");

  while (true) {
    int elementsRead = scanf(
        " %c < %d ; %d %c", &outputFormat, &lowerLimit, &upperLimit, &endChar
    );
    bool isValidCommand =
        outputFormat == OUTPUT_SHORT || outputFormat == OUTPUT_EXPANDED;

    if (elementsRead == EOF) break;

    if (endChar != '>' || !isValidCommand || elementsRead != 4 ||
        lowerLimit <= 0 || lowerLimit > upperLimit) {
      printf("Nespravny vstup.\n");

      return 0;
    }

    printTriples(lowerLimit, upperLimit, outputFormat);
  }

  return 0;
}

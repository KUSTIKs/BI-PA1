#include <stdbool.h>
#include <stdio.h>

#define TRANSFER_GAP 1

int gcd(int a, int b) {
  int remainder = a % b;

  if (remainder == 0) {
    return b;
  }

  return gcd(b, remainder);
}

bool readTime(const char *title, int *m) {
  int inputM = 0;
  int inputH = 0;

  printf("%s", title);
  int elementsRead = scanf("%d : %d", &inputH, &inputM);

  bool isValidTime = inputM >= 0 && inputM < 60;

  if (elementsRead != 2 || !isValidTime) {
    return false;
  }

  *m = inputH * 60 + inputM;

  return true;
}

int solve(int a, int b, int c) {
  int abGCD = gcd(a, b);

  if (c % abGCD != 0) {
    return -1;
  }

  int r1 = -b % a;
  int r2 = c % a;

  int i = 0;

  for (i = 0; i < a; i += 1) {
    bool isValid = (r1 * i) % a == -r2;

    if (isValid) {
      break;
    }
  }

  int sol = -b * i + c;

  return sol;
}

int main(void) {
  int i1 = 0, i2 = 0, iD = 0;

  if (!readTime("Interval spoje #1:\n", &i1) ||
      !readTime("Interval spoje #2:\n", &i2) ||
      !readTime("Cas od odjezdu spoje #2:\n", &iD)) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  // t1 = i1 * k1 + iD; k1 : int;
  // t2 = i2 * k2; k2 : int;
  // t1 = t2 - TRANSFER_GAP = t;
  // i1 * k1 + iD == i2 * k2 - TRANSFER_GAP;
  // i1 * k1 - i2 * k2 == -iD - TRANSFER_GAP;
  int t = solve(i1, -i2, -iD - TRANSFER_GAP);

  if (t == -1) {
    printf("Spoj #2 nikdy nebude odjizdet 1 minutu po prijezdu spoje #1.\n");

    return 0;
  } else if (t < 0) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  printf("Spoj #2 bude odjizdet 1 minutu po prijezdu spoje #1");

  if (t == 0) {
    printf(" prave nyni.\n");
  } else {
    printf(" za %d:%02d.\n", t / 60, t % 60);
  }

  return 0;
}

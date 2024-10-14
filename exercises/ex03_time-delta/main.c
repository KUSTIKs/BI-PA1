#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int time[4];

bool isValidTime(time t) {
  bool isValidH = t[0] >= 0 && t[0] < 24;
  bool isValidM = t[1] >= 0 && t[1] < 60;
  bool isValidS = t[2] >= 0 && t[2] < 60;
  bool isValidMs = t[3] >= 0 && t[3] < 1000;

  return isValidH && isValidM && isValidS && isValidMs;
}

int timeToMs(time t) {
  int h = t[0];
  int m = t[1] + h * 60;
  int s = t[2] + m * 60;
  int ms = t[3] + s * 1000;

  return ms;
}

void msToTime(int ms, time t) {
  int s = ms / 1000;
  int m = s / 60;
  int h = m / 60;

  t[0] = h;
  t[1] = m % 60;
  t[2] = s % 60;
  t[3] = ms % 1000;

  return;
}

int main(void) {
  time t1 = {0, 0, 0, 0};
  time t2 = {0, 0, 0, 0};
  time dt = {0, 0, 0, 0};

  int elementsRead = 0;

  printf("Zadejte cas t1:\n");
  elementsRead = scanf(" %u : %u : %u , %u", &t1[0], &t1[1], &t1[2], &t1[3]);

  if (elementsRead != 4 || !isValidTime(t1)) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  printf("Zadejte cas t2:\n");
  elementsRead = scanf(" %u : %u : %u , %u", &t2[0], &t2[1], &t2[2], &t2[3]);

  if (elementsRead != 4 || !isValidTime(t2)) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  int t1Ms = timeToMs(t1);
  int t2Ms = timeToMs(t2);

  if (t1Ms > t2Ms) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  int dtMs = t2Ms - t1Ms;

  msToTime(dtMs, dt);

  printf("Doba: %2u:%02u:%02u,%03u\n", dt[0], dt[1], dt[2], dt[3]);

  return 0;
}

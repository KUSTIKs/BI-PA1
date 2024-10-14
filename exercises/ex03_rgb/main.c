#include <stdbool.h>
#include <stdio.h>

#define MAX_COLOR_VALUE 255;

bool isValidRGB(int r, int g, int b) {
  bool isValidR = r >= 0 && r <= MAX_COLOR_VALUE;
  bool isValidG = g >= 0 && g <= MAX_COLOR_VALUE;
  bool isValidB = b >= 0 && b <= MAX_COLOR_VALUE;

  return isValidR && isValidG && isValidB;
}

int main(void) {
  int r = 0;
  int g = 0;
  int b = 0;

  char endBracket = '\0';

  printf("Zadejte barvu v RGB formatu:\n");
  int elementsRead = scanf(" rgb ( %d , %d , %d %c", &r, &g, &b, &endBracket);

  if (elementsRead != 4 || !isValidRGB(r, g, b) || endBracket != ')') {
    printf("Nespravny vstup.\n");

    return 0;
  }

  printf("#%02X%02X%02X\n", r, g, b);

  return 0;
}

#include <stdbool.h>
#include <stdio.h>

#define WHITE_TILE_SYMBOL ' '
#define BLACK_TILE_SYMBOL 'X'
#define CORNER_SYMBOL '+'
#define VERTICAL_BORDER_SYMBOL '|'
#define HORIZONTAL_BORDER_SYMBOL '-'

void printVerticalBorder(int size) {
  printf("%c", CORNER_SYMBOL);
  for (int col = 0; col < size; col += 1) {
    printf("%c", HORIZONTAL_BORDER_SYMBOL);
  }
  printf("%c\n", CORNER_SYMBOL);
}

void printChessboard(int tilesCount, int tileSize) {
  int lineSize = tilesCount * tileSize;

  printVerticalBorder(lineSize);

  for (int row = 0; row < tilesCount * tileSize; row += 1) {
    int tileRow = row / tileSize;

    printf("%c", VERTICAL_BORDER_SYMBOL);
    for (int col = 0; col < tilesCount * tileSize; col += 1) {
      int tileCol = col / tileSize;
      bool isWhiteTile = tileRow % 2 == tileCol % 2;

      printf("%c", isWhiteTile ? WHITE_TILE_SYMBOL : BLACK_TILE_SYMBOL);
    }
    printf("%c\n", VERTICAL_BORDER_SYMBOL);
  }

  printVerticalBorder(lineSize);
}

int main(void) {
  int tilesCount = 0;
  int tileSize = 0;

  printf("Zadejte pocet poli:\n");
  if (scanf("%d", &tilesCount) != 1 || tilesCount <= 0) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  printf("Zadejte velikost pole:\n");
  if (scanf("%d", &tileSize) != 1 || tileSize <= 0) {
    printf("Nespravny vstup.\n");

    return 0;
  }

  printChessboard(tilesCount, tileSize);

  return 0;
}

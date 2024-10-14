#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int quotesCount = 9;
const char *quotes[] = {
    "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' "
    "DoHlu'chugh lujbe'lu'.",
    "bortaS bIr jablu'DI' reH QaQqu' nay'.",
    "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH "
    "SuvwI''e'.",
    "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.",
    "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
    "Suvlu'taHvIS yapbe' HoS neH.",
    "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
    "Heghlu'meH QaQ jajvam.",
    "leghlaHchu'be'chugh mIn lo'laHbe' taj jej.",
};

int main() {
  int quoteIndex;
  char endChar;

  printf("ml' nob:\n");
  int readElementsCount = scanf("%d %c", &quoteIndex, &endChar);

  if (readElementsCount < 1) {
    printf("Neh mi'\n");
    return EXIT_FAILURE;
  }

  if (endChar != '\0') {
    printf("bIjatlh 'e' yImev\n");
    return EXIT_FAILURE;
  }

  bool isInRange = quoteIndex >= 0 && quoteIndex <= quotesCount - 1;

  if (!isInRange) {
    printf("Qih mi' %d\n", quoteIndex);
    return EXIT_FAILURE;
  }

  printf("Qapla'\n%s\n", quotes[quoteIndex]);
  return EXIT_SUCCESS;
}

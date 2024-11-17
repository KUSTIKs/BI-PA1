#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SET_PRICE_SYMBOL '+'
#define GET_PRICE_SYMBOL '?'

typedef struct Array {
  int* array;
  size_t capacity;
  size_t length;
} Array;

typedef enum InstrType {
  SET_PRICE,
  GET_PRICE,
} InstrType;

typedef struct SetPriceInstr {
  int price;
} SetPriceInstr;

typedef struct GetPriceInstr {
  int from;
  int to;
} GetPriceInstr;

typedef union InstrUnion {
  SetPriceInstr setPrice;
  GetPriceInstr getPrice;
} InstrUnion;

typedef struct Instr {
  InstrType type;
  InstrUnion value;
} Instr;

typedef struct ReadInstrResult {
  bool isSuccess;
  bool isEof;
  Instr instr;
} ReadInstrResult;

typedef struct ParseInstrTypeResult {
  bool isSuccess;
  InstrType type;
} ParseInstrTypeResult;

void initArray(Array* array, size_t capacity) {
  array->array = (int*)malloc(capacity * sizeof(int));
  array->capacity = capacity;
  array->length = 0;
}

void pushArray(Array* array, int value) {
  if (array->capacity == array->length) {
    array->capacity += 100;
    array->array = (int*)realloc(array->array, array->capacity * sizeof(int));
  }

  array->array[array->length] = value;
  array->length += 1;
}

void freeArray(Array* array) {
  free(array->array);
  array->length = 0;
}

ParseInstrTypeResult parseInstrType(char symbol) {
  ParseInstrTypeResult result = {false, SET_PRICE};

  switch (symbol) {
    case SET_PRICE_SYMBOL: {
      result.isSuccess = true;
      result.type = SET_PRICE;
      break;
    };
    case GET_PRICE_SYMBOL: {
      result.isSuccess = true;
      result.type = GET_PRICE;
      break;
    };
  }

  return result;
}

ReadInstrResult readSetPriceInstr() {
  ReadInstrResult result = {false, false};
  SetPriceInstr instr;

  int elementsRead = scanf(" %d", &instr.price);

  bool isValidPrice = instr.price >= 0;

  if (elementsRead != 1 || !isValidPrice) {
    return result;
  }

  result.instr.type = SET_PRICE;
  result.instr.value.setPrice = instr;

  result.isSuccess = true;
  return result;
}

ReadInstrResult readGetPriceInstr() {
  ReadInstrResult result = {false, false};
  GetPriceInstr instr;

  int elementsRead = scanf(" %d %d", &instr.from, &instr.to);

  bool isValidFrom = instr.from >= 0;
  bool isValidTo = instr.to >= instr.from;
  bool isValidInstr = isValidFrom && isValidTo;

  if (elementsRead != 2 || !isValidInstr) {
    return result;
  }

  result.instr.type = GET_PRICE;
  result.instr.value.getPrice = instr;

  result.isSuccess = true;
  return result;
}

ReadInstrResult readInstr() {
  ReadInstrResult result = {false, false};

  char instrTypeChar;
  int elementsRead = scanf(" %c", &instrTypeChar);

  if (elementsRead == -1) {
    result.isEof = true;
    return result;
  }

  ParseInstrTypeResult parseInstrTypeResult = parseInstrType(instrTypeChar);
  if (elementsRead != 1 || !parseInstrTypeResult.isSuccess) {
    return result;
  }

  switch (parseInstrTypeResult.type) {
    case SET_PRICE:
      return readSetPriceInstr();
    default:
      return readGetPriceInstr();
  }
}

int main() {
  Array priceRecords;
  initArray(&priceRecords, 100);

  while (true) {
    ReadInstrResult result = readInstr();

    if (result.isEof) {
      break;
    }

    if (!result.isSuccess) {
      printf("Nespravny vstup.\n");
      break;
    }

    if (result.instr.type == SET_PRICE) {
      pushArray(&priceRecords, result.instr.value.setPrice.price);
    }
  }

  printf("priceRecords: [");
  for (size_t i = 0; i < priceRecords.length; i += 1) {
    printf("%d", priceRecords.array[i]);
    if (i != priceRecords.length - 1) {
      printf(", ");
    }
  }
  printf("];\n");

  freeArray(&priceRecords);
}

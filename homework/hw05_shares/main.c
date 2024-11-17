#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SET_PRICE_SYMBOL '+'
#define GET_PRICE_SYMBOL '?'

typedef struct Array {
  int* array;
  int capacity;
  int length;
} Array;

typedef struct Context {
  Array* priceRecords;
} Context;

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

typedef struct ProfitRecord {
  int from;
  int to;
  int value;
} ProfitRecord;

typedef struct ReadInstrResult {
  bool isSuccess;
  bool isEof;
  Instr instr;
} ReadInstrResult;

typedef struct ParseInstrTypeResult {
  bool isSuccess;
  InstrType type;
} ParseInstrTypeResult;

typedef struct HandleInstrResult {
  bool isSuccess;
} HandleInstrResult;

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

void printAbsProfit(const char* label, ProfitRecord* profit) {
  printf("%s: ", label);
  if (profit->value == 0) {
    printf("N/A");
  } else {
    printf("%d (%d - %d)", abs(profit->value), profit->from, profit->to);
  }
  printf("\n");
}

HandleInstrResult handleGetPriceInstr(Context* context, Instr* instr) {
  HandleInstrResult result = {false};
  GetPriceInstr getPriceInstr = instr->value.getPrice;

  if (getPriceInstr.to >= context->priceRecords->length) {
    return result;
  }

  ProfitRecord maxProfit = {0, 0, 0};
  ProfitRecord minProfit = {0, 0, 0};

  for (int i = getPriceInstr.from; i <= getPriceInstr.to; i += 1) {
    int currentPrice = context->priceRecords->array[i];

    for (int j = getPriceInstr.from; j < i; j += 1) {
      int prevPrice = context->priceRecords->array[j];
      int priceDiff = currentPrice - prevPrice;

      if (priceDiff > maxProfit.value) {
        maxProfit.from = j;
        maxProfit.to = i;
        maxProfit.value = priceDiff;
      } else if (priceDiff < minProfit.value) {
        minProfit.from = j;
        minProfit.to = i;
        minProfit.value = priceDiff;
      }
    }
  }

  printAbsProfit("Nejvyssi zisk", &maxProfit);
  printAbsProfit("Nejvyssi ztrata", &minProfit);

  result.isSuccess = true;
  return result;
}

HandleInstrResult handleSetPriceInstr(Context* context, Instr* instr) {
  HandleInstrResult result = {false};

  pushArray(context->priceRecords, instr->value.setPrice.price);

  result.isSuccess = true;
  return result;
}

HandleInstrResult handleInstr(Context* context, Instr* instr) {
  switch (instr->type) {
    case SET_PRICE:
      return handleSetPriceInstr(context, instr);
    default:
      return handleGetPriceInstr(context, instr);
  }
}

int main() {
  Array priceRecords;
  initArray(&priceRecords, 100);

  Context context;
  context.priceRecords = &priceRecords;

  printf("Ceny, hledani:\n");

  while (true) {
    ReadInstrResult readInstrResult = readInstr();

    if (readInstrResult.isEof) {
      break;
    }

    if (!readInstrResult.isSuccess) {
      printf("Nespravny vstup.\n");
      break;
    }

    HandleInstrResult handleInstrResult =
        handleInstr(&context, &readInstrResult.instr);

    if (!handleInstrResult.isSuccess) {
      printf("Nespravny vstup.\n");
      break;
    }
  }

  freeArray(&priceRecords);
}

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

typedef struct MinMaxArrays {
  Array minLeft;
  Array maxRight;
  Array minRight;
  Array maxLeft;
} MinMaxArrays;

typedef struct Context {
  Array* priceRecords;
  MinMaxArrays* minMaxArrays;
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

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

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

MinMaxArrays computeMinMaxArrays(Array* array) {
  MinMaxArrays minMaxArrays;
  initArray(&minMaxArrays.minLeft, array->length);
  initArray(&minMaxArrays.maxRight, array->length);
  initArray(&minMaxArrays.minRight, array->length);
  initArray(&minMaxArrays.maxLeft, array->length);

  int leftMin = array->array[0];
  int rightMax = array->array[array->length - 1];
  int leftMax = array->array[0];
  int rightMin = array->array[array->length - 1];

  for (int i = 0; i < array->length; i += 1) {
    leftMin = min(leftMin, array->array[i]);
    minMaxArrays.minLeft.array[i] = leftMin;

    leftMax = max(leftMax, array->array[i]);
    minMaxArrays.maxLeft.array[i] = leftMax;

    int j = array->length - i - 1;

    rightMax = max(rightMax, array->array[j]);
    minMaxArrays.maxRight.array[j] = rightMax;

    rightMin = min(rightMin, array->array[j]);
    minMaxArrays.minRight.array[j] = rightMin;
  }

  return minMaxArrays;
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

void printProfit(const char* label, ProfitRecord* profit) {
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
  Array priceRecords = *context->priceRecords;

  if (getPriceInstr.to >= priceRecords.length) {
    return result;
  }

  ProfitRecord maxProfit = {0, 0, 0};
  ProfitRecord maxLoss = {0, 0, 0};

  for (int i = getPriceInstr.from; i <= getPriceInstr.to; i += 1) {
    int leftMin = context->minMaxArrays->minLeft.array[i];
    int rightMax = context->minMaxArrays->maxRight.array[i];
    int leftMax = context->minMaxArrays->maxLeft.array[i];
    int rightMin = context->minMaxArrays->minRight.array[i];

    int profit = rightMax - leftMin;
    int loss = rightMin - leftMax;

    if (profit > maxProfit.value) {
      maxProfit.from = leftMin;
      maxProfit.to = rightMax;
      maxProfit.value = profit;
    }
    if (loss < maxLoss.value) {
      maxLoss.from = leftMax;
      maxLoss.to = rightMin;
      maxLoss.value = loss;
    }
  }

  printProfit("Nejvyssi zisk", &maxProfit);
  printProfit("Nejvyssi ztrata", &maxLoss);

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
  Context context = {&priceRecords, NULL};

  initArray(context.priceRecords, 100);

  printf("Ceny, hledani:\n");

  while (true) {
    ReadInstrResult readInstrResult = readInstr();

    if (readInstrResult.isEof) {
      break;
    }

    if (!readInstrResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleaup;
    }

    MinMaxArrays minMaxArrays = computeMinMaxArrays(context.priceRecords);
    context.minMaxArrays = &minMaxArrays;

    HandleInstrResult handleInstrResult =
        handleInstr(&context, &readInstrResult.instr);

    if (!handleInstrResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleaup;
    }
  }

cleaup:
  freeArray(context.priceRecords);
  freeArray(&context.minMaxArrays->minLeft);
  freeArray(&context.minMaxArrays->maxRight);
  freeArray(&context.minMaxArrays->minRight);
  freeArray(&context.minMaxArrays->maxLeft);
}

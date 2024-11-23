#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SET_PRICE_SYMBOL '+'
#define GET_PRICE_SYMBOL '?'

typedef struct Array {
  int* items;
  int capacity;
  int length;
} Array;

typedef struct Context {
  Array priceRecords;
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

typedef struct PriceChange {
  int from;
  int to;
  int value;
} PriceChange;

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

Array initArray(size_t capacity) {
  Array array = {
      .items = (int*)malloc(capacity * sizeof(int)),
      .capacity = (int)capacity,
      .length = 0,
  };

  return array;
}

void resizeArray(Array* array, int capacity) {
  array->capacity = capacity;
  array->items = (int*)realloc(array->items, array->capacity * sizeof(int));
}

void pushArray(Array* array, int value) {
  if (array->length == array->capacity) {
    resizeArray(array, array->capacity * 2);
  }

  array->items[array->length] = value;
  array->length += 1;
}

void freeArray(Array* array) {
  free(array->items);
  array->length = 0;
  array->capacity = 0;
}

ParseInstrTypeResult parseInstrType(char symbol) {
  ParseInstrTypeResult result = {.isSuccess = true};

  switch (symbol) {
    case SET_PRICE_SYMBOL: {
      result.type = SET_PRICE;
      break;
    };
    case GET_PRICE_SYMBOL: {
      result.type = GET_PRICE;
      break;
    };
    default: {
      result.isSuccess = false;
    };
  }

  return result;
}

ReadInstrResult readSetPriceInstr() {
  ReadInstrResult result = {.isSuccess = false};
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
  ReadInstrResult result = {.isSuccess = false};
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
  ReadInstrResult result = {.isSuccess = false};

  char instrTypeChar;
  int elementsRead = scanf(" %c", &instrTypeChar);

  if (elementsRead == EOF) {
    result.isEof = true;
    return result;
  }

  ParseInstrTypeResult parseResult = parseInstrType(instrTypeChar);
  if (elementsRead != 1 || !parseResult.isSuccess) {
    return result;
  }

  switch (parseResult.type) {
    case SET_PRICE:
      return readSetPriceInstr();
    default:
      return readGetPriceInstr();
  }
}

void printPriceChange(const char* label, PriceChange* change) {
  printf("%s: ", label);
  if (change->value > 0) {
    printf("%d (%d - %d)", change->value, change->from, change->to);
  } else {
    printf("N/A");
  }
  printf("\n");
}

HandleInstrResult handleGetPriceInstr(Context* context, Instr* instr) {
  HandleInstrResult result = {.isSuccess = false};

  GetPriceInstr getPriceInstr = instr->value.getPrice;
  Array* priceRecords = &context->priceRecords;
  int* prices = priceRecords->items;

  if (getPriceInstr.to >= priceRecords->length) {
    return result;
  }

  PriceChange maxProfit = {
      .from = getPriceInstr.from,
      .to = getPriceInstr.from,
      .value = 0,
  };
  PriceChange maxLoss = maxProfit;

  int minPriceIndex = maxProfit.from;
  int maxPriceIndex = maxLoss.from;

  for (int i = getPriceInstr.from; i <= getPriceInstr.to; i += 1) {
    int currentPrice = prices[i];
    int minPrice = prices[minPriceIndex];
    int profit = currentPrice - minPrice;

    if (currentPrice < minPrice) {
      minPriceIndex = i;
    }

    if (profit > maxProfit.value) {
      maxProfit.from = minPriceIndex;
      maxProfit.to = i;
      maxProfit.value = profit;
    }

    int maxPrice = prices[maxPriceIndex];
    int loss = maxPrice - currentPrice;

    if (currentPrice > maxPrice) {
      maxPriceIndex = i;
    }

    if (loss > maxLoss.value) {
      maxLoss.from = maxPriceIndex;
      maxLoss.to = i;
      maxLoss.value = loss;
    }
  }

  printPriceChange("Nejvyssi zisk", &maxProfit);
  printPriceChange("Nejvyssi ztrata", &maxLoss);

  result.isSuccess = true;
  return result;
}

HandleInstrResult handleSetPriceInstr(Context* context, Instr* instr) {
  HandleInstrResult result = {.isSuccess = false};

  pushArray(&context->priceRecords, instr->value.setPrice.price);

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
  Context context = {
      .priceRecords = initArray(100),
  };

  printf("Ceny, hledani:\n");

  while (true) {
    ReadInstrResult readInstrResult = readInstr();

    if (readInstrResult.isEof) {
      goto cleaup;
    }

    if (!readInstrResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleaup;
    }

    HandleInstrResult instrResult =
        handleInstr(&context, &readInstrResult.instr);

    if (!instrResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleaup;
    }
  }

cleaup:
  freeArray(&context.priceRecords);
  return 0;
}

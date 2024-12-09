#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  COUNT = '#',
  PRINT = '?',
} ProblemType;

typedef struct {
  char* items;
  int capacity;
  int length;
} CharArray;

typedef struct {
  char* items;
  int length;
  int from;
} CharSlice;

typedef struct {
  CharSlice** items;
  int capacity;
  int length;
} SlicesArray;

typedef struct {
  ProblemType type;
  CharArray value;
} Problem;

typedef struct {
  Problem problem;
  bool isSuccess;
  bool isEof;
} ReadProblemResult;

typedef unsigned long long ull;

char CharSlice_at(CharSlice* slice, int i) {
  int offset = i < 0 ? slice->length + i : i;

  return slice->items[slice->from + offset];
}

void CharSlice_print(CharSlice* slice) {
  for (int i = 0; i < slice->length; i += 1) {
    printf("%c", CharSlice_at(slice, i));
  }
}

char* CharSlice_toBuffer(CharSlice* slice) {
  char* buffer = (char*)malloc(slice->length * sizeof(char) + 1);
  memcpy(buffer, slice->items + slice->from, slice->length);
  buffer[slice->length] = '\0';

  return buffer;
}

ull CharSlice_toUll(CharSlice* slice) {
  char* buffer = CharSlice_toBuffer(slice);

  ull result = strtoull(buffer, NULL, 10);

  free(buffer);

  return result;
}

CharArray CharArray_init() {
  CharArray array = {
      .items = (char*)malloc(100 * sizeof(char)),
      .capacity = 100,
      .length = 0,
  };

  return array;
}

void CharArray_push(CharArray* array, char digit) {
  if (array->length == array->capacity) {
    array->capacity *= 2;
    int newSize = array->capacity * sizeof(char);
    array->items = (char*)realloc(array->items, newSize);
  }

  array->items[array->length] = digit;
  array->length += 1;
}

void CharArray_free(CharArray* array) {
  free(array->items);
  array->items = NULL;
  array->length = 0;
}

SlicesArray SlicesArray_init() {
  SlicesArray array = {
      .items = (CharSlice**)malloc(100 * sizeof(CharSlice*)),
      .capacity = 100,
      .length = 0,
  };

  return array;
}

void SlicesArray_push(SlicesArray* array, CharSlice* slice) {
  if (array->length == array->capacity) {
    array->capacity *= 2;
    int newSize = array->capacity * sizeof(CharSlice*);
    array->items = (CharSlice**)realloc(array->items, newSize);
  }

  array->items[array->length] = slice;
  array->length += 1;
}

void SlicesArray_pop(SlicesArray* array) {
  array->items[array->length] = NULL;
  array->length -= 1;
}

CharSlice* SlicesArray_at(SlicesArray* array, int i) {
  int offset = i < 0 ? array->length + i : i;

  return array->items[offset];
}

void SlicesArray_print(SlicesArray* array) {
  for (int i = 0; i < array->length; i += 1) {
    CharSlice_print(array->items[i]);

    bool isLast = i == array->length - 1;
    if (!isLast) {
      printf(",");
    }
  }
}

void SlicesArray_free(SlicesArray* array) {
  free(array->items);
  array->items = NULL;
  array->length = 0;
}

ReadProblemResult readProblem() {
  ReadProblemResult result = {.isSuccess = false};
  Problem problem;

  char typeChar = getchar();

  switch (typeChar) {
    case COUNT:
      problem.type = COUNT;
      break;
    case PRINT:
      problem.type = PRINT;
      break;
    case EOF:
      result.isEof = true;
      return result;
    default:
      return result;
  }

  if (scanf("%*[ \t]") == EOF) {
    return result;
  }

  problem.value = CharArray_init();

  while (true) {
    char digit = getchar();

    if (digit == '\n' || digit == EOF) break;

    bool isValidDigit = digit >= '0' && digit <= '9';
    if (!isValidDigit) {
      CharArray_free(&problem.value);
      return result;
    }

    CharArray_push(&problem.value, digit);
  }

  if (problem.value.length == 0) {
    return result;
  };

  result.problem = problem;
  result.isSuccess = true;
  return result;
}

int computeChunks(CharSlice* slice, SlicesArray* stack, bool toPrint) {
  if (slice->length == 0) {
    if (toPrint) {
      printf("* ");
      SlicesArray_print(stack);
      printf("\n");
    }

    return 1;
  }

  int count = 0;

  for (int i = 1; i <= slice->length; i += 1) {
    CharSlice currentSlice = {
        .items = slice->items,
        .length = i,
        .from = slice->from,
    };
    CharSlice restSlice = {
        .items = slice->items,
        .length = slice->length - i,
        .from = slice->from + i,
    };

    if (stack->length > 0) {
      CharSlice* lastSlice = SlicesArray_at(stack, -1);
      bool isEven = CharSlice_at(lastSlice, -1) % 2 == 0;

      if (isEven) {
        ull currentNum = CharSlice_toUll(&currentSlice);
        ull lastNum = CharSlice_toUll(lastSlice);

        if (currentNum < lastNum) {
          continue;
        }
      }
    }

    SlicesArray_push(stack, &currentSlice);

    count += computeChunks(&restSlice, stack, toPrint);

    SlicesArray_pop(stack);
  }

  return count;
}

void solveProblem(Problem* problem) {
  SlicesArray slices = SlicesArray_init();
  CharSlice slice = {
      .items = problem->value.items,
      .length = problem->value.length,
      .from = 0,
  };

  int count = computeChunks(&slice, &slices, problem->type == PRINT);

  printf("Celkem: %d\n", count);

  SlicesArray_free(&slices);
}

int main() {
  printf("Posloupnost:\n");

  while (true) {
    ReadProblemResult readProblemResult = readProblem();

    if (readProblemResult.isEof) break;

    if (!readProblemResult.isSuccess) {
      printf("Nespravny vstup.\n");
      break;
    }

    solveProblem(&readProblemResult.problem);

    CharArray_free(&readProblemResult.problem.value);
  }

  return 0;
}

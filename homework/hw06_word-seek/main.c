#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRIKE_OUT_SYMBOL '-'
#define COUNT_SYMBOL '#'
#define DISPLAY_SYMBOL '?'

#define EMPTY_CHAR '.'

#define MAX_LINE_LENGTH 60
#define SHIFTS_COUNT 8

typedef struct Cell {
  char value;
  bool isStrikedOut;
} Cell;

typedef struct CellArray {
  Cell* items;
  int capacity;
  int length;
} CellArray;

typedef struct Field {
  int height;
  int width;
  CellArray cells;
} Field;

typedef struct OptionalCell {
  bool isNone;
  Cell* cell;
} OptionalCell;

typedef struct DynamicString {
  char* buffer;
  int capacity;
  int length;
} DynamicString;

typedef struct ReadFieldResult {
  bool isSuccess;
  Field field;
} ReadFieldResult;

typedef struct HandleCmdResult {
  bool isSuccess;
  bool isEof;
} HandleCmdResult;

typedef struct ReadWordResult {
  bool isSuccess;
  DynamicString word;
} ReadWordResult;

// clang-format off
const int SHIFTS[8][2] = {
    {-1, -1},   {-1,  0},   {-1,  1},
    { 0, -1}, /*{ 0,  0},*/ { 0,  1},
    { 1, -1},   { 1,  0},   { 1,  1},
};
// clang-format on

CellArray CellArray_init(size_t capacity) {
  CellArray array = {
      .items = (Cell*)malloc(capacity * sizeof(Cell)),
      .capacity = (int)capacity,
      .length = 0,
  };

  return array;
}

void CellArray_push(CellArray* array, Cell* item) {
  if (array->capacity == array->length) {
    array->capacity *= 2;
    array->items = (Cell*)realloc(array->items, array->capacity * sizeof(Cell));
  }

  array->items[array->length] = *item;
  array->length += 1;
}

void CellArray_free(CellArray* array) {
  free(array->items);
  array->items = NULL;
  array->length = 0;
  array->capacity = 0;
}

Field Field_init() {
  Field field = {
      .height = 0,
      .width = 0,
      .cells = CellArray_init(100),
  };

  return field;
}

OptionalCell Field_at(Field* field, int row, int column) {
  OptionalCell result = {.isNone = true};

  bool isColumnInRange = column >= 0 && column < field->width;
  bool isRowInRange = row >= 0 && row < field->height;
  if (!isColumnInRange || !isRowInRange) {
    return result;
  }

  int index = row * field->width + column;

  bool isIndexInRange = index < field->cells.length;
  if (!isIndexInRange) {
    return result;
  }

  Cell* item = &field->cells.items[index];

  result.isNone = false;
  result.cell = item;

  return result;
}

void Field_print(Field* field) {
  for (int i = 0; i < field->height; i += 1) {
    for (int j = 0; j < field->width; j += 1) {
      printf("%c", Field_at(field, i, j).cell->value);

      if (j != field->width - 1) {
        printf(" ");
      }
    }
    printf("\n");
  }
}

void Field_free(Field* field) {
  CellArray_free(&field->cells);
  field->height = 0;
  field->width = 0;
}

ReadFieldResult readField() {
  ReadFieldResult result = {.isSuccess = false};
  Field field = Field_init();

  printf("Osmismerka:\n");
  int column = 0;

  while (true) {
    char newChar;
    int elementsRead = scanf("%c", &newChar);

    bool isNewLine = newChar == '\n';
    bool isDot = newChar == EMPTY_CHAR;
    bool isAlpha = newChar >= 'a' && newChar <= 'z';

    bool isValidChar = isNewLine || isDot || isAlpha;
    bool isColumnOverflow = column > field.width;

    if (elementsRead != 1 || !isValidChar || isColumnOverflow) {
      Field_free(&field);
      return result;
    }

    if (isNewLine) {
      if (column == 0) break;

      bool isFullWidth = column == field.width;
      if (!isFullWidth) {
        Field_free(&field);
        return result;
      }

      field.height += 1;
      column = 0;
      continue;
    }

    Cell newCell = {
        .value = newChar,
        .isStrikedOut = false,
    };
    CellArray_push(&field.cells, &newCell);

    if (field.height == 0) {
      field.width += 1;
    }

    column += 1;
  }

  bool isValidSize = field.height >= 1 && field.width >= 1;
  if (!isValidSize) {
    Field_free(&field);
    return result;
  }

  result.isSuccess = true;
  result.field = field;
  return result;
}

void DynamicString_free(DynamicString* string) {
  free(string->buffer);
  string->buffer = NULL;
  string->length = 0;
  string->capacity = 0;
}

DynamicString DynamicString_init(size_t capacity) {
  int actualCapacity = capacity + 1;

  DynamicString string = {
      .buffer = (char*)malloc(actualCapacity * sizeof(char)),
      .capacity = actualCapacity,
      .length = 0,
  };

  string.buffer[0] = '\0';

  return string;
}

void DynamicString_push(DynamicString* string, char newChar) {
  if (string->length == string->capacity - 1) {
    string->capacity *= 2;
    string->buffer = (char*)realloc(string->buffer, string->capacity);
  }

  string->buffer[string->length] = newChar;
  string->buffer[string->length + 1] = '\0';
  string->length += 1;
}

ReadWordResult readWord() {
  ReadWordResult result = {.isSuccess = false};
  DynamicString word = DynamicString_init(128);

  while (true) {
    char newChar;
    int elementsRead = scanf("%c", &newChar);

    bool isNewLine = newChar == '\n';
    bool isEof = elementsRead == EOF;
    bool isAlpha = newChar >= 'a' && newChar <= 'z';

    bool isSpace = newChar == ' ' || newChar == '\t';
    if (word.length == 0 && isSpace) continue;

    bool isValidChar = isNewLine || isEof || isAlpha;
    if (!isValidChar) {
      DynamicString_free(&word);
      return result;
    }

    if (isNewLine || isEof) break;

    DynamicString_push(&word, newChar);
  }

  result.word = word;
  result.isSuccess = true;
  return result;
}

HandleCmdResult handleWordCmd(Field* field, bool strikeOut) {
  HandleCmdResult result = {.isSuccess = false};
  ReadWordResult readWordResult = readWord();

  if (!readWordResult.isSuccess) {
    DynamicString_free(&readWordResult.word);
    return result;
  }

  DynamicString* word = &readWordResult.word;

  bool isValidLength = word->length >= 2;
  if (!isValidLength) {
    DynamicString_free(word);
    return result;
  }

  Cell** matchedCells = (Cell**)malloc(word->length * sizeof(Cell*));
  int occurrencesCount = 0;

  for (int i = 0; i < field->height; i += 1) {
    for (int j = 0; j < field->width; j += 1) {
      for (int s = 0; s < SHIFTS_COUNT; s += 1) {
        const int* shift = SHIFTS[s];

        int lastRow = i + (word->length - 1) * shift[0];
        int lastColumn = j + (word->length - 1) * shift[1];

        bool isRowValid = lastRow >= 0 && lastRow < field->height;
        bool isColumnValid = lastColumn >= 0 && lastColumn < field->width;

        bool isShiftValid = isRowValid && isColumnValid;
        if (!isShiftValid) continue;

        int matchedCellsCount = 0;

        for (int c = 0; c < word->length; c += 1) {
          int row = i + c * shift[0];
          int column = j + c * shift[1];

          OptionalCell item = Field_at(field, row, column);

          if (item.isNone) break;
          if (word->buffer[c] != item.cell->value) break;

          matchedCells[matchedCellsCount] = item.cell;
          matchedCellsCount += 1;
        }

        if (matchedCellsCount == word->length) {
          occurrencesCount += 1;

          if (strikeOut) {
            for (int i = 0; i < matchedCellsCount; i += 1) {
              matchedCells[i]->isStrikedOut = true;
            }
          }
        }
      }
    }
  }

  printf("%s: %dx\n", word->buffer, occurrencesCount);

  DynamicString_free(word);
  free(matchedCells);

  result.isSuccess = true;
  return result;
}

HandleCmdResult handleDisplayCmd(Field* field) {
  HandleCmdResult result = {.isSuccess = false};

  char endChar;
  int elementsRead = scanf("%c", &endChar);
  if (elementsRead != EOF && endChar != '\n') {
    return result;
  }

  int printedCount = 0;

  printf("Tajenka:\n");

  for (int i = 0; i < field->cells.length; i += 1) {
    Cell cell = field->cells.items[i];

    if (cell.value != EMPTY_CHAR && !cell.isStrikedOut) {
      printf("%c", cell.value);
      printedCount += 1;

      bool isFullRow = printedCount % MAX_LINE_LENGTH == 0;
      if (isFullRow) {
        printf("\n");
        continue;
      }
    }

    bool isLast = i == field->cells.length - 1;
    if (isLast && printedCount != 0) {
      printf("\n");
    }
  }

  result.isSuccess = true;
  return result;
}

HandleCmdResult handleCmd(Field* field) {
  HandleCmdResult result = {.isSuccess = false};

  char cmdChar;
  int elementsRead = scanf("%c", &cmdChar);

  if (elementsRead == EOF) {
    result.isEof = true;
    return result;
  }

  if (elementsRead != 1) {
    return result;
  }

  switch (cmdChar) {
    case STRIKE_OUT_SYMBOL:
      return handleWordCmd(field, true);
    case COUNT_SYMBOL:
      return handleWordCmd(field, false);
    case DISPLAY_SYMBOL:
      return handleDisplayCmd(field);
    default:
      return result;
  }
}

int main() {
  ReadFieldResult readFieldResult = readField();

  if (!readFieldResult.isSuccess) {
    printf("Nespravny vstup.\n");
    goto cleanup;
  }

  while (true) {
    HandleCmdResult handleCmdResult = handleCmd(&readFieldResult.field);

    if (handleCmdResult.isEof) {
      goto cleanup;
    }

    if (!handleCmdResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleanup;
    }
  }

cleanup:
  Field_free(&readFieldResult.field);
  return 0;
}

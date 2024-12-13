#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BOARD_SIZE 3
#define MAX_BOARD_SIZE 26

const int DIRECTIONS[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

typedef enum { BLACK, WHITE } Color;

typedef struct {
  int row;
  int column;
} Position;

typedef struct {
  bool isKing;
  Color color;
} Piece;

typedef struct {
  int size;
  Piece* squares[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
} Board;

typedef struct {
  bool isSuccess;
  Board board;
} ReadBoardResult;

typedef struct {
  bool isSuccess;
  bool isEof;
  Piece piece;
  Position position;
} ReadPieceResult;

typedef struct {
  bool isSuccess;
} OperationResult;

typedef struct {
  Position positions[MAX_BOARD_SIZE * MAX_BOARD_SIZE];
  int length;
  int captureCount;
} Move;

typedef struct {
  Move* moves;
  int capacity;
  int length;
} MovesArray;

typedef bool CaptureMap[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

Piece* Piece_malloc() {
  Piece* piece = (Piece*)malloc(sizeof(Piece));
  memset(piece, 0, sizeof(Piece));

  return piece;
}

void Position_print(Position* position) {
  char columnChar = position->column + 'a';
  char row = position->row + 1;

  printf("%c%d", columnChar, row);
}

void Move_pushPosition(Move* move, Position* position) {
  move->positions[move->length] = *position;
  move->length += 1;
}

void Move_print(Move* move) {
  for (int i = 0; i < move->length; i += 1) {
    Position position = move->positions[i];
    Position_print(&position);

    bool isLast = i == move->length - 1;
    if (!isLast) {
      printf(" -> ");
    }
  }

  if (move->captureCount > 0) {
    printf(" +%d", move->captureCount);
  }

  printf("\n");
}

MovesArray MovesArray_init() {
  MovesArray array = {
      .moves = (Move*)malloc(100 * sizeof(Move)),
      .capacity = 100,
      .length = 0,
  };

  return array;
}

void MovesArray_push(MovesArray* array, Move* move) {
  if (array->length == array->capacity) {
    array->capacity *= 2;
    int newSize = array->capacity * sizeof(Move);
    array->moves = (Move*)realloc(array->moves, newSize);
  }

  array->moves[array->length] = *move;
  array->length += 1;
}

void MovesArray_free(MovesArray* array) {
  free(array->moves);
  array->moves = NULL;
  array->length = 0;
}

Board Board_init(int size) {
  Board board = {.size = size};

  return board;
}

void Board_free(Board* board) {
  for (int i = 0; i < board->size; i += 1) {
    for (int j = 0; j < board->size; j += 1) {
      if (board->squares[i][j] == NULL) continue;
      free(board->squares[i][j]);
    }
  }

  board->size = 0;
}

bool Board_isWithinSize(Board* board, Position* position) {
  bool isValidColumn = position->column >= 0 && position->column < board->size;
  bool isValidRow = position->row >= 0 && position->row < board->size;

  return isValidColumn && isValidRow;
}

bool Board_isValidPosition(Board* board, Position* position) {
  bool isBlackSquare = (position->row + position->column) % 2 == 0;

  return Board_isWithinSize(board, position) && isBlackSquare;
}

Piece** Board_at(Board* board, Position* position) {
  int row = position->row;
  int column = position->column;

  return &board->squares[row][column];
}

OperationResult Board_placePiece(
    Board* board, Piece* piece, Position* position
) {
  OperationResult result = {.isSuccess = false};

  if (!Board_isValidPosition(board, position)) {
    return result;
  }

  Piece** p_piece = Board_at(board, position);

  if (*p_piece != NULL) {
    return result;
  }

  *p_piece = Piece_malloc();
  memcpy(*p_piece, piece, sizeof(Piece));

  result.isSuccess = true;
  return result;
}

void Board_findPieceMoves(
    Board* board, CaptureMap captureMap, Piece* piece, const int* lastDirection,
    Position* position, Move move, MovesArray* movesArray
) {
  int directionsCount = piece->isKing ? 4 : 2;

  for (int i = 0; i < directionsCount; i += 1) {
    const int* direction = DIRECTIONS[i];
    Position newPosition = {
        .row = position->row + direction[0],
        .column = position->column + direction[1],
    };
    Piece* targetPiece = NULL;

    if (lastDirection != NULL && direction[0] == -lastDirection[0] &&
        direction[1] == -lastDirection[1]) {
      continue;
    }

    while (true) {
      if (!Board_isWithinSize(board, &newPosition)) break;

      targetPiece = *Board_at(board, &newPosition);

      bool isCaptured = captureMap[newPosition.row][newPosition.column];
      bool isPiece = targetPiece != NULL && !isCaptured;

      if (isPiece) break;

      if (move.captureCount == 0) {
        Move newMove = move;
        Move_pushPosition(&newMove, &newPosition);
        MovesArray_push(movesArray, &newMove);
      }

      if (!piece->isKing) break;

      newPosition.row += direction[0];
      newPosition.column += direction[1];
    }

    if (!Board_isWithinSize(board, &newPosition)) continue;
    targetPiece = *Board_at(board, &newPosition);

    bool* p_isCaptured = &captureMap[newPosition.row][newPosition.column];

    bool isCaptruted = *p_isCaptured;
    bool isTargetBlack = targetPiece != NULL && targetPiece->color == BLACK;

    if (!isTargetBlack || isCaptruted) continue;

    *p_isCaptured = true;

    newPosition.row += direction[0];
    newPosition.column += direction[1];

    while (true) {
      if (!Board_isWithinSize(board, &newPosition)) break;

      targetPiece = *Board_at(board, &newPosition);

      bool isCaptured = captureMap[newPosition.row][newPosition.column];
      bool isPiece = targetPiece != NULL && !isCaptured;
      if (isPiece) break;

      Move newMove = move;
      Move_pushPosition(&newMove, &newPosition);
      newMove.captureCount += 1;

      MovesArray_push(movesArray, &newMove);

      Board_findPieceMoves(
          board, captureMap, piece, direction, &newPosition, newMove, movesArray
      );

      if (!piece->isKing) break;

      newPosition.row += direction[0];
      newPosition.column += direction[1];
    }

    *p_isCaptured = false;
  }
}

void Board_findMoves(Board* board) {
  MovesArray movesArray = MovesArray_init();

  for (int i = 0; i < board->size; i += 1) {
    for (int j = 0; j < board->size; j += 1) {
      Position position = {.row = i, .column = j};
      Piece* piece = *Board_at(board, &position);
      Move move = {.length = 1, .captureCount = 0};
      CaptureMap captureMap = {false};

      move.positions[0] = position;
      captureMap[i][j] = true;

      if (piece == NULL || piece->color != WHITE) continue;

      Board_findPieceMoves(
          board, captureMap, piece, NULL, &position, move, &movesArray
      );
    }
  }

  for (int i = 0; i < movesArray.length; i += 1) {
    Move_print(&movesArray.moves[i]);
  }

  printf("Celkem ruznych tahu: %d\n", movesArray.length);

  MovesArray_free(&movesArray);
}

ReadBoardResult readBoard() {
  ReadBoardResult result = {.isSuccess = false};

  printf("Velikost hraci plochy:\n");

  int boardSize;
  int elementsRead = scanf("%d", &boardSize);

  bool isValidSize = boardSize >= MIN_BOARD_SIZE && boardSize <= MAX_BOARD_SIZE;
  if (elementsRead != 1 || !isValidSize) {
    return result;
  }

  char endChar = getchar();
  if (endChar != '\n' && endChar != EOF) {
    return result;
  }

  Board board = Board_init(boardSize);

  result.board = board;
  result.isSuccess = true;

  return result;
}

ReadPieceResult readPiece() {
  ReadPieceResult result = {.isSuccess = false};
  Piece piece;

  char pieceChar = getchar();
  if (pieceChar == EOF) {
    result.isEof = true;
    return result;
  }

  char columnChar;
  int rawRow;
  int elementsRead = scanf(" %c%d", &columnChar, &rawRow);

  int column = columnChar - 'a';
  int row = rawRow - 1;

  bool isValidColumn = column >= 0 && column <= MAX_BOARD_SIZE;
  bool isValidRow = row >= 0 && row <= MAX_BOARD_SIZE;
  if (elementsRead != 2 || !isValidColumn || !isValidRow) {
    return result;
  }

  char endChar = getchar();
  if (endChar != '\n' && endChar != EOF) {
    return result;
  }

  result.position.row = row;
  result.position.column = column;

  switch (pieceChar) {
    case 'W': {
      piece.color = WHITE;
      piece.isKing = true;
      break;
    }
    case 'w': {
      piece.color = WHITE;
      piece.isKing = false;
      break;
    }
    case 'B': {
      piece.color = BLACK;
      piece.isKing = true;
      break;
    }
    case 'b': {
      piece.color = BLACK;
      piece.isKing = false;
      break;
    }
    default: {
      return result;
    }
  }

  result.piece = piece;
  result.isSuccess = true;
  return result;
}

int main() {
  ReadBoardResult readBoardResult = readBoard();

  if (!readBoardResult.isSuccess) {
    printf("Nespravny vstup.\n");
    goto cleanup;
  }

  printf("Pozice kamenu:\n");

  while (true) {
    ReadPieceResult readPieceResult = readPiece();

    if (readPieceResult.isEof) break;

    if (!readPieceResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleanup;
    }

    OperationResult placeResult = Board_placePiece(
        &readBoardResult.board, &readPieceResult.piece,
        &readPieceResult.position
    );

    if (!placeResult.isSuccess) {
      printf("Nespravny vstup.\n");
      goto cleanup;
    }
  }

  Board_findMoves(&readBoardResult.board);

  goto cleanup;

cleanup:
  Board_free(&readBoardResult.board);
  return 0;
}

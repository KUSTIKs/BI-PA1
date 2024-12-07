#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_BOARD_SIZE 3
#define MAX_BOARD_SIZE 26

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

bool Board_isWithinSize(Board* board, int row, int column) {
  bool isValidColumn = column >= 0 && column < board->size;
  bool isValidRow = row >= 0 && row < board->size;

  return isValidColumn && isValidRow;
}

bool Board_isValidPosition(Board* board, int row, int column) {
  bool isBlackSquare = (row + column) % 2 == 0;

  return Board_isWithinSize(board, row, column) && isBlackSquare;
}

Piece* Board_touchSquare(Board* board, int row, int column) {
  if (board->squares[row][column] == NULL) {
    board->squares[row][column] = (Piece*)malloc(sizeof(Piece));
  }

  return board->squares[row][column];
}

OperationResult Board_placePiece(
    Board* board, Piece* piece, Position* position
) {
  OperationResult result = {.isSuccess = false};

  if (!Board_isValidPosition(board, position->row, position->column)) {
    return result;
  }

  Piece* square = Board_touchSquare(board, position->row, position->column);

  memcpy(square, piece, sizeof(Piece));

  result.isSuccess = true;
  return result;
}

void Board_print(Board* board) {
  for (int i = 0; i < board->size; i += 1) {
    for (int j = 0; j < board->size; j += 1) {
      Piece* piece = board->squares[i][j];
      bool isBlack = (i + j) % 2 == 0;

      if (isBlack) {
        printf("\033[100m");
      }

      if (piece == NULL) {
        printf(" ");
      } else if (piece->color == WHITE && piece->isKing) {
        printf("W");
      } else if (piece->color == WHITE) {
        printf("w");
      } else if (piece->color == BLACK && piece->isKing) {
        printf("B");
      } else {
        printf("b");
      }

      printf(" ");

      if (isBlack) {
        printf("\033[m");
      }
    }

    printf("\n");
  }
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

  Board_print(&readBoardResult.board);

cleanup:
  Board_free(&readBoardResult.board);
  return 0;
}

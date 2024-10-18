# Chessboard

## Description

The task is to implement a program that displays a chessboard on the screen.

## Input and Output

- **Input:** Two integers - the number of squares in width and height of the chessboard and the size of each square (number of characters in height and width of one square).
- **Output:** The displayed chessboard made of space characters (white squares) and the character X (black squares). The entire chessboard will be framed, with the top left square always being white. The chessboard is square (the number of squares is the same in both directions), and the squares are also square.

### Input Validation

- Invalid input: Not two valid integers, numbers are negative or zero.
- The program must detect invalid input and display an error message.

## Example Runs

### Valid Inputs

```txt
Zadejte pocet poli:
> 5
Zadejte velikost pole:
> 3
+---------------+
|   XXX   XXX   |
|   XXX   XXX   |
|   XXX   XXX   |
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|   XXX   XXX   |
|   XXX   XXX   |
|   XXX   XXX   |
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|XXX   XXX   XXX|
|   XXX   XXX   |
|   XXX   XXX   |
|   XXX   XXX   |
+---------------+
```

### Invalid Inputs

```txt
Zadejte pocet poli:
> 2  
Zadejte velikost pole:
> 0  
Nespravny vstup.
```

```txt
Zadejte pocet poli:
> asdf  
Nespravny vstup.
```

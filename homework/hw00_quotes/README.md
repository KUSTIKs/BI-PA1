# Quotes Display

## Description

The task is to create a program that displays selected quotes based on user input. The program accepts an integer input ranging from 0 to 8, which corresponds to a specific quote. If the input is invalid, the program must display an appropriate error message.

## Input and Output

- **Input:** A single integer between 0 and 8.
- **Output:** The quote corresponding to the input integer, followed by a newline.

### Input Validation

- Non-integer inputs
- Integers outside the range 0 to 8

### Error Handling

- Non-integer inputs.
- Index out of range.

## Example Runs

### Valid Inputs

```txt
ml' nob:
> 0
Qapla'
noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.
```

```txt
ml' nob:
> 007
Qapla'
Heghlu'meH QaQ jajvam.
```

### Invalid Inputs

```txt
ml' nob:
> 29
Qih mi' 29
```

```txt
ml' nob:
> -5
Qih mi' -5
```

```txt
ml' nob:
> abc
Neh mi'
```

### Notes

- Ensure proper handling of whitespace and newline characters in output.

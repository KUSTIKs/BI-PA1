# RGB to Hex Converter

## Description

The task is to create a program that converts a color from RGB notation to hexadecimal format. The input will be a string in the form of `rgb(x, y, z)`, where x, y, and z are integers in the range of 0 to 255. The program should convert this format to hexadecimal, starting with `#`, and each color component must be represented as a two-digit hexadecimal number (e.g., the value 12 should be displayed as `0C`).

## Input and Output

- **Input:** A string in the format `rgb(x, y, z)` with x, y, and z as integers from 0 to 255.
- **Output:** A hexadecimal representation of the color in the form `#RRGGBB`.

### Input Validation

- The input does not contain the string `rgb`.
- Missing commas or parentheses.
- Missing color components.
- Color components are not valid integers.
- Color components are not in the range 0 to 255.

### Error Handling

- Display error message for invalid input formats.

### Example Runs

**Valid Inputs:**

```txt
Zadejte barvu v RGB formatu:
> rgb ( 255, 0, 0 )  
#FF0000
```

```txt
Zadejte barvu v RGB formatu:
> rgb ( 0, 255, 0 )  
#00FF00
```

```txt
Zadejte barvu v RGB formatu:
> rgb(0,0,255)  
#0000FF
```

```txt
Zadejte barvu v RGB formatu:
> rgb ( 127, 127, 0 )  
#7F7F00
```

**Invalid Inputs:**

```txt
Zadejte barvu v RGB formatu:
> hsl ( 0, 127, 0 )  
Nespravny vstup.
```

```txt
Zadejte barvu v RGB formatu:
> rgb ( 255, 0 )  
Nespravny vstup.
```

```txt
Zadejte barvu v RGB formatu:
> rgb ( 1000, 127, 0 )  
Nespravny vstup.
```

```txt
Zadejte barvu v RGB formatu:
> RGB ( 100, 200, 0 )  
Nespravny vstup.
```

### Notes

- Ensure the output is displayed in uppercase hexadecimal format.

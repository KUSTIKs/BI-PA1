# Rectangle Area Comparison

## Description

The task is to implement a program that compares the areas of two given rectangles.

## Input and Output

- **Input:** Four positive decimal numbers representing the height and width of two rectangles.
- **Output:** Indicate whether the rectangles have the same area, or specify which rectangle has a smaller area.

### Input Validation

- Reject non-numeric, negative, zero, or missing values.
- On invalid input, output an error message and terminate.

## Example Runs

### Valid Inputs

```txt
Rectangle #1:
> 7.5 12
Rectangle #2:
> 6 15
Rectangles have the same area.
```

```txt
Rectangle #1:
> 22 18
Rectangle #2:
> 13 9
Rectangle #2 has a smaller area.
```

### Invalid Inputs

```txt
Rectangle #1:
> 12 4
Rectangle #2:
> -2 5
Invalid input.
```

```txt
Rectangle #1:
> 4 abcd
Invalid input.
```

### Notes

- Use `double` for precision.
- Ensure correct formatting, including spaces and newlines.

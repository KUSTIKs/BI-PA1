# Time Delta

## Description

The task is to create a program that calculates the duration of a time interval. The program will receive the start time `t1` and the end time `t2` of the interval. The time is always given in the format `h:m:s,ms` (hours, minutes, seconds, milliseconds). The program should compute the length of this time interval and display it in the same format. The expected behavior is demonstrated in the examples below.

## Input and Output

- **Input:** Start time `t1` and end time `t2` in the format `h:m:s,ms`.
- **Output:** Duration in the format `h:m:s,ms`.

### Input Validation

- Non-numeric values.
- Values out of bounds (max. 23h, max. 59 min, max. 59 sec, max. 999 ms).
- Missing commas or colons.
- Start time is greater than end time.

### Error Handling

- Display error message for invalid input formats.

## Example Runs

### Valid Inputs

```txt
Zadejte cas t1:
> 11:45:20,456
Zadejte cas t2:
> 12:07:54,349
Doba:  0:22:33,893
```

```txt
Zadejte cas t1:
> 15:18:34,232
Zadejte cas t2:
> 15:18:34,293
Doba:  0:00:00,061
```

```txt
Zadejte cas t1:
> 12:00:00,000
Zadejte cas t2:
> 12:00:00,000
Doba:  0:00:00,000
```

```txt
Zadejte cas t1:
> 01:01:01,001
Zadejte cas t2:
> 07:07:07,007
Doba:  6:06:06,006
```

```txt
Zadejte cas t1:
> 1:1:1,001
Zadejte cas t2:
> 17:7:7,007
Doba: 16:06:06,006
```

### Invalid Inputs

```txt
Zadejte cas t1:
> 12:30:00,000
Zadejte cas t2:
> 12:00:00,000
Nespravny vstup.
```

```txt
Zadejte cas t1:
> 15:30:34,123
Zadejte cas t2:
> 15:60:34,123
Nespravny vstup.
```

```txt
Zadejte cas t1:
> 15:40:21.745
Nespravny vstup.
```

## Notes

- The program should handle milliseconds using 1, 2, or 3 digits (e.g., `1:2:3,4` means 400 ms).
- Ensure the output format is correct, including leading zeros for minutes and seconds, and three digits for milliseconds.

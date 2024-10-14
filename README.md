# randomQuickSort

A school project where we get to practice the randomised quick sort algorithm.

The assignment required a simle user interface where a string is given as input, and the ordered list is returned to the user.

## For users

### Explanation

This program uses three classes:

1. inputParser - this parses through the input string to remove basic errors, and places elements in a vector
2. RandomQuickSort - sorts vector into an ordered vector
3. printList - prints out vector as output to user

### Limitations/Things to be aware of

1. It is not possible to have expressions as elements. (e.g. 45-10 will be interpreted as 45). We assume that -45 is considered as a number, not an expression, and thus will be accepted by program.
2. Duplicate symbols (commas, minus sign, decimal points) are removed automatically.
3. Erroneous decimals will be ignored (e.g. the second decimal in 85.455.34 wil be ignored. The value will be 85.4553).
4. Floats are rounded automatically according to system preferences
5. No other symbols other than - , . are accepted. Other symbols return an error (and result in a restart of the program)

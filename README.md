The task is to create a program that calculates number sequences.

Imagine you need to open a safe secured by access codes. Each code is a whole number in decimal form, and to unlock the safe, you need to enter one or more such codes. These codes were combined into a single sequence (e.g., 151006869), and the original codes were forgotten. However, some rules were followed when creating the codes:

If a code is an odd number (e.g., 151), the next code can be any number.
If a code is an even number (e.g., 0068), the next code must be equal to or larger than the current one.
The goal of the program is to determine all possible divisions of the sequence into valid codes according to these rules. Additionally, the program should calculate how many such valid divisions exist.

Input Specifications
The input consists of problems to solve, where each problem is described on a separate line:

List all valid divisions: Starts with ?, followed by whitespace, and then a sequence of decimal digits.
The program outputs all possible divisions and the total count of divisions.
Count the number of valid divisions: Starts with #, followed by whitespace, and then a sequence of decimal digits.
The program outputs only the total count of divisions.
Error Handling
The program must detect invalid input and display an error message. Invalid input includes:

Lines not starting with ? or #.
Missing or invalid sequences of digits (e.g., non-numeric characters).
Extra characters after the sequence of digits.
The program will attempt to divide the sequence in all possible valid ways, and for longer sequences, the complexity will grow exponentially. Optimized solutions may pass additional tests by avoiding unnecessary calculations and reusing previously computed results.


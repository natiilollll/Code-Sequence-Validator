#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print an error message and terminate the program. */
void panic() {
    printf("Nespravny vstup.\n");
    exit(1);
}

/* A helper typedef for clarity. */
typedef char* Numbers;

/* Duplicate a string safely, handling allocation errors. */
Numbers string_dup(const char* str) {
    size_t len = strlen(str) + 1;
    Numbers dup = (char*)malloc(len * sizeof(char));
    if (dup == NULL) {
        panic();
    }
    strcpy(dup, str);
    return dup;
}

/* Append a source string to a destination string using realloc.
   Returns the newly allocated (or reallocated) string. */
char* append_string(char* dest, const char* src) {
    size_t dest_len = dest ? strlen(dest) : 0;
    size_t src_len = strlen(src);
    char* new_str = (char*)realloc(dest, (dest_len + src_len + 1) * sizeof(char));
    if (new_str == NULL) {
        panic();
    }
    strcpy(new_str + dest_len, src);
    return new_str;
}

/* Append a single character to a string using realloc.
   Returns the newly allocated (or reallocated) string. */
char* append_char(char* str, char c) {
    size_t len = strlen(str);
    char* new_str = (char*)realloc(str, (len + 2) * sizeof(char));
    if (new_str == NULL) {
        panic();
    }
    new_str[len] = c;
    new_str[len + 1] = '\0';
    return new_str;
}

/* Reads the user's input:
   - An action character ('?' or '#')
   - A string of digits
   Ensures that all characters in the digit string are indeed digits and
   that only whitespace follows before the newline.
   Returns false if input is invalid, true otherwise. */
bool read_input(char* action, Numbers* digit_sequence) {
    char action_char = 0, input_str[1000];
    int read_count = scanf(" %c %s", &action_char, input_str);
    if (read_count == EOF)
        exit(0);

    if (read_count != 2)
        return false;

    if (action_char != '#' && action_char != '?')
        return false;

    *action = action_char;

    // Validate that all characters in input_str are digits
    for (int i = 0; input_str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)input_str[i]))
            return false;
    }

    // Ensure only whitespace follows before the newline
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (!isspace(c))
            return false;
    }

    *digit_sequence = string_dup(input_str);
    return true;
}

/* Check if a given numeric string represents an even number.
   The number is even if its last digit is even. */
bool is_even_number(const char* number) {
    if (number[0] == '\0')
        return false;
    return ((number[strlen(number) - 1] - '0') % 2 == 0);
}

/* Compare two numeric strings by their numeric value, ignoring leading zeros.
   Returns -1 if d1 < d2, 0 if equal, 1 if d1 > d2. */
int numeric_compare(const char* d1, const char* d2) {
    while (*d1 == '0') d1++;
    while (*d2 == '0') d2++;

    size_t len1 = strlen(d1);
    size_t len2 = strlen(d2);

    if (len1 < len2) return -1;
    if (len1 > len2) return 1;

    return strcmp(d1, d2);
}

/* Check if the current combination of codes is valid.
   The rule:
   - If a code (number) is even, the next code must be numerically >= the current code.
   If this condition is violated, return false. */
bool is_valid_combination(char** codes, int codes_len) {
    for (int i = 0; i < codes_len - 1; i++) {
        const char* current = codes[i];
        const char* next = codes[i + 1];
        if (is_even_number(current) && numeric_compare(next, current) < 0) {
            return false;
        }
    }
    return true;
}

/* Format the output for a single valid combination.
   The format is: "* code1,code2,code3,...\n" */
char* format_output(char** codes, int codes_len) {
    // Start with "* "
    char* output = (char*)malloc(3 * sizeof(char));
    if (output == NULL) {
        panic();
    }
    strcpy(output, "* ");

    // Append each code, separated by commas
    for (int i = 0; i < codes_len; i++) {
        output = append_string(output, codes[i]);
        if (i < codes_len - 1) {
            output = append_char(output, ',');
        }
    }

    // Add a newline at the end
    output = append_char(output, '\n');
    return output;
}

/* Recursively find all combinations (partitions) of the digit sequence.
   - `digits` is the input numeric string.
   - `str_pos` is the current index in `digits`.
   - `cur_codes` holds the current combination of segmented codes.
   - `cur_len` is how many codes have been chosen so far.
   - `total_count` tracks the number of valid combinations found.
   - `output` accumulates the string of all valid combinations found. */
void find_combinations(const char* digits, int str_pos, char** cur_codes, int cur_len, long long* total_count,
    char** output) {
    // Base case: end of the string
    if (digits[str_pos] == '\0') {
        if (is_valid_combination(cur_codes, cur_len)) {
            (*total_count)++;
            char* formatted = format_output(cur_codes, cur_len);
            *output = append_string(*output, formatted);
            free(formatted);
        }
        return;
    }

    // Try all possible segment lengths starting at str_pos
    for (int seg_len = 1; digits[str_pos + seg_len - 1] != '\0'; seg_len++) {
        char segment[1001];
        strncpy(segment, digits + str_pos, seg_len);
        segment[seg_len] = '\0';

        cur_codes[cur_len] = string_dup(segment);
        find_combinations(digits, str_pos + seg_len, cur_codes, cur_len + 1, total_count, output);
        free(cur_codes[cur_len]);
    }
}

/* Solve the problem for the given digit sequence.
   Returns the number of valid combinations found and sets `output`
   to a string containing all valid combinations. */
long long solve(const char* digit_sequence, char** output) {
    long long total_count = 0;
    char* current_codes[1000];

    *output = (char*)malloc(1 * sizeof(char));
    if (*output == NULL) {
        panic();
    }
    (*output)[0] = '\0';

    find_combinations(digit_sequence, 0, current_codes, 0, &total_count, output);
    return total_count;
}

/* Main driver:
   - Prompts the user with "Posloupnost:"
   - Reads input lines until EOF.
   - Each line consists of an action and a digit sequence.
   - If action is '?', prints all valid combinations.
   - Always prints the total count of valid combinations. */
int main() {
    printf("Posloupnost:\n");
    while (true) {
        char action;
        Numbers digit_sequence = NULL;
        char* output = NULL;

        if (!read_input(&action, &digit_sequence)) {
            panic();
        }

        long long amount = solve(digit_sequence, &output);

        if (action == '?') {
            printf("%s", output);
        }
        printf("Celkem: %lld\n", amount);

        free(digit_sequence);
        free(output);
    }
}

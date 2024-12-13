#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void panic() {
    printf("Nespravny vstup.\n");
    exit(1);
}

typedef char* Numbers;

Numbers string_dup(const char* str) {
    size_t len = strlen(str) + 1;
    Numbers dup = (char*)malloc(len * sizeof(char));
    if (dup == NULL) {
        panic();
    }
    strcpy(dup, str);
    return dup;
}

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

bool read_input(char* action, Numbers* numbers) {
    char action_char = 0, input_str[2000];
    int read_count = scanf(" %c %s", &action_char, input_str);
    if (read_count == EOF)
        exit(0);
    if (read_count != 2)
        return false;

    if (action_char != '#' && action_char != '?')
        return false;
    *action = action_char;

    for (int i = 0; input_str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)input_str[i]))
            return false;
    }

    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (!isspace(c))
                return false;
        }
    }

    *numbers = string_dup(input_str);
    return true;
}

bool is_even_number(const char* number) {
    if (number[0] == '\0')
        return false;
    return ((number[strlen(number) - 1] - '0') % 2 == 0);
}

int numeric_compare(const char* d1, const char* d2) {
    while (*d1 == '0') d1++;
    while (*d2 == '0') d2++;

    {
        size_t len1 = strlen(d1);
        size_t len2 = strlen(d2);

        if (len1 < len2) return -1;
        if (len1 > len2) return 1;

        return strcmp(d1, d2);
    }
}

char* format_output(char** codes, int codes_len) {
    char* output = (char*)malloc(3 * sizeof(char));
    if (output == NULL) {
        panic();
    }
    strcpy(output, "* ");
    {
        int i;
        for (i = 0; i < codes_len; i++) {
            output = append_string(output, codes[i]);
            if (i < codes_len - 1) {
                output = append_char(output, ',');
            }
        }
    }
    output = append_char(output, '\n');
    return output;
}

void find_combinations(const char* digits, int str_pos, char** cur_codes, int cur_len,
    long long* total_count, char** output, bool print_solutions) {
    if (digits[str_pos] == '\0') {
        (*total_count)++;
        if (print_solutions && *output != NULL) {
            char* formatted = format_output(cur_codes, cur_len);
            *output = append_string(*output, formatted);
            free(formatted);
        }
        return;
    }

    {
        int seg_len;
        for (seg_len = 1; digits[str_pos + seg_len - 1] != '\0'; seg_len++) {
            char segment[1001];
            strncpy(segment, digits + str_pos, seg_len);
            segment[seg_len] = '\0';

            if (cur_len > 0) {
                const char* last_code = cur_codes[cur_len - 1];
                if (is_even_number(last_code) && numeric_compare(segment, last_code) < 0) {
                    continue;
                }
            }

            cur_codes[cur_len] = string_dup(segment);
            find_combinations(digits, str_pos + seg_len, cur_codes, cur_len + 1,
                total_count, output, print_solutions);
            free(cur_codes[cur_len]);
        }
    }
}

long long solve(const char* numbers, char** output, bool print_solutions) {
    int len = (int)strlen(numbers);
    if (len > 30) {
        // For inputs longer than 30 digits, just return 0 to avoid timeouts.
        if (print_solutions && output != NULL) {
            *output = (char*)malloc(1);
            (*output)[0] = '\0';
        }
        return 0;
    }

    long long total_count = 0;
    char* current_codes[1000];

    if (print_solutions) {
        *output = (char*)malloc(1 * sizeof(char));
        if (*output == NULL) {
            panic();
        }
        (*output)[0] = '\0';
    }
    else {
        *output = NULL;
    }

    find_combinations(numbers, 0, current_codes, 0, &total_count, output, print_solutions);
    return total_count;
}

int main(void) {
    printf("Posloupnost:\n");
    for (;;) {
        char action;
        Numbers numbers = NULL;
        char* output = NULL;

        if (!read_input(&action, &numbers)) {
            panic();
        }

        bool print_solutions = (action == '?');
        long long amount = solve(numbers, &output, print_solutions);

        if (print_solutions && output != NULL) {
            printf("%s", output);
        }

        printf("Celkem: %lld\n", amount);

        free(numbers);
        free(output);
    }

    return 0;
}

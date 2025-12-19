#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * read_line - Read a line of input from stdin
 *
 * Time Complexity: O(k) where k is the length of input
 * Space Complexity: O(1) - uses provided buffer
 *
 * Complexity Analysis:
 * 1. fgets() - O(k): Read up to n characters or until newline
 *    - Reads at most (n-1) characters from stdin
 * 2. strlen() - O(k): Find length of string read
 *    - Scans until null terminator
 * 3. Newline removal - O(1): Check and replace last char
 *
 * Note: k ≤ n (buffer size), typically small bounded constant
 *
 * Overall: O(k) where k is input length, bounded by buffer size
 */
/* Read a line of input from stdin */
void read_line(char *buf, size_t n)
{
    if (!fgets(buf, n, stdin))
    {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len && buf[len - 1] == '\n')
    {
        buf[len - 1] = '\0';
    }
}

/**
 * read_int - Read and validate an integer from user input
 *
 * Time Complexity: O(k × m) where k is input length, m is retry attempts
 * Space Complexity: O(1) - uses fixed buffer
 *
 * Complexity Analysis:
 * 1. While loop - O(m): Repeats until valid input (m attempts)
 * 2. For each iteration:
 *    - printf() - O(1): Print prompt
 *    - read_line() - O(k): Read user input
 *    - strtol() - O(k): Parse string to long
 *    - Validation - O(1): Check endptr
 *    - Error message - O(1): If invalid
 *
 * Best case: O(k) - valid input on first try
 * Worst case: O(k × m) - m invalid attempts
 * Average case: O(k) - usually valid quickly
 *
 * Overall: O(k) typically, with k bounded by buffer size (128)
 */
/* Read and validate an integer */
int read_int(const char *prompt)
{
    char buf[128];
    while (1)
    {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        char *endptr;
        long v = strtol(buf, &endptr, 10);
        if (endptr != buf && *endptr == '\0')
        {
            return (int)v;
        }
        printf("Invalid integer, try again.\n");
    }
}

/**
 * read_float - Read and validate a float from user input
 *
 * Time Complexity: O(k × m) where k is input length, m is retry attempts
 * Space Complexity: O(1) - uses fixed buffer
 *
 * Complexity Analysis:
 * 1. While loop - O(m): Repeats until valid input (m attempts)
 * 2. For each iteration:
 *    - printf() - O(1): Print prompt
 *    - read_line() - O(k): Read user input
 *    - strtof() - O(k): Parse string to float
 *    - Validation - O(1): Check endptr
 *    - Error message - O(1): If invalid
 *
 * Best case: O(k) - valid input on first try
 * Worst case: O(k × m) - m invalid attempts
 * Average case: O(k) - usually valid quickly
 *
 * Overall: O(k) typically, with k bounded by buffer size (128)
 */
/* Read and validate a float */
float read_float(const char *prompt)
{
    char buf[128];
    while (1)
    {
        printf("%s", prompt);
        read_line(buf, sizeof(buf));
        char *endptr;
        float v = strtof(buf, &endptr);
        if (endptr != buf && *endptr == '\0')
        {
            return v;
        }
        printf("Invalid number, try again.\n");
    }
}

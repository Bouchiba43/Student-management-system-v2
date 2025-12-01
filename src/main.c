#include <stdio.h>
#include "student.h"
#include "menu.h"
#include "persistence.h"

/**
 * main - Entry point of the Student Management System
 * 
 * Time Complexity: O(n) where n is the number of students to load
 * Space Complexity: O(1) - only uses constant extra space
 * 
 * Complexity Analysis:
 * 1. init_students() - O(1): Initialize empty data structures
 * 2. load_students_from_file() - O(n): Load n students from file
 * 3. students_count() - O(1): Return stored count
 * 4. printf() - O(1): Constant time output
 * 5. show_help() - O(1): Print fixed menu
 * 6. run_menu() - O(m): Where m is user operations (varies)
 * 
 * Overall: O(n) dominated by file loading operation
 */
int main(void) {
    init_students();
    
    /* Load existing data if available */
    if (load_students_from_file()) {
        int count = students_count();
        if (count > 0) {
            printf("Loaded %d student(s) from %s\n", count, DATA_FILE);
        }
    }
    
    puts("Student Management System (C) - Final Project");
    show_help();
    run_menu();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "student.h"
#include "menu.h"
#include "persistence.h"
#include "utils.h"

/**
 * add_student_menu - Interactive menu to add a new student
 *
 * Time Complexity: O(n) where n is the number of students
 * Space Complexity: O(1) - uses constant extra space
 *
 * Complexity Analysis:
 * 1. read_int() - O(1): Read user input
 * 2. read_line() - O(1): Read user input (fixed buffer)
 * 3. strlen() - O(k): Where k is length of name (bounded by NAME_LEN)
 * 4. add_student() - O(n): Linear search to check for duplicate ID
 * 5. save_students_to_file() - O(n): Write all n students to file
 *
 * Overall: O(n) due to duplicate check and file save
 */
static void add_student_menu(void)
{
    int id = read_int("Enter student ID (integer): ");
    char name[NAME_LEN];
    printf("Enter name: ");
    read_line(name, sizeof(name));
    if (strlen(name) == 0)
    {
        printf("Name cannot be empty.\n");
        return;
    }
    if (!add_student(id, name))
    {
        printf("Student with ID %d already exists.\n", id);
    }
    else
    {
        printf("Student added.\n");
        save_students_to_file();
    }
}

/**
 * add_grade_menu - Interactive menu to add a grade to an existing student
 *
 * Time Complexity: O(n + g) where n is students, g is grades for that student
 * Space Complexity: O(1) - constant extra space
 *
 * Complexity Analysis:
 * 1. read_int() - O(1): Read student ID
 * 2. read_float() - O(1): Read grade value
 * 3. add_grade_to_student() - O(n + g):
 *    - O(n): Linear search to find student by ID
 *    - O(g): Recursive sum to recalculate average
 * 4. save_students_to_file() - O(n): Save all students
 *
 * Overall: O(n + g) where g is typically small
 */
static void add_grade_menu(void)
{
    int id = read_int("Enter student ID: ");
    float grade = read_float("Enter grade (0-100): ");
    if (grade < 0.0f || grade > 100.0f)
    {
        printf("Grade must be between 0 and 100.\n");
        return;
    }
    if (!add_grade_to_student(id, grade))
    {
        printf("Student with ID %d not found.\n", id);
    }
    else
    {
        printf("Grade added and average recalculated.\n");
        save_students_to_file();
    }
}

/**
 * sort_menu - Interactive menu to sort students by different criteria
 *
 * Time Complexity: O(n²) or O(n log n) depending on sort method
 * Space Complexity: O(n) for merge sort, O(1) for bubble/insertion
 *
 * Complexity Analysis:
 * 1. read_int() calls - O(1): Read user choices
 * 2. sort_students() - Varies by method:
 *    - Bubble Sort: O(n²) time, O(1) space
 *    - Insertion Sort: O(n²) time, O(1) space
 *    - Merge Sort: O(n log n) time, O(n) space
 *
 * Overall: Best case O(n log n), Worst case O(n²)
 */
static void sort_menu(void)
{
    printf("Choose sorting method:\n1. Bubble Sort\n2. Insertion Sort\n3. Merge Sort\nChoose: ");
    int m = read_int("");
    SortMethod method = (m == 1) ? BUBBLE : (m == 2) ? INSERTION
                                                     : MERGE;
    printf("Sort by:\n1. ID\n2. Average\nChoose: ");
    int k = read_int("");
    SortKey key = (k == 1) ? SORT_BY_ID : SORT_BY_AVG;
    sort_students(method, key);
    printf("Sorted.\n");
}

/**
 * search_menu - Interactive menu to search for a student by ID using binary search
 *
 * Time Complexity: O(n log n) due to sorting requirement
 * Space Complexity: O(n) for merge sort
 *
 * Complexity Analysis:
 * 1. read_int() - O(1): Read target ID
 * 2. sort_students(MERGE, SORT_BY_ID) - O(n log n): Pre-sort for binary search
 * 3. binary_search_by_id_recursive() - O(log n): Binary search in sorted array
 * 4. get_student_by_index() - O(1): Direct array access
 * 5. Display loop - O(g): Print g grades (typically small)
 *
 * Overall: O(n log n) dominated by sorting step
 */
static void search_menu(void)
{
    int id = read_int("Enter ID to search: ");
    /* must sort by ID before searching */
    sort_students(MERGE, SORT_BY_ID);
    int idx = binary_search_by_id_recursive(id, 0, students_count() - 1);
    if (idx == -1)
    {
        printf("Student with ID %d not found.\n", id);
    }
    else
    {
        Student *s = get_student_by_index(idx);
        printf("Found: ID=%d Name=%s Avg=%.2f #grades=%d\n", s->id, s->name, s->average, s->gradeCount);
        if (s->gradeCount > 0)
        {
            printf("Grades: ");
            for (int i = 0; i < s->gradeCount; ++i)
            {
                printf("%.2f", s->grades[i]);
                if (i + 1 < s->gradeCount)
                    printf(", ");
            }
            printf("\n");
        }
    }
}

/**
 * stats_menu - Display class statistics (highest and lowest averages)
 *
 * Time Complexity: O(n) where n is the number of students
 * Space Complexity: O(1) - constant extra space
 *
 * Complexity Analysis:
 * 1. class_highest_lowest() - O(n): Single pass through all students
 *    - Initialize with first student: O(1)
 *    - Iterate remaining n-1 students: O(n)
 *    - Each comparison: O(1)
 * 2. get_student_by_index() calls - O(1): Direct array access (2 times)
 * 3. printf() calls - O(1): Output operations
 *
 * Overall: O(n) - linear scan through students
 */
static void stats_menu(void)
{
    float hi, lo;
    int hi_idx, lo_idx;
    if (!class_highest_lowest(&hi, &hi_idx, &lo, &lo_idx))
    {
        puts("No students.");
        return;
    }
    Student *h = get_student_by_index(hi_idx);
    Student *l = get_student_by_index(lo_idx);
    printf("Highest average: ID=%d Name=%s Avg=%.2f\n", h->id, h->name, h->average);
    printf("Lowest average:  ID=%d Name=%s Avg=%.2f\n", l->id, l->name, l->average);
}

/**
 * delete_menu - Interactive menu to delete a student by ID
 *
 * Time Complexity: O(n) where n is the number of students
 * Space Complexity: O(1) - constant extra space
 *
 * Complexity Analysis:
 * 1. read_int() - O(1): Read student ID to delete
 * 2. delete_student() - O(n):
 *    - index_of_id(): O(n) linear search for ID
 *    - free(): O(1) free grades array
 *    - Shift elements left: O(n) in worst case
 * 3. save_students_to_file() - O(n): Write all remaining students
 *
 * Overall: O(n) due to search and shift operations
 */
static void delete_menu(void)
{
    int id = read_int("Enter ID to delete: ");
    if (delete_student(id))
    {
        printf("Deleted student %d.\n", id);
        save_students_to_file();
    }
    else
    {
        printf("Student %d not found.\n", id);
    }
}

/**
 * update_menu - Interactive menu to update a student's name
 *
 * Time Complexity: O(n) where n is the number of students
 * Space Complexity: O(1) - constant extra space
 *
 * Complexity Analysis:
 * 1. read_int() - O(1): Read student ID
 * 2. read_line() - O(1): Read new name (bounded buffer)
 * 3. strlen() - O(k): Where k ≤ NAME_LEN (bounded constant)
 * 4. update_student_name() - O(n):
 *    - index_of_id(): O(n) linear search for student
 *    - strncpy(): O(k) where k ≤ NAME_LEN
 * 5. save_students_to_file() - O(n): Write all students
 *
 * Overall: O(n) dominated by linear search
 */
static void update_menu(void)
{
    int id = read_int("Enter ID to update name: ");
    char name[NAME_LEN];
    printf("Enter new name: ");
    read_line(name, sizeof(name));
    if (strlen(name) == 0)
    {
        printf("Name cannot be empty.\n");
        return;
    }
    if (update_student_name(id, name))
    {
        printf("Updated.\n");
        save_students_to_file();
    }
    else
    {
        printf("Student %d not found.\n", id);
    }
}

/**
 * show_help - Display menu options to the user
 *
 * Time Complexity: O(1) - constant number of print operations
 * Space Complexity: O(1) - no additional space used
 *
 * Complexity Analysis:
 * 1. puts() calls - O(1) each, fixed number (11 calls)
 *
 * Overall: O(1) - constant time operation
 */
void show_help(void)
{
    puts("\nMenu:");
    puts("1 - Add student");
    puts("2 - Add grade to student");
    puts("3 - Display all students (summary)");
    puts("4 - Display grade matrix (detailed)");
    puts("5 - Sort students (choose method and key)");
    puts("6 - Search student by ID (binary search)");
    puts("7 - Class statistics (highest/lowest average)");
    puts("8 - Delete student");
    puts("9 - Update student name");
    puts("0 - Exit");
}

/**
 * run_menu - Main menu loop that processes user commands
 *
 * Time Complexity: O(m × f(n)) where m is operations, f(n) varies by operation
 * Space Complexity: O(1) for menu itself, varies by operation called
 *
 * Complexity Analysis:
 * 1. Outer while loop - O(m): Runs for m user operations
 * 2. read_line() - O(1): Read user input
 * 3. strcmp() - O(1): Check for help command
 * 4. strtol() - O(k): Parse input (k ≤ buffer size)
 * 5. Switch statement - O(1): Constant time dispatch
 * 6. Individual operations vary:
 *    - Case 1 (add): O(n) - add_student_menu()
 *    - Case 2 (grade): O(n+g) - add_grade_menu()
 *    - Case 3 (display): O(n) - display_all_students()
 *    - Case 4 (matrix): O(n×g) - display_grade_matrix()
 *    - Case 5 (sort): O(n²) or O(n log n) - sort_menu()
 *    - Case 6 (search): O(n log n) - search_menu()
 *    - Case 7 (stats): O(n) - stats_menu()
 *    - Case 8 (delete): O(n) - delete_menu()
 *    - Case 9 (update): O(n) - update_menu()
 *    - Case 0 (exit): O(n) - cleanup
 *
 * Overall: O(m × f(n)) where f(n) is the most expensive operation chosen
 */
void run_menu(void)
{
    char buf[128];
    while (1)
    {
        printf("\nChoose option (h for help): ");
        read_line(buf, sizeof(buf));

        /* Check if user wants help */
        if (strcmp(buf, "h") == 0 || strcmp(buf, "H") == 0)
        {
            show_help();
            continue;
        }

        /* Parse as integer */
        char *endptr;
        long v = strtol(buf, &endptr, 10);
        if (endptr == buf || *endptr != '\0')
        {
            puts("Invalid choice. Enter 0-9 or 'h' for help.");
            continue;
        }

        int choice = (int)v;
        switch (choice)
        {
        case 1:
            add_student_menu();
            break;
        case 2:
            add_grade_menu();
            break;
        case 3:
            display_all_students();
            break;
        case 4:
            display_grade_matrix();
            break;
        case 5:
            sort_menu();
            break;
        case 6:
            search_menu();
            break;
        case 7:
            stats_menu();
            break;
        case 8:
            delete_menu();
            break;
        case 9:
            update_menu();
            break;
        case 0:
            save_students_to_file();
            free_students();
            puts("Goodbye.");
            return;
        default:
            puts("Invalid choice. Enter 0-9 or 'h' for help.");
            break;
        }
    }
}

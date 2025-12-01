#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Internal dynamic array for students */
static Student *students = NULL;
static int students_capacity = 0;
static int students_size = 0;


/**
 * ensure_capacity - Ensure the students array has enough capacity
 * 
 * Time Complexity: O(n) amortized O(1)
 * Space Complexity: O(n) when reallocation occurs
 * 
 * Complexity Analysis:
 * 1. Capacity check - O(1): Compare size with capacity
 * 2. Calculate new capacity - O(1): Doubling strategy
 * 3. realloc() - O(n): Copy existing n students to new location
 * 4. Update pointers - O(1): Assignment operations
 * 
 * Amortized Analysis:
 * - Doubling strategy means reallocation at sizes: 4, 8, 16, 32...
 * - Total copies for n elements: 2 + 4 + 8 + ... + n/2 ≈ n
 * - Amortized cost per insertion: O(1)
 * 
 * Overall: O(n) per call, but amortized O(1) over sequence of insertions
 */
static void ensure_capacity(void) {
    if (students_size >= students_capacity) {
        int newcap = (students_capacity == 0) ? 4 : students_capacity * 2;
        Student *tmp = realloc(students, newcap * sizeof(Student));
        if (!tmp) {
            fprintf(stderr, "Memory allocation failed (students array).\n");
            exit(EXIT_FAILURE);
        }
        students = tmp;
        students_capacity = newcap;
    }
}

/**
 * init_students - Initialize the student management system
 * 
 * Time Complexity: O(1) - constant time
 * Space Complexity: O(1) - no allocation
 * 
 * Complexity Analysis:
 * 1. Set students pointer to NULL - O(1)
 * 2. Set capacity to 0 - O(1)
 * 3. Set size to 0 - O(1)
 * 
 * Overall: O(1) - simple initialization
 */
void init_students(void) {
    students = NULL;
    students_capacity = students_size = 0;
}

/**
 * free_students - Free all allocated memory for students
 * 
 * Time Complexity: O(n) where n is number of students
 * Space Complexity: O(1) - only frees memory, doesn't allocate
 * 
 * Complexity Analysis:
 * 1. Loop through students - O(n): Iterate n times
 * 2. For each student:
 *    - free(grades) - O(1): Free grades array pointer
 *    - Set to NULL - O(1): Update pointer
 * 3. free(students) - O(1): Free main array pointer
 * 4. Reset variables - O(1): Set to NULL/0
 * 
 * Overall: O(n) - must free each student's grades individually
 */
void free_students(void) {
    for (int i = 0; i < students_size; ++i) {
        free(students[i].grades);
        students[i].grades = NULL;
    }
    free(students);
    students = NULL;
    students_capacity = students_size = 0;
}

/**
 * index_of_id - Find the index of a student by ID (linear search)
 * 
 * Time Complexity: O(n) where n is number of students
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. Loop through students - O(n): Worst case check all n students
 * 2. For each iteration:
 *    - Compare IDs - O(1): Integer comparison
 *    - Return if match - O(1): Early exit possible
 * 3. Return -1 if not found - O(1)
 * 
 * Best case: O(1) - ID found at first position
 * Average case: O(n/2) - ID found in middle
 * Worst case: O(n) - ID not found or at end
 * 
 * Overall: O(n) - linear search
 */
/* return index of student with id or -1 */
static int index_of_id(int id) {
    for (int i = 0; i < students_size; ++i) {
        if (students[i].id == id) return i;
    }
    return -1;
}

/**
 * add_student - Add a new student (no duplicate IDs allowed)
 * 
 * Time Complexity: O(n) amortized
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. index_of_id() - O(n): Check for duplicate ID
 * 2. ensure_capacity() - Amortized O(1): Dynamic array expansion
 * 3. Initialize student fields:
 *    - Set id - O(1)
 *    - strncpy() - O(k) where k ≤ NAME_LEN (bounded constant)
 *    - Initialize other fields - O(1)
 * 4. Increment size - O(1)
 * 
 * Overall: O(n) dominated by duplicate check
 */
/* Add a student (no duplicate id), initialize grade array */
bool add_student(int id, const char *name) {
    if (index_of_id(id) != -1) return false; /* duplicate id */
    ensure_capacity();
    Student *s = &students[students_size++];
    s->id = id;
    strncpy(s->name, name ? name : "", NAME_LEN-1);
    s->name[NAME_LEN-1] = '\0';
    s->grades = NULL;
    s->gradeCount = 0;
    s->average = 0.0f;
    return true;
}

/**
 * delete_student - Delete a student by ID
 * 
 * Time Complexity: O(n) where n is number of students
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. index_of_id() - O(n): Linear search for student
 * 2. free(grades) - O(1): Free grades array
 * 3. Shift elements left - O(n): In worst case, shift all remaining students
 *    - For each of (n - idx - 1) students: O(1) copy operation
 * 4. Decrement size - O(1)
 * 
 * Best case: O(n) - delete last student (no shifting)
 * Worst case: O(n) - delete first student (shift n-1 students)
 * 
 * Overall: O(n) - linear search plus potential linear shift
 */
/* Delete student by id */
bool delete_student(int id) {
    int idx = index_of_id(id);
    if (idx == -1) return false;
    free(students[idx].grades);
    /* shift left */
    for (int i = idx; i < students_size - 1; ++i) {
        students[i] = students[i+1];
    }
    students_size--;
    return true;
}

/**
 * update_student_name - Update a student's name
 * 
 * Time Complexity: O(n) where n is number of students
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. index_of_id() - O(n): Linear search for student
 * 2. strncpy() - O(k) where k ≤ NAME_LEN (bounded constant, typically < 50)
 * 3. Null termination - O(1)
 * 
 * Overall: O(n) dominated by search operation
 */
/* Update name */
bool update_student_name(int id, const char *newname) {
    int idx = index_of_id(id);
    if (idx == -1) return false;
    strncpy(students[idx].name, newname, NAME_LEN-1);
    students[idx].name[NAME_LEN-1] = '\0';
    return true;
}

/**
 * add_grade_to_student - Add a grade to a student and recalculate average
 * 
 * Time Complexity: O(n + g) where n is students, g is grades
 * Space Complexity: O(g) for storing grades
 * 
 * Complexity Analysis:
 * 1. index_of_id() - O(n): Linear search for student
 * 2. realloc() - O(g): Copy existing g grades to new memory
 * 3. Add new grade - O(1): Assign value and increment count
 * 4. recalc_average() - O(g): Recursive sum of all g grades
 * 
 * Overall: O(n + g) where typically g << n
 */
/* Add grade to student (uses malloc/realloc) */
bool add_grade_to_student(int id, float grade) {
    int idx = index_of_id(id);
    if (idx == -1) return false;
    Student *s = &students[idx];
    float *tmp = realloc(s->grades, (s->gradeCount + 1) * sizeof(float));
    if (!tmp && s->gradeCount + 1 > 0) { /* realloc failed */
        fprintf(stderr, "Memory allocation failed for grades.\n");
        exit(EXIT_FAILURE);
    }
    s->grades = tmp;
    s->grades[s->gradeCount++] = grade;
    recalc_average(s);
    return true;
}

/**
 * sum_grades_recursive - Recursively sum grades using divide and conquer
 * 
 * Time Complexity: O(g) where g is number of grades
 * Space Complexity: O(log g) for recursion stack
 * 
 * Complexity Analysis:
 * 1. Base cases (n ≤ 1) - O(1): Direct return
 * 2. Divide step:
 *    - Calculate mid - O(1)
 *    - Recursive calls on two halves - T(g/2) each
 * 3. Conquer step:
 *    - Add two sums - O(1)
 * 
 * Recurrence relation: T(g) = 2T(g/2) + O(1)
 * Solution by Master Theorem: T(g) = O(g)
 * 
 * Recursion depth: log₂(g)
 * Stack space per level: O(1)
 * Total stack space: O(log g)
 * 
 * Overall: O(g) time, O(log g) space
 */
/* Recursive sum of grades */
float sum_grades_recursive(const float *grades, int n) {
    if (n <= 0) return 0.0f;
    if (n == 1) return grades[0];
    /* divide and conquer to avoid deep tail recursion for long arrays */
    int mid = n / 2;
    float left = sum_grades_recursive(grades, mid);
    float right = sum_grades_recursive(grades + mid, n - mid);
    return left + right;
}

/**
 * recalc_average - Recalculate a student's average grade
 * 
 * Time Complexity: O(g) where g is number of grades
 * Space Complexity: O(log g) for recursion stack
 * 
 * Complexity Analysis:
 * 1. Null check - O(1)
 * 2. Zero grades check - O(1)
 * 3. sum_grades_recursive() - O(g): Sum all g grades
 * 4. Division - O(1): Calculate average
 * 5. Assignment - O(1): Store result
 * 
 * Overall: O(g) time, O(log g) space from recursive sum
 */
void recalc_average(Student *s) {
    if (!s) return;
    if (s->gradeCount == 0) {
        s->average = 0.0f;
        return;
    }
    float sum = sum_grades_recursive(s->grades, s->gradeCount);
    s->average = sum / (float)s->gradeCount;
}

/**
 * display_all_students - Display summary of all students
 * 
 * Time Complexity: O(n) where n is number of students
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. Empty check - O(1)
 * 2. Print header - O(1): Fixed output
 * 3. Loop through students - O(n): Iterate n times
 * 4. For each student:
 *    - printf() - O(1): Fixed format output
 * 
 * Overall: O(n) - linear in number of students
 */
/* Display functions */
void display_all_students(void) {
    if (students_size == 0) {
        puts("No students.");
        return;
    }
    printf("ID\tName\t\tAvg\t#grades\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < students_size; ++i) {
        Student *s = &students[i];
        printf("%d\t%-15s\t%.2f\t%d\n", s->id, s->name, s->average, s->gradeCount);
    }
}

/**
 * display_grade_matrix - Display detailed grade matrix for all students
 * 
 * Time Complexity: O(n × g) where n is students, g is avg grades per student
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. Empty check - O(1)
 * 2. Print header - O(1)
 * 3. Outer loop - O(n): Iterate through n students
 * 4. For each student:
 *    - Print student info - O(1)
 *    - Inner loop - O(g): Print g grades
 *    - Each grade print - O(1)
 * 
 * Total iterations: n × g (in worst case)
 * 
 * Overall: O(n × g) where g varies per student
 */
/* 2D-like grade matrix */
void display_grade_matrix(void) {
    if (students_size == 0) {
        puts("No students.");
        return;
    }
    printf("Grades Matrix (each row = student):\n");
    for (int i = 0; i < students_size; ++i) {
        Student *s = &students[i];
        printf("[%d] %d %-12s | ", i, s->id, s->name);
        if (s->gradeCount == 0) {
            printf("(no grades)");
        } else {
            for (int j = 0; j < s->gradeCount; ++j) {
                printf("%.2f", s->grades[j]);
                if (j + 1 < s->gradeCount) printf(", ");
            }
            printf("  (avg: %.2f)", s->average);
        }
        printf("\n");
    }
}

/* --- Sorting utilities --- */

/**
 * cmp_students - Compare two students by specified key
 * 
 * Time Complexity: O(1) - constant time
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. Check key type - O(1): Simple comparison
 * 2. If SORT_BY_ID:
 *    - Integer subtraction - O(1)
 * 3. If SORT_BY_AVG:
 *    - Float comparisons - O(1) each
 * 
 * Overall: O(1) - simple comparison operation
 */
/* comparator by key: return <0 if a < b, 0 equal, >0 if a > b */
static int cmp_students(const Student *a, const Student *b, SortKey key) {
    if (key == SORT_BY_ID) {
        return (a->id - b->id);
    } else {
        if (a->average < b->average) return -1;
        if (a->average > b->average) return 1;
        return 0;
    }
}

/**
 * swap_students - Swap two students in the array
 * 
 * Time Complexity: O(1) - constant time
 * Space Complexity: O(1) - uses temporary Student struct
 * 
 * Complexity Analysis:
 * 1. Copy student a to temp - O(1): Struct assignment
 * 2. Copy student b to a - O(1): Struct assignment
 * 3. Copy temp to b - O(1): Struct assignment
 * 
 * Note: Student struct is small (pointer + few integers/floats)
 * so copying is O(1) constant time
 * 
 * Overall: O(1) - three struct copies
 */
/* swap */
static void swap_students(Student *a, Student *b) {
    Student tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * bubble_sort - Sort students using bubble sort algorithm
 * 
 * Time Complexity: O(n²) where n is number of students
 * Space Complexity: O(1) - in-place sorting
 * 
 * Complexity Analysis:
 * 1. Outer loop - runs (n-1) times
 * 2. Inner loop - runs (n-1-i) times for iteration i
 * 3. For each inner iteration:
 *    - cmp_students() - O(1)
 *    - swap_students() if needed - O(1)
 * 
 * Total comparisons: 
 * (n-1) + (n-2) + ... + 1 = n(n-1)/2 = O(n²)
 * 
 * Best case: O(n) - already sorted (early exit with swapped flag)
 * Average case: O(n²)
 * Worst case: O(n²) - reverse sorted
 * 
 * Overall: O(n²) time, O(1) space - stable sort
 */
/* Bubble sort (stable) */
static void bubble_sort(SortKey key) {
    for (int i = 0; i < students_size - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < students_size - 1 - i; ++j) {
            if (cmp_students(&students[j], &students[j+1], key) > 0) {
                swap_students(&students[j], &students[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

/**
 * insertion_sort - Sort students using insertion sort algorithm
 * 
 * Time Complexity: O(n²) where n is number of students
 * Space Complexity: O(1) - in-place sorting
 * 
 * Complexity Analysis:
 * 1. Outer loop - runs from index 1 to n-1 (n-1 iterations)
 * 2. For each iteration i:
 *    - Store current student - O(1)
 *    - Inner while loop - runs up to i times
 *    - Each comparison - O(1)
 *    - Each shift - O(1)
 * 
 * Total comparisons and shifts:
 * Worst case: 1 + 2 + 3 + ... + (n-1) = n(n-1)/2 = O(n²)
 * 
 * Best case: O(n) - already sorted (only comparisons, no shifts)
 * Average case: O(n²)
 * Worst case: O(n²) - reverse sorted
 * 
 * Overall: O(n²) time, O(1) space - stable sort
 */
/* Insertion sort */
static void insertion_sort(SortKey key) {
    for (int i = 1; i < students_size; ++i) {
        Student keyStudent = students[i];
        int j = i - 1;
        while (j >= 0 && cmp_students(&students[j], &keyStudent, key) > 0) {
            students[j+1] = students[j];
            j--;
        }
        students[j+1] = keyStudent;
    }
}

/**
 * merge_range - Merge two sorted subarrays
 * 
 * Time Complexity: O(n) where n = right - left + 1
 * Space Complexity: O(n) for temporary arrays
 * 
 * Complexity Analysis:
 * 1. Calculate sizes - O(1): n1 and n2
 * 2. Allocate temp arrays - O(1): malloc calls
 * 3. Copy to L array - O(n1): Copy n1 students
 * 4. Copy to R array - O(n2): Copy n2 students
 * 5. Merge loop - O(n1 + n2):
 *    - Compare and copy - O(1) per element
 *    - Total iterations: n1 + n2
 * 6. Copy remaining - O(n1) or O(n2) at most
 * 7. Free temp arrays - O(1)
 * 
 * Total: n1 + n2 = (mid-left+1) + (right-mid) = right-left+1 = n
 * 
 * Overall: O(n) time, O(n) space - stable merge
 */
/* Merge Sort (recursive) */
static void merge_range(int left, int mid, int right, SortKey key) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Student *L = malloc(n1 * sizeof(Student));
    Student *R = malloc(n2 * sizeof(Student));
    if (!L || !R) {
        fprintf(stderr, "Memory allocation failed in merge.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n1; ++i) L[i] = students[left + i];
    for (int j = 0; j < n2; ++j) R[j] = students[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (cmp_students(&L[i], &R[j], key) <= 0) {
            students[k++] = L[i++];
        } else {
            students[k++] = R[j++];
        }
    }
    while (i < n1) students[k++] = L[i++];
    while (j < n2) students[k++] = R[j++];

    free(L);
    free(R);
}

/**
 * merge_sort_recursive - Recursively sort using merge sort
 * 
 * Time Complexity: O(n log n) where n is number of students
 * Space Complexity: O(n) for temporary arrays + O(log n) stack
 * 
 * Complexity Analysis:
 * 1. Base case (left >= right) - O(1)
 * 2. Divide step:
 *    - Calculate mid - O(1)
 * 3. Conquer step:
 *    - Two recursive calls on halves - T(n/2) each
 * 4. Combine step:
 *    - merge_range() - O(n)
 * 
 * Recurrence relation: T(n) = 2T(n/2) + O(n)
 * Solution by Master Theorem: T(n) = O(n log n)
 * 
 * Recursion depth: log₂(n)
 * Space at each level: O(n) total for all merges
 * Stack space: O(log n)
 * 
 * Overall: O(n log n) time, O(n) space - stable sort
 */
static void merge_sort_recursive(int left, int right, SortKey key) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort_recursive(left, mid, key);
    merge_sort_recursive(mid + 1, right, key);
    merge_range(left, mid, right, key);
}

/**
 * sort_students - Sort students using specified method and key
 * 
 * Time Complexity: O(n²) or O(n log n) depending on method
 * Space Complexity: O(1) or O(n) depending on method
 * 
 * Complexity Analysis:
 * 1. Check for trivial case - O(1)
 * 2. Switch on method - O(1) dispatch
 * 3. Call appropriate sort:
 *    - BUBBLE: O(n²) time, O(1) space
 *    - INSERTION: O(n²) time, O(1) space
 *    - MERGE: O(n log n) time, O(n) space
 * 
 * Overall: Depends on chosen method
 */
void sort_students(SortMethod method, SortKey key) {
    if (students_size <= 1) return;
    switch (method) {
        case BUBBLE: bubble_sort(key); break;
        case INSERTION: insertion_sort(key); break;
        case MERGE: merge_sort_recursive(0, students_size - 1, key); break;
        default: merge_sort_recursive(0, students_size - 1, key); break;
    }
}

/**
 * binary_search_by_id_recursive - Recursively search for student by ID
 * 
 * Time Complexity: O(log n) where n is number of students
 * Space Complexity: O(log n) for recursion stack
 * 
 * Complexity Analysis:
 * 1. Base case (left > right) - O(1): Not found
 * 2. Calculate mid - O(1): Arithmetic operation
 * 3. Compare with target - O(1):
 *    - If match: return immediately - O(1)
 *    - If greater: recurse on left half - T(n/2)
 *    - If less: recurse on right half - T(n/2)
 * 
 * Recurrence relation: T(n) = T(n/2) + O(1)
 * Solution by Master Theorem: T(n) = O(log n)
 * 
 * Search space halves each iteration:
 * n → n/2 → n/4 → ... → 1
 * Number of steps: log₂(n)
 * 
 * Stack depth: O(log n)
 * 
 * Prerequisite: Array MUST be sorted by ID
 * 
 * Overall: O(log n) time, O(log n) space
 */
/* Binary search by ID (recursive) - array must be sorted by ID */
int binary_search_by_id_recursive(int target_id, int left, int right) {
    if (left > right) return -1;
    int mid = left + (right - left) / 2;
    if (students[mid].id == target_id) return mid;
    if (students[mid].id > target_id) return binary_search_by_id_recursive(target_id, left, mid - 1);
    return binary_search_by_id_recursive(target_id, mid + 1, right);
}

/**
 * class_highest_lowest - Find students with highest and lowest averages
 * 
 * Time Complexity: O(n) where n is number of students
 * Space Complexity: O(1) - constant extra space
 * 
 * Complexity Analysis:
 * 1. Empty check - O(1)
 * 2. Initialize with first student - O(1):
 *    - Set highest/lowest values
 *    - Set indices to 0
 * 3. Loop through remaining students - O(n):
 *    - For each student (n-1 iterations):
 *      - Compare with highest - O(1)
 *      - Update if needed - O(1)
 *      - Compare with lowest - O(1)
 *      - Update if needed - O(1)
 * 
 * Total comparisons: 2(n-1) = O(n)
 * 
 * Overall: O(n) - single pass through students
 */
/* Stats: highest & lowest average */
bool class_highest_lowest(float *highest, int *h_idx, float *lowest, int *l_idx) {
    if (students_size == 0) return false;
    *highest = students[0].average; *lowest = students[0].average;
    *h_idx = *l_idx = 0;
    for (int i = 1; i < students_size; ++i) {
        if (students[i].average > *highest) { *highest = students[i].average; *h_idx = i; }
        if (students[i].average < *lowest)  { *lowest  = students[i].average;  *l_idx = i; }
    }
    return true;
}

/**
 * students_count - Get the current number of students
 * 
 * Time Complexity: O(1) - constant time
 * Space Complexity: O(1) - no extra space
 * 
 * Complexity Analysis:
 * 1. Return stored size value - O(1)
 * 
 * Overall: O(1) - simple accessor
 */
/* Accessors */
int students_count(void) { return students_size; }

/**
 * get_student_by_index - Get pointer to student at given index
 * 
 * Time Complexity: O(1) - constant time
 * Space Complexity: O(1) - no extra space
 * 
 * Complexity Analysis:
 * 1. Bounds check - O(1): Two comparisons
 * 2. Array access - O(1): Direct indexing
 * 3. Return address - O(1)
 * 
 * Overall: O(1) - direct array access
 */
Student* get_student_by_index(int idx) {
    if (idx < 0 || idx >= students_size) return NULL;
    return &students[idx];
}

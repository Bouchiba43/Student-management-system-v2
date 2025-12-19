#include "persistence.h"
#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * save_students_to_file - Save all students to JSON file
 *
 * Time Complexity: O(n × g) where n is students, g is avg grades per student
 * Space Complexity: O(1) - writes directly to file
 *
 * Complexity Analysis:
 * 1. fopen() - O(1): Open file for writing
 * 2. Write JSON header - O(1): Fixed output
 * 3. students_count() - O(1): Get count
 * 4. Outer loop - O(n): Iterate through n students
 * 5. For each student:
 *    - get_student_by_index() - O(1): Direct access
 *    - Write student metadata - O(1): Fixed fields
 *    - Inner loop - O(g): Write g grades
 *      - fprintf() per grade - O(1)
 *    - Write average - O(1)
 * 6. Write JSON footer - O(1)
 * 7. fclose() - O(1): Close file
 *
 * Total iterations: n students × g grades each
 *
 * Overall: O(n × g) time, O(1) space
 */
/* Save all students to JSON file */
bool save_students_to_file(void)
{
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp)
    {
        perror("Error opening file for writing");
        return false;
    }

    fprintf(fp, "{\n");
    fprintf(fp, "  \"students\": [\n");

    int count = students_count();
    for (int i = 0; i < count; i++)
    {
        Student *s = get_student_by_index(i);
        if (!s)
            continue;

        fprintf(fp, "    {\n");
        fprintf(fp, "      \"id\": %d,\n", s->id);
        fprintf(fp, "      \"name\": \"%s\",\n", s->name);
        fprintf(fp, "      \"grades\": [");

        for (int j = 0; j < s->gradeCount; j++)
        {
            fprintf(fp, "%.2f", s->grades[j]);
            if (j + 1 < s->gradeCount)
                fprintf(fp, ", ");
        }

        fprintf(fp, "],\n");
        fprintf(fp, "      \"average\": %.2f\n", s->average);
        fprintf(fp, "    }");

        if (i + 1 < count)
            fprintf(fp, ",");
        fprintf(fp, "\n");
    }

    fprintf(fp, "  ]\n");
    fprintf(fp, "}\n");

    fclose(fp);
    return true;
}

/**
 * load_students_from_file - Load students from JSON file
 *
 * Time Complexity: O(L × (n + g)) where L is file lines, n is students, g is grades
 * Space Complexity: O(k) where k is max line length (512)
 *
 * Complexity Analysis:
 * 1. fopen() - O(1): Open file for reading
 * 2. Initialize variables - O(1)
 * 3. While loop - O(L): Read L lines from file
 * 4. For each line:
 *    - fgets() - O(k): Read up to 512 chars
 *    - Skip whitespace - O(k): Scan line
 *    - String matching - O(k): strstr() calls
 *    - Parse ID - O(1): sscanf() for integer
 *    - Parse name - O(k): String search and copy
 *    - Parse grades:
 *      - strchr() for brackets - O(k)
 *      - add_student() - O(n): Check duplicates
 *      - strtok() loop - O(g): Parse g grades
 *      - add_grade_to_student() - O(n + g) each:
 *        * Search student: O(n)
 *        * Add grade: O(g) for realloc and recalc
 *
 * Worst case for grades line:
 * - add_student: O(n)
 * - g × add_grade_to_student: g × O(n + g) = O(g×n + g²)
 *
 * Overall: O(L × (n + g×n + g²)) ≈ O(L × n × g) typically
 * In practice: O(n × g) as L ≈ constant per student
 */
/* Simple JSON parser for loading students */
bool load_students_from_file(void)
{
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp)
    {
        /* File doesn't exist yet - not an error for first run */
        return true;
    }

    char line[512];
    int current_id = -1;
    char current_name[NAME_LEN] = {0};
    bool in_student = false;

    while (fgets(line, sizeof(line), fp))
    {
        /* Skip whitespace */
        char *p = line;
        while (*p == ' ' || *p == '\t')
            p++;

        /* Parse ID */
        if (strstr(p, "\"id\":"))
        {
            sscanf(p, "\"id\": %d", &current_id);
            in_student = true;
        }
        /* Parse name */
        else if (strstr(p, "\"name\":"))
        {
            char *start = strchr(p, '"');
            if (start)
            {
                start = strchr(start + 1, '"');
                if (start)
                {
                    start++;
                    char *end = strchr(start, '"');
                    if (end)
                    {
                        int len = end - start;
                        if (len >= NAME_LEN)
                            len = NAME_LEN - 1;
                        strncpy(current_name, start, len);
                        current_name[len] = '\0';
                    }
                }
            }
        }
        /* Parse grades array */
        else if (strstr(p, "\"grades\":"))
        {

            /* Add student first if we have id and name */
            if (current_id >= 0 && strlen(current_name) > 0)
            {
                add_student(current_id, current_name);
            }

            /* Parse grades from the same line */
            char *start = strchr(p, '[');
            char *end = strchr(p, ']');
            if (start && end && current_id >= 0)
            {
                start++;
                char *token = strtok(start, ",");
                while (token && token < end)
                {
                    float grade;
                    if (sscanf(token, "%f", &grade) == 1)
                    {
                        add_grade_to_student(current_id, grade);
                    }
                    token = strtok(NULL, ",");
                }
            }
        }
        /* End of student object */
        else if (strstr(p, "}") && in_student)
        {
            /* Reset for next student */
            current_id = -1;
            current_name[0] = '\0';
            in_student = false;
        }
    }

    fclose(fp);
    return true;
}

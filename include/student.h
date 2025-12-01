#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    float *grades;
    int gradeCount;
    float average;
} Student;

void init_students(void);
void free_students(void);

bool add_student(int id, const char *name);
bool delete_student(int id);
bool update_student_name(int id, const char *newname);

bool add_grade_to_student(int id, float grade);
float sum_grades_recursive(const float *grades, int n);
void recalc_average(Student *s);

void display_all_students(void);
void display_grade_matrix(void);

typedef enum { SORT_BY_ID = 0, SORT_BY_AVG = 1 } SortKey;
typedef enum { BUBBLE=1, INSERTION=2, MERGE=3 } SortMethod;

void sort_students(SortMethod method, SortKey key);

int binary_search_by_id_recursive(int target_id, int left, int right); 

bool class_highest_lowest(float *highest, int *h_idx, float *lowest, int *l_idx);

int students_count(void);
Student* get_student_by_index(int idx); 

#endif

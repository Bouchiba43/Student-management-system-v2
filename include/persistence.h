#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdbool.h>

#define DATA_FILE "data/students.json"

bool save_students_to_file(void);

bool load_students_from_file(void);

#endif

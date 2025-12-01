CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -Iinclude
BINDIR = bin
SRCDIR = src

# Source files in their respective directories
MAIN_SRC = $(SRCDIR)/main.c
MENU_SRC = $(SRCDIR)/menu/menu.c
STUDENT_SRC = $(SRCDIR)/student/student.c
PERSISTENCE_SRC = $(SRCDIR)/persistence/persistence.c
UTILS_SRC = $(SRCDIR)/utils/utils.c

# Object files
OBJS = $(BINDIR)/main.o $(BINDIR)/menu.o $(BINDIR)/student.o $(BINDIR)/persistence.o $(BINDIR)/utils.o
TARGET = $(BINDIR)/sms

.PHONY: all clean

all: $(BINDIR) $(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/main.o: $(MAIN_SRC) include/student.h include/menu.h include/persistence.h
	$(CC) $(CFLAGS) -c $(MAIN_SRC) -o $@

$(BINDIR)/menu.o: $(MENU_SRC) include/menu.h include/student.h include/persistence.h include/utils.h
	$(CC) $(CFLAGS) -c $(MENU_SRC) -o $@

$(BINDIR)/student.o: $(STUDENT_SRC) include/student.h
	$(CC) $(CFLAGS) -c $(STUDENT_SRC) -o $@

$(BINDIR)/persistence.o: $(PERSISTENCE_SRC) include/persistence.h include/student.h
	$(CC) $(CFLAGS) -c $(PERSISTENCE_SRC) -o $@

$(BINDIR)/utils.o: $(UTILS_SRC) include/utils.h
	$(CC) $(CFLAGS) -c $(UTILS_SRC) -o $@

clean:
	rm -rf $(BINDIR)

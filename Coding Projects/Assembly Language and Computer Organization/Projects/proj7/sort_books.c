#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TITLE_LEN       32
#define AUTHOR_LEN      20
#define SUBJECT_LEN     10

struct book {
    char title[TITLE_LEN + 1];
    char author[AUTHOR_LEN + 1];    /* first author */
    char subject[SUBJECT_LEN + 1];  /* Nonfiction, Fantasy, Mystery, ... */
    unsigned int year;              /* year of e-book release */
};

void* book1;
void* book2;
void print_books(struct book books[], int numBooks);
void sort_books(struct book books[], int numBooks);

extern int compare (const void* book1, const void* book2);

void sort_books(struct book books[], int numBooks) {

    int i, j, min;
    int cmpResult;

    qsort(books, numBooks, size, compare);

}

void print_books(struct book books[], int numBooks) {

  int i = 0;

  for (i = 0; i < numBooks; i++) {
    printf(books[i].title);
    printf(books[i].author);
    printf(books[i].subject);
  }
}

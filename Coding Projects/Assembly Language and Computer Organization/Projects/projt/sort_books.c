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


void print_books(struct book books[], int numBooks);
void sort_books(struct book books[], int numBooks);

extern int bookcmp (const void*, const void*);

void sort_books(struct book books[], int numBooks) {

    qsort(books, numBooks, sizeof(books[0]), bookcmp);

}

void print_books(struct book books[], int numBooks) {

  int i = 0;

  for (i = 0; i < numBooks; i++) {
    printf("%s,%s,%s,%d\n", books[i].title, books[i].author, books[i].subject, books[i].year);
  }
}

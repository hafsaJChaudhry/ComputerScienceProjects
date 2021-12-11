#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Following is straight from the project description
 */
#define TITLE_LEN       32
#define AUTHOR_LEN      20
#define SUBJECT_LEN     10

struct book {
  char title[TITLE_LEN + 1];
  char author[AUTHOR_LEN + 1];    /* first author */
  char subject[SUBJECT_LEN + 1];  /* Nonfiction, Fantasy, Mystery, ... */
  unsigned int year;              /* year of e-book release */
};

int main(int argc, char **argv) {

  struct book book1;

  return 0;

}

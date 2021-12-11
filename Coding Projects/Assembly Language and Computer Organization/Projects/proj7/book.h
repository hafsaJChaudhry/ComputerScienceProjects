#ifndef BOOK_H
#define BOOK_H

#define TITLE_LEN       32
#define AUTHOR_LEN      20
#define SUBJECT_LEN     10

struct book {
    char title[TITLE_LEN + 1];
    char author[AUTHOR_LEN + 1];
    char subject[SUBJECT_LEN + 1];
    unsigned int year;
};
#endif

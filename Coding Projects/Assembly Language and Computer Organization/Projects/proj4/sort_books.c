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

struct book* book1;
struct book* book2;
void print_books(struct book books[], int numBooks);
void sort_books(struct book books[], int numBooks);

/*
 * Declarations for functions that are defined in other files
 */

/*STUB: ADD EXTERNAL FUNCTION DECLARATIONS HERE*/
extern int bookcmp(void);

/*
 * Declarations for global variables that need to be accessed
 * from other files
 */



/*STUB: ADD DECLARATIONS FOR YOUR OWN GLOBAL VARIABLES HERE*/


#define MAX_BOOKS 100

int main(int argc, char **argv) {
    struct book books[MAX_BOOKS];
    int numBooks;

    /* STUB: ADD CODE HERE TO READ A RECORD AT A TIME FROM STDIN USING scanf()
       UNTIL IT RETURNS EOF
 */
    int i;
    int numFields;
    for (i = 0; i < MAX_BOOKS; i++) {
	/* Sample line: "Breaking Point, Pamela Clare, Romance, 2011" */

	/* I decided to give you the scanf() format string; note that
	 * for the string fields, it uses the conversion spec "%##[^,]",
	 * where "##" is an actual number. This says to read up to a
	 * maximum of ## characters (not counting the null terminator!),
	 * stopping at the first ','  Also note that the first field spec--
	 * the title field--specifies 80 chars.  The title field is NOT
	 * that large, so you need to read it into a temporary buffer first.
	 * All the other fields should be read directly into the struct book's
	 * members.
	 */
      unsigned int* d;
      char title1[81], author1[20], subject1[10];

      numFields = scanf("%80[^,], %20[^,], %10[^,], %u \n", title1, author1, subject1, d);
      /*STUB: REST OF ARGS TO scanf()???);*/

      if(numFields == 4) {

	title1[30] = '\0';
	strncpy(books[i].title,title1, 30);
	strcpy(books[i].author, author1);
	strcpy(books[i].subject,subject1);
	books[i].year = d;
	
      }

      if (numFields == EOF) {
	numBooks = i;
	break;
      }
      
	/* Now, process the record you just read.
	 * First, confirm that you got all the fields you needed (scanf()
	 * returns the actual number of fields matched).
	 * Then, post-process title (see project spec for reason)
	 */

	/*STUB: VERIFY AND PROCESS THE RECORD YOU JUST READ*/
	
    }
    
    /* Following assumes you stored actual number of books read into
     * var numBooks
     */
    sort_books(books, numBooks);
    printf("Hello, World!");
    print_books(books, numBooks);

    exit(1);
}

/*
 * sort_books(): receives an array of struct book's, of length
 * numBooks.  Sorts the array in-place (i.e., actually modifies
 * the elements of the array).
 *
 * This is almost exactly what was given in the pseudocode in
 * the project spec--need to replace STUBS with real code
 */
void sort_books(struct book books[], int numBooks) {
    int i, j, min;
    int cmpResult;
    /* Make pointers point to something*/

    for (i = 0; i < numBooks - 1; i++) {
	min = i;
	for (j = i + 1; j < numBooks; j++) {

	    /* Copy pointers to the two books to be compared into the
	     * global variables book1 and book2 for bookcmp() to see
	     */
	    /*STUB: ADD STUFF HERE
	      extern int bookcmp(void); ////////////////////////////////////// FIND WHERE THIS GOES
	    cmpResult = bookcmp();*/
	    /* bookcmp returns result in register EAX--above saves
	     * it into cmpResult */
	  book1 = &books[i];
	  book2 = &books[j];

	  cmpResult = bookcmp();
	  if (cmpResult == 1) {
		min = j;
		}
	}
	if (min != i) {
	  /*STUB: SWAP books[i], books[min];*/
	  
	}
    }
}

void print_books(struct book books[], int numBooks) {

  /*STUB: ADD CODE HERE TO OUTPUT LIST OF BOOKS*/
  int i = 0;

  for (i = 0; i < numBooks; i++) {
    printf(books[i].title);
    printf(books[i].author);
    printf(books[i].subject);
  }


}

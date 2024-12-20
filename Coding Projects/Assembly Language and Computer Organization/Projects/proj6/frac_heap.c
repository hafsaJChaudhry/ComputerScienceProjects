#include <stdio.h>
#include <stdlib.h>
#include "frac_heap.h"
#define GLOBALSIZE 5

fraction_block* last;
fraction_block* master;
fraction_block* freeBlock;

/*
 * init_heap():
 * must be called once by the program using your
 * functions before calls to any other functions are made. This
 * allows you to set up any housekeeping needed for your memory
 * allocator. For example, this is when you can initialize
 * your free block list.
 */
void init_heap(void) {

  master = NULL;

}

/*
 * new_frac():
 * must return a pointer to fractions.
 * It should be an item taken from the list of free blocks.
 * (Don't forget to remove it from the list of free blocks!)
 * If the free list is empty, you need to get more using malloc()
 * (Number of blocks to malloc each time is specified in the project
 * description)
 */

fraction *new_frac(void) {

  //fraction_block* freeBlock;

  if (master == NULL) {

    freeBlock = (fraction_block*) malloc (sizeof(fraction_block));

    if(freeBlock == NULL) {
      
      printf("Underflow_error");
      exit(1);
      
    } else {
      
      int i;
      for(i = 0; i < GLOBALSIZE; i++) {
	
	if(i < GLOBALSIZE - 1) {
	  
	  freeBlock[i].frac.sign = 0;
	  freeBlock[i].frac.numerator = 0;
	  freeBlock[i].frac.denominator = 0;
	  freeBlock[i].next = &freeBlock[i + 1];
	  
	} else {
	  
	  freeBlock[i].frac.sign = 0;
	  freeBlock[i].frac.numerator = 0;
	  freeBlock[i].frac.denominator = 0;
	  freeBlock[i].next = NULL;

	}
      }
      
      
      
      master = &freeBlock[0];
      master -> next = freeBlock[0].next;
      printf("called malloc() (%d): returned %p\n)", sizeof(fraction_block), master);
      fraction* loc;
      loc = &master -> frac;    
      
      
    }    /*********************************************************************************/
  } else {
    
    int p;
    for(p = 0; p < GLOBALSIZE; p++) {

      if(p < GLOBALSIZE - 1) {

	freeBlock[p].frac.sign = 0;
	freeBlock[p].frac.numerator = 0;
	freeBlock[p].frac.denominator = 0;
	freeBlock[p].next = &freeBlock[p + 1];

      } else {

	freeBlock[p].frac.sign = 0;
	freeBlock[p].frac.numerator = 0;
	freeBlock[p].frac.denominator = 0;
	freeBlock[p].next = NULL;

      }
    }
    
    fraction_block* temp = master;
    temp -> next = master -> next;

    int found = 0;
    while(found == 0) {

      if (temp -> next == NULL && (temp -> frac.sign != 0 || temp -> frac.numerator != 0 || temp -> frac.denominator != 0)) {

	dump_heap();

	fraction_block* temp1 = &freeBlock[4];
	fraction_block* temp2 = temp;

	printf("temp1 located at: (%p), and temp2 is located at(%p)\n", temp1, temp2);

	printf("Overflow_error");
	exit(1);

      } else if (temp -> next != NULL && (temp -> frac.numerator == 0 && temp -> frac.denominator == 0)) {

	fraction* temp1 = &(temp -> frac);
	return temp1;

      } else {

	temp = temp -> next;

      }

    }

  }

}

/*
 * del_frac():
 * takes a pointer to a fraction and adds that item to the free block list.
 * The programmer using your functions promises to never use that item again,
 * unless the item is given to her/him by a subsequent call to new_frac().
 */
void del_frac(fraction *frac) {
  
  free(frac);
  fraction_block *fbp;
  fbp = (fraction_block *) frac;

}

/*
 * dump_heap():
 * For debugging/diagnostic purposes.
 * It should print out the entire contents of the free list,
 * printing out the address for each item.
 */
void dump_heap(void) {

  printf("*** BEGIN HEAP DUMP ***\n");
  fraction_block* temp;
  int i;

  for(i = 0; i < GLOBALSIZE; i++) {
    temp = &freeBlock[i];
    if(temp -> frac.numerator == 0 && temp -> frac.denominator == 0) {

      printf("%p\n",temp);

    }

  }

  printf("*** END HEAP DUMP ***\n");
  
}

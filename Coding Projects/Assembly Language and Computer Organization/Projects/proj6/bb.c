#include <stdio.h>
#include <stdlib.h>
#include "frac_heap.h"
#define GLOBALSIZE 5

fraction_block* master;
fraction_block* freeBlock;
fraction_block* last;

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

  /*

  if(master == NULL) {
    int i;
    for(i = 0; i < GLOBALSIZE; i++) {

      if(i < GLOBALSIZE - 1) {

        freeBlocks[i].frac.sign = 0;
        freeBlocks[i].frac.numerator = 0;
        freeBlocks[i].frac.denominator = 0;
        freeBlocks[i].next = &freeBlocks[i + 1];

      } else {

        freeBlocks[i].frac.sign = 0;
        freeBlocks[i].frac.numerator = 0;
        freeBlocks[i].frac.denominator = 0;
        freeBlocks[i].next = NULL;

      }


    }


    /* Set master pointer to the first block and the next should be the second
    master = &freeBlocks[0];
    master -> next = &freeBlocks[1];
    
  }    

  /*
  int x;
  for(x = 0; x < GLOBALSIZE; x ++){

    if(x < GLOBALSIZE - 1) {
      heap[x].denominator = x + 1;
      freeBlocks[x].frac.denominator = x + 1;
    } else {
      heap[x].denominator = -1;
      freeBlocks[x].frac.denominator = -1;
    }
  }

  */

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
      last = &freeBlock[1];
      
      printf("called malloc() (%d): returned %p\n)", sizeof(fraction_block), master);
      fraction* loc;
      loc = &master -> frac;    
      
      
    }    
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
  
  
    fraction_block* temp1 = &freeBlock[0];
    printf("free1 located at: (%p), and freeBlock[0] is located at(%p)\n", free1, temp1);
    master -> next = &freeBlock[1];
    
    fraction_block* temp = master -> next;
    temp1 = &freeBlock[1];

    printf("Temp located at: (%p), and freeBlock[1] is located at(%p)\n", temp, temp1);
    
    fraction* loc;
    int found = 0;
    while(found == 0) {

      if (temp -> next == NULL || temp -> frac.sign != 0 || temp -> frac.numerator != 0 || temp -> frac.denominator != 0) {

	dump_heap();
	printf("Overflow_error");
	exit(1);

      } else if(temp -> next != NULL) {
	printf("YEET");


	found = 1;
	loc = &temp -> frac;
	return loc;

      } else {

	temp = temp -> next;

      }

    }

  }

  /*
  fraction_block* temp = master;
  int index = 0;
  int found = 0;
  int done = 0;
  /*While you are not at the end or you are standing on a used fraction
  while((done == 0) && (found == 0)) {

    if(heap[index].sign == 0 && heap[index].numerator == 0) {

    if(temp -> frac.sign != 0 || temp -> frac.numerator != 0 || temp -> frac.denominator != 0 && temp -> next != NULL) {

      
      temp = temp -> next;



    } else if (temp -> frac.sign == 0 || temp -> frac.numerator == 0 || temp -> frac.denominator == 0) {

      fraction* newFrac = temp -> frac;

      found = 1;
      done = 1;;

      return newFrac;
    } else if (temp -> frac.sign != 0 || temp -> frac.numerator != 0 || temp -> frac.denominator != 0 && temp -> next == NULL) {



    }


    } else {


    }
    index++;
  }

  return temp;
  }
  */
}

/*
 * del_frac():
 * takes a pointer to a fraction and adds that item to the free block list.
 * The programmer using your functions promises to never use that item again,
 * unless the item is given to her/him by a subsequent call to new_frac().
 */
void del_frac(fraction *frac) {

  /*
  fraction* limit = &heap[GLOBALSIZE - 1];
  int validity = limit - frac;
  if(validity < GLOBALSIZE && validity >= 0) {


    limit = frac;
    frac -> sign = 0;
    frac -> numerator = 0;
    frac -> denominator = 0;
    validity = GLOBALSIZE - validity;
    /*While you're not at the end and have not been set to a specific number 
    while(frac -> denominator != -1 && frac -> denominator == 0) {
      limit++;
      validity++;
      if(limit -> sign == 0 && limit -> numerator == 0) {
        frac -> denominator = validity - 1;
      }
      if(limit -> denominator == -1) {
        frac -> denominator = -1;
      }
    }

  }
  int o;
  for(o = 0; o < GLOBALSIZE; o++) {

    freeBlocks[o].frac = *&heap[o];

  }

  */

}

/*
 * dump_heap():
 * For debugging/diagnostic purposes.
 * It should print out the entire contents of the free list,
 * printing out the address for each item.
 */
void dump_heap(void) {

  /*
  int x;
  for(x = 0; x < GLOBALSIZE; x ++){
    fraction* temp = &heap[x];
    printf("*** BEGIN HEAP DUMP ***\n");
    /*printf("%d", x);
    printf(":\t");
    printf("%d\t%d\t%d\n",temp->sign, temp->numerator, temp->denominator);
    printf("%x\n", &heap[x]);
    printf("%x\n", &*temp);
    printf("*** END HEAP DUMP ***\n");
  }
  */

  printf("*** BEGIN HEAP DUMP ***\n");
  fraction_block* temp;
  int i;
  for(i = 0; i < GLOBALSIZE; i++) {
    temp = &freeBlock[i];
    if(temp -> frac.numerator != 0|| temp -> frac.denominator != 0 || temp -> frac.sign != 0) {

      printf("%p\n",temp);
    
    }

  }

  printf("*** END HEAP DUMP ***\n");

}

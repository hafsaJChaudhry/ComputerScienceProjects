// File frac_heap.c
#include <stdio.h>
#include <stdlib.h>
#include "frac_heap.h"

#define GLOBALSIZE 20

fraction heap[GLOBALSIZE] = {0};
block freeBlocks[GLOBALSIZE] = {0};
int startingBlock = 0;

void init_heap(){
  int x;
  for(x = 0; x < GLOBALSIZE; x ++){    
    block *currBlock = &freeBlocks[x];
    currBlock->isFree = 1;  
  
    if(x < 19) {
      heap[x].denominator = x + 1;
    } else {
      heap[x].denominator = -1;
    }
  }

  

}
void dump_heap(){
  int x;
  for(x = 0; x < GLOBALSIZE; x ++){
    fraction* temp = &heap[x];
    printf("%d", x);
    printf(":\t");
    printf("%d\t%d\t%d\n",temp->sign, temp->numerator, temp->denominator);
  }   

}

fraction* new_frac(){

  fraction* temp = &heap[0];
  int index = 0;
  int found = 0;
  int location = 0;
  /*While you are not at the end or you are standing on a used fraction*/
  while((index < 20) && (found == 0)) {
    
    if(heap[index].sign == 0 && heap[index].numerator == 0) {

      found = 1;
      temp = &heap[index];
      location = index; /* This is the location in the list that this fraction is made for*/

    }
    index++;
  }

  return temp;

} 

void del_frac(fraction* x) {

  fraction* limit = &heap[GLOBALSIZE - 1];
  int validity = limit - x;
  if(validity < GLOBALSIZE && validity >= 0) {


    limit = x;
    x -> sign = 0;
    x -> numerator = 0;
    x -> denominator = 0;
    validity = GLOBALSIZE - validity;
    /*While you're not at the end and have not been set to a specific number */
    while(x -> denominator != -1 && x -> denominator == 0) {
      limit++;
      validity++;
      if(limit -> sign == 0 && limit -> numerator == 0) {
	x -> denominator = validity - 1;
      }
      if(limit -> denominator == -1) {
	x -> denominator = -1;
      }
    }
    
  }
  int o;
  for(o = 0; o < GLOBALSIZE; o++) {

    freeBlocks[o].isFree = heap[o].denominator;

  }

}

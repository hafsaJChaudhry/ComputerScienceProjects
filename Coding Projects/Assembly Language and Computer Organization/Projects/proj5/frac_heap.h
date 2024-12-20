/*frac_heap.h*/

/*typedefs*/

typedef struct
{
  signed char sign;
  unsigned int denominator;
  unsigned int numerator;
}fraction;

typedef struct
{
  unsigned int isFree;
}block;

void dump_heap();
void init_heap();
/*fraction init_frac(signed char plusMin, unsigned int num, unsigned int den);*/
fraction* new_frac(); 
fraction* init_frac(signed char s, unsigned int n, unsigned int d);
void del_frac(fraction* x);

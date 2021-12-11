/* time_matmul.c  fine how large matrix multiple can be */
/*                check that time increases order n^3   */
/*                doubling N takes 8 times as long      */
/*                can be days for 10,000 by 10,000      */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double *a; /* input matrix */
static double *b; /* input matrix */
static double *c; /* result matrix */

int main(int argc, char *argv[])
{
  int N;
  int i,j,k;
  double t1, t2;

  for(N=100; N<=3200; N=N*2) /* N<=3200 is OK, takes minutes */
  {
    printf("multiply N by N matrices, N=%d \n", N);
    a = (double *)malloc(N*N*sizeof(double));
    b = (double *)malloc(N*N*sizeof(double));
    c = (double *)malloc(N*N*sizeof(double));
    for(i=0; i<N; i++)
    {
      for(j=0; j<N; j++)
      {
        a[i*N+j] = (double)i;
        b[i*N+j] = (double)j;
      }
    }
    printf("initialized \n");
    t1 = (double)clock()/(double)CLOCKS_PER_SEC;

    for(i=0; i<N; i++)
    {
      for(j=0; j<N; j++)
      {
        c[i*N+j] = 0.0;
        for(k=0; k<N; k++)
          c[i*N+j] = c[i*N+j] + a[i*N+k]*b[k*N+j]; /* most time spent here! */
      }
    }
    t2 = (double)clock()/(double)CLOCKS_PER_SEC;
    printf("N=%d, c=%g, raw time=%g seconds\n", N, c[5], t2-t1);
    t2 = 1.0e9*(t2-t1)/((double)N*(double)N*(double)N);
    printf("order N^3 normalized time=%g \n\n", t2);
    free(a);
    free(b);
    free(c);
  }
  return 0;
} /* end time_matmul.c */

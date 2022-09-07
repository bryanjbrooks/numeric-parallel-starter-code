// The series used for computation of the exponential exp(x) is defined well by a Taylor Series
//
// https://en.wikipedia.org/wiki/Taylor_seriesa
//
// For sin(x), see https://people.math.sc.edu/girardi/m142/handouts/10sTaylorPolySeries.pdf
//
// The series is simply x - (x*x*x)/3! + (x*x*x*x*x)/5! + ...
//
//  Note: n! is simple 1*2*3*4*5...(n-1)*n
//
//  Note - be careful with DOUBLE range, smallest decimal, and inf
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <math.h>

int main(int argc, char* argv[])
{
  int thread_count;
  int iterations=0, idx=0, cnt=0;
  double sum, term;
  double x;

  if (argc < 4)
  {
    printf ( "usage: expseries x <number threads> <iterations> \n " );
    exit(-1);
  }
  else
  {
    sscanf(argv[1], "%lf", &x);
    sscanf(argv[2], "%d", &thread_count);
    sscanf(argv[3], "%d", &iterations);
  }

  term=x; sum=x;

  printf("DBL_EPSILON = %le\n", DBL_EPSILON);

// The goal of a series computation is to avoid use of the math library completely
// and to use only basic arithmetic to compute transcendental and polynomial
// functions.
//
#pragma omp parallel for reduction(+:sum) num_threads(thread_count)
  for(idx=2; idx < iterations*2; idx=idx+2) 
  {
    term = -term * (x*x)/ ((double)(idx+1)*idx);

    sum += term; cnt++;
  }

  printf ( "C math library sin(x)  =%15.14lf\n", sin(x));
  printf ( "Series sin(x) =%15.14lf, iterations=%d, error=%le\n", sum, cnt, fabs(sum - sin(x)));

}

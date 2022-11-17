// This is the simplest brute force stepping method possible to find a root
// on an interval with a fixed step size equal to error tolerance.
//
// It simply detects a sign change and declares the root to be half way between
// the step before the sign change and the step after and quits.
//
// Inspired by questions from Section 02 CSCI 551, 2022
//
// Why not just search by making small steps for a zero crossing.  This definitely works, but
// does require small step sizes, potentially many steps to search a large interval, and error
// will simply be stepsize / 2.0
//
// Makes an intersting comparison to Newton, Bisection, and Regula Falsi, all which base stepping
// on an estimate of where the root is on an interval.
//
// Generally, Newton should converge fastest, followed by Regula Falsi, then Bisection, and the Bruteroot will
// get the root stepping in one direction looking for a simple sign change.
//
#include <stdio.h>
#include <math.h>
#include <float.h>

// model a function with desmos for example to "see" roots
// 
// https://www.desmos.com/calculator
double f(double x)
{
    //return x*log10(x) - 1.2;

    //return cos(x*x);

    // As can be seen with Desmos, this example equation has roots near: -2.851, 4.758, and 7.792
    return (-(x*x*x) + 9.7*(x*x) -1.3*x -105.7);
}

int main(void)
{
    int itr, maxitr, rootfound=0;
    double max_error=0.0;
    double step=2.0*DBL_EPSILON, x0, x1, xf, sign, start;

    printf("\nEnter x0 start, xf end, step size and maximum iterations\n");
    scanf("%lf %lf %lf %d", &x0, &xf, &step, &maxitr);
    start=x0;

    printf("Will step at %lf from %lf to %lf to find root\n", step, x0, (maxitr*step));

    do
    {
        for (itr=1; itr<=maxitr && (x0 < xf); itr++)
        {

            x1=x0+step;

            // Any negative value x positive or vice versa will result in a sign less than zero - a crossing
            sign = f(x0) * f(x1);

            if(sign < 0.0)
            {
                printf("Sign change at %3d, estimated root at %20.15lf, f[root]=%20.15lf\n", itr, (x1+x0)/2.0, f((x1+x0)/2.0));
                if(fabs(f((x1+x0)/2.0)) > max_error) max_error=fabs(f((x1+x0)/2.0));
                rootfound++;

                // We can exit on first root (zero crossing found) or keep looking
                break;
            }

            x0=x1;
        }

        // Start search for next root one step beyond last one found
        x0=x1;
        itr=1;

    } while((x0 <  xf) && itr < maxitr);

    if(!rootfound)
    {
        printf("After %d iterations: No solution (zero crossing) found on interval %lf to %lf\n", maxitr, start, x0);
    }
    else
    {
        printf("%d roots found on interval %lf to %lf, with max error=%lf\n", rootfound, start, x0, max_error);
    }

    return 1;
}

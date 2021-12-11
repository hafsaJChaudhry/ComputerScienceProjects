HW3 , Integration 25 pts

1. Compare two methods of integrating  sin(x) from 0 to 1
Take as the exact solution  -cos(1.0)+cos(0.0) = 1.0-cos(1.0)
Print out number of points, your integration computed value, and
your_computed_value - exact_solution the error
Remember lecture 7.

1.a) Use the trapezoidal method with 16, 32, 64 and 128 points.
   Put this in a loop  e.g. in C  for(n=16; n<=128; n=n*2)
   Note how the error decreases as n increases.
   h = 1/n
   your_value = h * ((sin(0)+sin(1.0))/2 + sum i=1..n-1 sin(i*h))

  trapezoide.py sample integration
  trapezoide.java sample integration
   
1.b) Use the Gauss Legendre method with 8 and 16 points.
   Copy the function  gaulegf  from your choice of language
   or convert to your choice of language. Be sure to keep
   the #undef abs and #define abs in "C" code.

   double x[17], w[17]
   n=8   then again for n=16
   gaulegf(0.0, 1.0, x, w, n)
   your_value = 0.0
   for(j=1; j<=n; j++)
      your_value = your_value + w[j]*sin(x[j])


   Note the significantly smaller error than for trapezoid.

gaulegf.py sample integration
gaulegf.java sample integration


2. Write a small program in the language of your choice to
   numerically compute, to at least 3 significant digits,
   the area that is outside Circle 1 and inside both Circle 2
   and Circle 3,e.g. inside if x^2+y^2<9, outside if x^2+Y^2>9
 
   Circle 1 center at (2,2) radius 1   (x-2)^2+(y-2)^2=1^2
   Circle 2 center at (0,2) radius 2    x^2+(y-2)^2=2^2
   Circle 3 center at (0,0) radius 3    x^2+y^2=3^2

   Method: count the dots in the area and multiply count by the
   area of a square. Run with a grid 0.1, 0.01, and  0.001,
   print count of dots and area for each grid size,
   to see if your computation is converging. Obviously, if the
   left hand side of the equation is larger than the right
   hand side of the equation, the point is outside the circle.
   Obviously 0.1 is not exactly representable, thus "=" is useless.
   ![image](https://user-images.githubusercontent.com/60797103/145663098-ddb7da8e-4e9d-40f1-b8e4-b1b4378c99f3.png)

submit cs455 hw3 your-source your-output    for 1a, 1b and 2

your-source  and  your-output  ust be  text files.
Do not submit  a.out  that is a binary file.


   (Note: in problem 2, if you were given a function z=F(x,y) and
    needed to compute the volume over the area, you would
    probably combine part of problem 1 with problem 2.)

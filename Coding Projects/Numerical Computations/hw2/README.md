In a language of your choice, write a program that
does a least square fit of the thrust data used in Homework 1.
Use your thrust data  x =  0.0  0.1   0.2  ...  1.8 1.9
                      y =  0.0  6.0  14.1  ...  4.5 0.0
Be sure time 0.0 has value 0.0, time 1.9 has value 0.0

Do the least square fit with 3, 4, ... , 17 degree polynomials.
Compute the maximum error, the average error and RMS error
for each fit. If convenient, look at the plots of your fit
and compare to the input.

Print out your data. Remove excess printout, print:
Print out your polynomial order for each degree 3..17 and
print out maximum, average and RMS error for each.
Lecture 1 shows how to compute error
copy whatever you need from Lecture 4, Least square fit
Note: Python polyfit does not do a least square fit, no "order" parameter.
Many languages covered in Lecture 4, a must read.

If using python, see test_lsfit.py3, lsfit.py3, peval.py3  from download
OK to use my files.

submit your source code file(s) and your output file

submit cs455 hw2 your-source your-output

My MatLab errors looked like:hw2_m.out
   (with a lot of complaints from MatLab!)

UGH! Matlab has started blabbing errors. I will ignore them.

Plotting three points between each starting point in MatLab, n=19, gave:
![image](https://user-images.githubusercontent.com/60797103/145663066-f63863a3-ab9c-42f8-a46d-13eefc42fcf1.png)
The steps are typically:
Have the thrust data in X array, time values.
Have the thrust data in Y array, newton values.
Build the least square simultaneous equations, n=3 first

Given a value of Y for each value of X, 
  Y_approximate =  C0 + C1 * X + C2 * X^2 +  C3 * X^3
Then the linear equations would be:

| SUM(1  *1) SUM(1  *X) SUM(1  *X^2) SUM(1  *X^3) | | C0 |   | SUM(1  *Y) |
| SUM(X  *1) SUM(X  *X) SUM(X  *X^2) SUM(X  *X^3) | | C1 |   | SUM(X  *Y) |
| SUM(X^2*1) SUM(X^2*X) SUM(X^2*X^2) SUM(X^2*X^3) |x| C2 | = | SUM(X^2*Y) |
| SUM(X^3*1) SUM(X^3*X) SUM(X^3*X^2) SUM(X^3*X^3) | | C3 |   | SUM(X^3*Y) |

Note that index [i][j] of the matrix has SUM(X^i*X^j))
for i=0 to 3, j=0 to 3   X^0 = 1   X^1 = X

Solve the simultaneous equations for C0, C1, C2, C3
using some version of "simeq". Remember 4 C values for power 3.
This is equivalent to polyfit.

Compute errors by using "peval" with the coefficients
for each X and subtract the corresponding Y.
Save maximum error, compute average and RMS error.
Print.

Repeat, advancing n to 4, 5, ..., 17

put in time and thrust data from homework 1
k=20          number of data points
x=time[i]
y=thrust[i]
Do some least square polyfit on x,y get C values
use  err_each_term = polyval(pwr, x, C)   all 20
compute max, avg, rms error for each of 20  err_each_term-y[i]

Use lsfit.py3,peval.py3 not polyfit,polyval if you are better with Python 3.
You must understand lecture 4.

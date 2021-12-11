HW6, Timing 25 points
 
Compile and run the program time_matmul.c
on some computer. You may use other languages and MatLab.
Time the matrix multiply for 100 by 100, 200 by 200, 400 by 400 etc
and record the actual time and a normalized time. It is OK to stop
when a single size takes over 10 minutes.

The normalized time is some constant multiplied by actual time divided by N^3.
Matrix multiply is order N cubed, thus the time goes up by a factor
of 8 each time N doubles. Note that the small 100 by 100 runs very fast.
As the matrix gets larger, the time increases by more than a factor of 8.
This is because of cache performance verses memory bandwidth.

If you can not run "C", a Java version, a little faster, is:
time_matmul.java

If you want a really slow version, run the Python:
time_matmul.py
Actually, you should be using  numpy  for numerical computation.
time_matmuln.py using numpy

Comment on or explain any large variation in normalized time,
normalized time should be the same for all sizes, for the
language and operating system and machine you ran this homework.

Turn in paper or submit your code (or my code) and your output and comment.

submit cs455 hw6 your-files

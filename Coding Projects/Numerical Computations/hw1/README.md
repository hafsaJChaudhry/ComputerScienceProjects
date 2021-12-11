HW1 , Rocket height 25 points, 5% of grade
Modeling and simulation: Rocket flight, how high?
In a language of your choice, write the program defined in
lecture 2. Test your program and
produce an output file, then  submit both files
on a GL machine:
(Do not compute at time = 0.0, you will get a negative velocity
  due to force of gravity, first Ft should be 6.0, from array,
  keep computing after time = 1.8 with Ft = 0.0 until V < 0.0 after
  about 7 seconds, about 350 meters, your thrust may be different.)

For each time 0.1, 0.2, ... about 7.0 seconds,
print t time, s altitude, v velocity, a acceleration, F net force, m mass 	  
							  
submit cs455 hw1 your-source your-output

print output as a text file, do not submit  a.out  that is a binary file.
Print every dt=0.1, tenth of a second.
All values are in Metric system. Rho = 1.293
							 
The graphics class sees the flight that produces this as a moving flight

![numcomphw1pic](https://user-images.githubusercontent.com/60797103/145663030-2e8ff968-ff4e-44a9-b0c9-af5261577929.png)

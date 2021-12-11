#File: hw3.py
#Author: Muaz Latif
import math

def f(x):
  return math.sin(x)

def trap_int(f, xmin, xmax, nstep): # integrate f(x) from xmin to xmax
  area=(f(xmin)+f(xmax))/2.0
  h = (xmax-xmin)/nstep
  for i in range(1,nstep):
    x = xmin+i*h
    area = area + f(x)

  return area*h # trapezoidal method

def gaulegf(a, b, n):
  x = list(range(n+1)) # x[0] unused
  w = list(range(n+1)) # w[0] unused
  eps = 3.0E-14
  m = int((n+1)/2)
  xm = 0.5*(b+a)
  xl = 0.5*(b-a)
  for i in range(1, int(m+1)):
    z = math.cos(3.141592654*(i-0.25)/(n+0.5))
    while True:
      p1 = 1.0
      p2 = 0.0
      for j in range(1,n+1):
        p3 = p2
        p2 = p1
        p1 = ((2.0*j-1.0)*z*p2-(j-1.0)*p3)/j

      pp = n*(z*p1-p2)/(z*z-1.0)
      z1 = z
      z = z1 - p1/pp
      if abs(z-z1) <= eps:
            break

    x[i] = xm - xl*z
    x[n+1-i] = xm + xl*z
    w[i] = 2.0*xl/((1.0-z*z)*pp*pp)
    w[n+1-i] = w[i]
  return x, w


def main():
	xmin = 0.0
	xmax = 1.0
	x = []
	w = []
	start = 16
	area = 0
	errorNum = xmax - math.cos(xmax)
	print("--------Part 1a--------")
	while(start <= 128):

		area = trap_int(f, xmin, xmax, start)
		error = area - errorNum
		print("n = ", start, " value = ", area, " error:", error)
		start = start*2
	print("-----End of Part 1a-----")
	start = 8
	print("--------Part 1b--------")
	while (start <= 16):
		your_value = 0.0
		x, w = gaulegf(xmin, xmax, start)
		for j in range(1, start+1):
			your_value = your_value + w[j]*(math.sin(x[j]))
		error = your_value - errorNum
		print("n = ", start, "value = ", your_value, "error:", error)
		start = start * 2
	print("-----End of Part 1b-----")
	print("--------Part 2--------")
	start = 10
	while(start <= 1000):
		your_value = 0.0
		for i in range(-3*start, 3*start+1):
			for j in range(-4*start, 4*start):
				x = i/start
				y = j/start
				if((x-2)**2 + (y-2)**2) >= 1 and (x**2 + (y-2)**2) < 4 and (x**2 + y**2) < 9:
					your_value = your_value + 1

		print("n = ", 1/start, "grid count = ", your_value, "area:", your_value/start**2)
		start = start * 10

	print("-----End of Part 2-----")
main()
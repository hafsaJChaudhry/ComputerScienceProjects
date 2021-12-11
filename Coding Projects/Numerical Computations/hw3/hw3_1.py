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
  m = (n+1)/2
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
    print ("HW3 problem 1a and 1b starting")
    xmin = 0.0
    xmax = 1.0
    x = []
    w = []
    nt = 16
    ng = 8
    
    #trap_int below
    while nt <= 128:
        area = trap_int(f, xmin, xmax, nt)
        error = abs(area - (1.0 - math.cos(1.0)))
        print("n =", nt, "computed value:", area, " error:", error)
        nt = nt*2
    
    print("\n---------------\n")
    
    # Gauss Legendre below
    your_value = 0.0
    while ng <= 16:
        x, w = gaulegf(0.0, 1.0, ng)
        
        for j in range(ng-1):
            your_value = your_value + w[j]*(math.sin(x[j]))
        
        error = abs(your_value - (1.0 - math.cos(1.0)))
        print("n =", ng, "computed value:", your_value, "error:", error)
        ng *= 2
    
main()
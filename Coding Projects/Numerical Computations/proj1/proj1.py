from mpmath import *

def main():
    
	mpf.dps = 100 # computation digits
	mp.dps = 100 # default printout digits
	xmin = -1
	xmax = 1
	ymin = -1
	ymax = 1
	z = exp(sin(50.0*x)) + sin(60.0 * exp(y)) + sin(80.0*sin(x)) + sin(sin(70.0*y)) - sin(10.0*(x+y)) + (x*x+y*y)/4.0
	

main()

# def optm(f, xmin, xmax, xf, minstep=1.0e-6, tol=1.0e-6, maxeval=1000):
#   cnt = 0
#   dx = minstep # must be positive, should check
#   xb = xf
#   vb = f(xb)  # vb best = f(xb) to date
#   while 1:
#     x = xb # previous best
#     v = vb # previous best
#     xl = max(xmin,x-dx) # stay inside limits
#     vl = f(xl)
#     if vl < vb:
#       vb = vl
#       xb = xl
#     xh = min(xmax,x+dx) # stay inside limits
#     vh = f(xh)
#     if vh < vb :
#       vb = vh
#       xb = xh
#     cnt += 1
#     if vl < v and v < vh:
#       dx = 2.0*dx  # slope 1
#     if vl < v and vh < v:
#       dx = dx/2.0;  # hump  2 and 4
#     if v < vh and v < vl:
#       dx = dx/2.0;  # valley  3 and 5
#     if vl > v and v > vh:
#       dx = 2.0*dx   # slope 6 

#     # end this iteration, test for finish 
#     close = abs(v-vl)+abs(v-vh)
#     if  dx < minstep or close < tol or cnt > maxeval:
#       return xb, vb, cnt 
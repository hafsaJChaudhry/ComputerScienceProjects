from mpmath import *

def fact(n):
	
	if n == 0:
		return 1
	else:
		return n * fact(n - 1)
def main():
	
	num = fact(52)
	print("52! is: ",num)	
	bignum = fact(52) / (fact(52-5) * fact(5))
	print("52 choose 5 is: ", int(bignum))
main()

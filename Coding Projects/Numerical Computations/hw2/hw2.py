#HW2.py
import numpy as np
import math
def main():
    
    xd = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9]
    yd = [0, 6, 14, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0]


    for n in range(4,19):
        A = [[0.0 for i in range(n)] for j in range(n)]
        Y = [0.0 for i in range(n)]
        pwr = [0.0 for i in range(n+1)]

        for k in range(20):
            y = yd[k]
            x = xd[k]
            pwr[0] = 1.0
            for i in range(1, n+1):
                pwr[i] = pwr[i-1] * x
            for i in range(n):
                for j in range(n):
                    A[i][j] = A[i][j] + (pwr[i] * pwr[j])
                Y[i] = Y[i] + (y * pwr[i])

       
        B = np.array(Y)
        C = np.array(A)
        D = np.linalg.solve(C,B) 


        #Compute the errors
        maxError = 0.0 #Maximum Error
        rmsError = 0.0 #RMS Error
        avgError = 0.0 #Average Error
        

        for i in range(20):
            temp = 0.0
            for j in range(n):
                temp += (D[j] * (xd[i] ** j))
            temp = abs(temp - yd[i])
            if temp > maxError:
                maxError = temp
            rmsError += (temp ** 2)
            avgError += temp
        rmsError /= 20
        rmsError = math.sqrt(rmsError)
        avgError /= 20

        #Print Everything
        print("degree", n - 1, ":\t")
        print("Max Error = ", maxError)
        print("RMS Error = ", rmsError,)
        print("Avg Error = ", avgError)
        for i in range(n):
            print( "C" , i, "=", D[i])
        print("\n")
main()

// hysteresis.c :

#include <stdio.h>
#include <stdlib.h>

const int MAX_DATA = 269450 + 1;  // Number of data points in input data file


// project #4 -- write hysteresis function
int hysteresis(int in)
{

    const int low_limit = 5;
    const int high_limit = 25;
    const int out_high = 40;
    const int out_low = 0;
    int previous_out;

    if(in < low_limit)
    {
        previous_out = out_low;
        return out_low;
    }
    else if(in > high_limit)
    {
        previous_out = out_high;
        return out_high;
    }
    else if(in < high_limit && in > low_limit){return previous_out;}
}


int main(void)
{
    FILE* fp_in;  // pointer to input file pointer
    FILE* fp_out; // output file pointer

    int in = 0;   // input value
    int out = 0;  // output result
    int dpn = 0;  // data point number

    // use data_per_sec with count down timer
    int data_per_sec = 16000;   // There are 16,000 data points per second
    // there are 269,450 data points

    // please update file path names for you computer
    fp_in = fopen("in.txt", "r");
    fp_out = fopen("hyst.txt", "w");

    if (!fp_in){
        printf("In.txt File Open Error.  File was not opened. \n");
    }
    if (!fp_out){
        printf("Out.txt File Open Error.  File was not opened. \n");
    }

    else
    {
        while (!feof(fp_in) && dpn < MAX_DATA)
        {
            fscanf(fp_in, "%d", &in);

            out = hysteresis(in); // high pass filter.

            dpn = dpn + 1;

            fprintf(fp_out, "%d \t %d \n", in, out);
        }
    }
    fclose(fp_in);
    fclose(fp_out);
    printf("Done. \n");
    return 0;
}




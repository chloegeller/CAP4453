// Chloe Geller
// CAP 4453 - Spring 2020
// Assignment 1 - Part A

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];
int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
double ival[256][256], maxival;

int main(argc, argv)
int argc;
char *argv[];
{
    FILE *fo1, *fo2, *fp1, *fo3, *fopen();

    char *foobar;
    char fileName[50];
    
    int i, j, p, q,maxR, sum1, sum2, rows, cols;
    double lowThreshold,highThreshold;
    

    if (argc != 5)
    {
        printf("Please follow the proper syntax: ./a.out <input.pgm> <output> <lowTSH> <highTSH>");
        return -1;
    }

    argc--;
    argv++;
    foobar = *argv;
    fp1 = fopen(foobar, "rb");

    argc--;
    argv++;
    foobar = *argv;

	sprintf(fileName,"%s%s",foobar,"magnitude.pgm");
    fo1 = fopen(fileName, "wb");
    printf("File output: %s\n", fileName);

    sprintf(fileName,"%s%s",foobar,"lowTSH.pgm");
    fo2 = fopen(fileName, "wb");
    printf("File output: %s\n", fileName);

    sprintf(fileName,"%s%s",foobar,"highTSH.pgm");
    fo3 = fopen(fileName, "wb");
    printf("File output: %s\n", fileName);

    argc--;
    argv++;
    foobar = *argv;
    lowThreshold = atof(foobar);

    argc--;
    argv++;
    foobar = *argv;
    highThreshold = atof(foobar);

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            pic[i][j] = getc(fp1);
            pic[i][j] &= 0377;
        }
    }

    maxR = 1;
    
    for (i = maxR; i < 256 - maxR; i++)
    {
        for (j = maxR; j < 256 - maxR; j++)
        {
            sum1 = 0;
            sum2 = 0;

            for (p = -maxR; p <= maxR; p++)
            {
                for (q = -maxR; q <= maxR; q++)
                {
                    sum1 += pic[i + p][j + q] * maskx[p + maxR][q + maxR];
                    sum2 += pic[i + p][j + q] * masky[p + maxR][q + maxR];
                }
                
                outpicx[i][j] = sum1;
                outpicy[i][j] = sum2;
            }
        }
    }

    maxival = 0;
    
    for (i = maxR; i < 256 - maxR; i++)
    {
        for (j = maxR; j < 256 - maxR; j++)
        {
            ival[i][j] = sqrt((double)((outpicx[i][j] * outpicx[i][j]) + 
                                       (outpicy[i][j] * outpicy[i][j])));

            if (ival[i][j] > maxival)
            {
                maxival = ival[i][j];
            }
        }
    }
   
    cols = 256;
    rows = 256;

    fprintf(fo1, "P5\n"); 
    fprintf(fo2, "P5\n");
    fprintf(fo3, "P5\n");

    fprintf(fo1, "%d %d\n", rows, cols);
    fprintf(fo2, "%d %d\n", rows, cols);
    fprintf(fo3, "%d %d\n", rows, cols);

    fprintf(fo1, "255\n");
    fprintf(fo2, "255\n");
    fprintf(fo3, "255\n");

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1, "%c", (char)((int)(ival[i][j])));
        }
    }

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            //ival[i][j] = (ival[i][j] / lowThreshold + maxival) * 255;
            
            if (ival[i][j] > lowThreshold)
            {
                fprintf(fo2,"%c",(char)(255));
            }
            else
            {
                fprintf(fo2,"%c",(char)(0));
            }
        }
    }

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            //ival[i][j] = (ival[i][j] / highThreshold + maxival) * 255;

            if (ival[i][j] > highThreshold)
            {
                fprintf(fo3,"%c", (char)(255));
            }
            else
            {
                 fprintf(fo3,"%c",(char)(0));
            }
        }
    }

    fclose(fo1);
    fclose(fo2); 
    fclose(fo3);
}
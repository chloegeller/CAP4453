// Chloe Geller
// CAP 4453 - Spring 2020
// Assignment 1 - Part B

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PICSIZE 256
#define MAXMASK 100
#define WHITE 255
#define BLACK 0

double xOutput[PICSIZE][PICSIZE];
double yOutput[PICSIZE][PICSIZE];
double xMask[MAXMASK][MAXMASK];
double yMask[MAXMASK][MAXMASK];
double xConv[PICSIZE][PICSIZE];
double yConv[PICSIZE][PICSIZE];
double ival[PICSIZE][PICSIZE];
double mag[PICSIZE][PICSIZE];

int peaksOutput[PICSIZE][PICSIZE];
int finalOutput[PICSIZE][PICSIZE];
int pic[PICSIZE][PICSIZE];

void loop(int i, int j, int loTSH)
{
    int p, q;

    for (p = -1; p <= 1 ; p++)
    {
        for (q = -1; q <= 1; q++)
        {
            if (peaksOutput[i + p][j + q] == WHITE)
            {
                peaksOutput[i + p][j + q] = BLACK;
                finalOutput[i + p][j + q] = WHITE;
                loop(i + p, j + q, loTSH);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    char *foobar, fileName[50];

    FILE  *fo1, *fo2, *fo3, *fp1, *fopen();

    double  maskValX, maskValY, xSum, ySum, sigma = 1.0;
    double maxival, percentage, slope, area;

    int i, j, p, q, tsh, maxR, xCent, yCent, sum;
    int rows = PICSIZE, cols = PICSIZE, hiTSH, loTSH, histogram[PICSIZE];

    printf("Please enter your desired value for a percentage:");
    scanf("%lf", &percentage);

    argc--; 
    argv++;
    foobar = *argv;
    fp1 = fopen(foobar,"rb");

    argc--;
    argv++;
    foobar = *argv;
    sprintf(fileName,"%s%s",foobar,"magnitude.pgm");
    fo1 = fopen(fileName,"wb");
    printf("File output: %s\n", fileName);

    argc--;
    argv++;
    foobar = *argv;
    sprintf(fileName,"%s%s",foobar,"peaks.pgm");
    fo2 = fopen(fileName,"wb");
    printf("File output: %s\n", fileName);

    argc--;
    argv++;
    foobar = *argv;
    sprintf(fileName,"%s%s",foobar,"final.pgm");
    fo3 = fopen(fileName, "wb");
    printf("File output: %s\n", fileName);

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            pic[i][j] = getc(fp1);
        }
    }

    maxR = (int)(sigma * 3);
    xCent = (MAXMASK / 2);
    yCent = (MAXMASK / 2);

    for (i = 0; i < PICSIZE; i++)
    {
        histogram[i] = BLACK;
    }

    for (p = -maxR; p <= maxR; p++)
    {
        for (q = -maxR; q <= maxR; q++)
        {
            maskValX = (-q / (sigma * sigma)) * ((exp(-1 * (((p * p) + 
                       (q * q)) / (2 * (sigma * sigma))))));
            maskValY = (-p / (sigma * sigma)) * ((exp(-1 * (((p * p) +
                       (q * q)) / (2 * (sigma * sigma))))));

            (xMask[p + yCent][q + xCent]) = maskValX;
            (yMask[p + yCent][q + xCent]) = maskValY;
        }
    }


    for (i = maxR; i <= WHITE - maxR; i++)
    {
        for (j = maxR; j <= WHITE - maxR; j++)
        {
            xSum = 0;
            ySum = 0;

            for (p = -maxR; p <= maxR; p++)
            {
                for (q = -maxR; q <= maxR; q++)
                {
                    xSum += pic[i + p][j + q] * xMask[p + yCent][q + xCent];
                    ySum += pic[i + p][j + q] * yMask[p + yCent][q + xCent];
                }
            }

            xOutput[i][j] = xSum;
            yOutput[i][j] = ySum;
            xConv[i][j] = xSum;
            yConv[i][j] = ySum;
        }
    }

    maxival = 0;

    for (i = maxR; i < PICSIZE - maxR; i++)
    {
        for (j = maxR; j < PICSIZE - maxR; j++)
        {
            ival[i][j] = sqrt((double)((xOutput[i][j] * xOutput[i][j]) + 
                             (yOutput[i][j] * yOutput[i][j])));

            if (ival[i][j] > maxival)
            {
                maxival = ival[i][j];
            }
        }
    }

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            mag[i][j] = ival[i][j];
        }
    }

    for (i = maxR; i < PICSIZE - maxR; i++)
    {
        for (j = maxR; j < PICSIZE - maxR; j++)
        {
            if ((xConv[i][j]) == 0.0) 
            {
                xConv[i][j] = .00001;
            }

            slope = yConv[i][j] / xConv[i][j];

            if ((slope <= .4142) && (slope > -.4142))
            {
                if ((mag[i][j] > mag[i][j - 1]) && (mag[i][j] > mag[i][j + 1]))
                {
                    peaksOutput[i][j] = WHITE;
                }
            }
            else if ((slope <= 2.4142) && (slope > .4142))
            {
                if ((mag[i][j] > mag[i - 1][j - 1]) && (mag[i][j] > mag[i + 1][j + 1]))
                {
                    peaksOutput[i][j] = WHITE;
                }
            }
            else if ((slope <= -.4142) && (slope > -2.4142))
            {
                if ((mag[i][j] > mag[i + 1][j - 1]) && (mag[i][j] > mag[i - 1][j + 1]))
                {
                    peaksOutput[i][j] = WHITE;
                }
            }
            else
            {
                if((mag[i][j] > mag[i - 1][j]) && (mag[i][j] > mag[i + 1][j]))
                {
                    peaksOutput[i][j] = WHITE;
                }
            }
        }
    }
    
    fprintf(fo1,"P5\n");
    fprintf(fo1,"%d %d\n", rows, cols);
    fprintf(fo1,"255\n");

    fprintf(fo2,"P5\n");
    fprintf(fo2,"%d %d\n", rows, cols);
    fprintf(fo2,"255\n");

    fprintf(fo3,"P5\n");
    fprintf(fo3,"%d %d\n", rows, cols);
    fprintf(fo3,"255\n");

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            mag[i][j] = (mag[i][j] / maxival) * WHITE;
            fprintf(fo1, "%c", (char)((int)(mag[i][j])));
        }
    }

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            if (peaksOutput[i][j] == WHITE)
            {
                fprintf(fo2,"%c",(char)((int)(WHITE)));
            }
            else
            {
                fprintf(fo2,"%c",(char)((int)(BLACK)));
            }
        }
    }

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            histogram[(int)(mag[i][j])]++;
        }
    }

    area = (percentage / MAXMASK) * (PICSIZE * PICSIZE);

    for (tsh = WHITE; tsh > 0; tsh--)
    {
        sum += histogram[tsh];

        if (sum > (int)(area))
        {
            break;
        }
    }

    hiTSH = tsh;
    loTSH = hiTSH * 0.35;

    printf("Percentage value: %lf\n", percentage);
    printf("High treshold value: %d\n", hiTSH);
    printf("Low threshold value: %d\n", loTSH);

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            if (peaksOutput[i][j] == WHITE)
            {
                if (mag[i][j] > hiTSH)
                {
                    peaksOutput[i][j] = BLACK;
                    finalOutput[i][j] = WHITE;
                    loop(i, j, loTSH);
                }
                else if (mag[i][j] < loTSH)
                {
                    peaksOutput[i][j] = BLACK;
                    finalOutput[i][j] = BLACK;
                }
            }
        }
    }

    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j < PICSIZE; j++)
        {
            if (finalOutput[i][j] == WHITE)
            {
                fprintf(fo3,"%c",(char)((int)(WHITE)));
            }
            else
            {
                fprintf(fo3,"%c",(char)((int)(BLACK)));
            }
        }
    }

    fclose(fp1);
    fclose(fo1);
    fclose(fo2);
    fclose(fo3);

    return 0;
}


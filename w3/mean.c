
/* mean.c 
   (C) Joshua Knowles 2010

   simple code to calculate the mean, variance and standard error

   INPUT: text file of 1 column of (up to 100) numbers
   OUTPUT: output to stdout (the screen) of the MEAN, VAR and STANDARD ERROR

   To compile:
   g++ mean.c -o mean -lm

   To run:
   ./mean [inputfilename]

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *fp;

double *V;
int N;

void get_input(char *filename);
void mean_plus_variance(double *data, double *mean, double *variance, int num);

int main(int argc, char *argv[])
{
  int i;
  if(argc > 1)
    {
      get_input(argv[1]);
    }
  else
    {
      fprintf(stderr,"Need input filename\n");
      exit(0);
    }

  double mean,var;
  mean_plus_variance(V,&mean,&var,N);
  printf("%g %g %g\n", mean, var, sqrt(var)/sqrt(double(N)));

}

void get_input(char *filename)
{
  int i=0, j=0;
  int c;

  V = (double *)malloc(100*sizeof(double));

  if(fp=fopen(filename, "rb"))
    {
      while((!feof(fp))&&(i<100))
	{
	  fscanf(fp, "%lf\n", &V[i]);
	  i++;
	}
    }
  else
    {
      fprintf(stderr,"Couldn't open %s\n", filename);
      exit(0);
    }
  N=i;
}

void mean_plus_variance(double *data, double *mean, double *variance, int num)
{
  int i, j;
  int n=0;
  *mean=0;
  double M2=0;
  double delta;
  double x;
  *variance=0;
  for(i=0;i<num;i++)
    {
      n++;
      x=data[i];
      delta=x-*mean;
      *mean+=delta/(double)n;
      M2 = M2+delta*(x-*mean);
      //   printf("%g\n", x);
    }
  *variance=M2/(double)n;
}

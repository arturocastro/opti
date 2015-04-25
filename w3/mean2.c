
/* mean2.c
   (C) Joshua Knowles 2010

   Use for calculating mean and standard error of a matrix of results

   INPUT: Fitness values against function evaluations from several runs of an optimization algorithm
   OUTPUT: Mean and standard error fitness values at each number of function evaluations.

   COMPILE: 
   g++ mean2.c -o mean2 -lm
   
   RUN:
   ./mean2 [num_runs] [num_datapoints] [inputfile]

   where num_datapoints is the number of datapoints per run

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

FILE *fp;

int runs;
int gn; // number of data points

double **V;


void get_input(char *filename);
void mean_plus_variance(double *data, double *mean, double *variance, int num);

int main(int argc, char *argv[])
{
  runs=atoi(argv[1]);
  gn=atoi(argv[2]);

  V = (double **)malloc(gn*sizeof(double *));
  for(int i=0;i<gn;i++)
    V[i]=(double *)malloc(runs*sizeof(double));

  int i, j;
  int *num;
  num=(int *)malloc(gn*sizeof(int));

  for(i=0;i<gn-1;i++)
    num[i]=10000*(i+1)+1;
  num[gn-1]=10000*(gn);

  if(argc > 3)
    {
      get_input(argv[3]);
    }
  else
    {
      fprintf(stderr,"Need <#runs> <#datapoints> <infilename>\n");
      exit(0);
    }

  double mean,var;

  for(i=0;i<gn;i++)
    {
      /*if(i==1){
	for(j=0;j<runs;j++)
	  fprintf(stderr,"%g\n", V[i][j]);
	  }*/
      mean_plus_variance(V[i],&mean,&var,runs);
      printf("%d %g %g %g\n", num[i], mean, var, sqrt(var)/sqrt(runs));
    }
}

void get_input(char *filename)
{
  int i=0, j=0;
  int c;
  double v;


  //  V = (double *)malloc(100*sizeof(double));

  if(fp=fopen(filename, "rb"))
    {
      while((!feof(fp))&&(i<100))
	{
	  fscanf(fp, "%*d %lf\n", &v);
	  V[j][i]=v;
	  j++;
	  if(j==gn)
	    {
	      j=0;
	      i++;
	    }
	}
    }
  else
    {
      fprintf(stderr,"Couldn't open %s\n", filename);
      exit(0);
    }

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

/*
evaluate_MAXSAT.c (C) Joshua Knowles April 2013

This program reads in a MAXSAT instance file written in DIMACS CNF format, and
reads in a file consisting of a binary solution vector. It interprets the solution vector
as the truth assignment of the variables in the MAXSAT instance and outputs
two objectives, (i) the number of satisfied clauses, and (ii) the number of true
variables in the assignment.


Compile: gcc evaluate_MAXSAT.c -o eval -lm 

Usage: ./eval <instance.file> <vector.file>


** Please contact me - Joshua Knowles - if you have any comments, suggestions
or questions regarding this program or multiobjective MAXSAT problems. My email
address is j.knowles@manchester.ac.uk

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version. 

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details. 

   The GNU General Public License is available at:
      http://www.gnu.org/copyleft/gpl.html
   or by writing to: 
        The Free Software Foundation, Inc., 
        675 Mass Ave, Cambridge, MA 02139, USA.  


This program also uses Professor Peter Ross' (Napier University) getdouble() and getint() functions.

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>

#define MAX_N 400  // max number of variables - change as required
#define MAX_C 500 // max number of clauses
#define MAX_VPC 5 // max variables per clause
#define MAX_K 2    // max umber of objectives - change as required

// true and false used in getdouble() and getint()
#define GDFALSE 0  
#define GDTRUE 1

#define RN ran0(&seed)
  
/* Random number generator */
/* Copyright Numerical Recipes in C */
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define MASK 123459876

//using namespace std;
  
double ran0(long *idum); 
/* End copyright Numerical Recipes in C */

FILE *fp;

void getSAT(char *infile);
double getdouble(FILE * file, double *valaddr, int stopateol);
int getint(FILE * f, int *valaddr, int stopateol);
long getlong(FILE * f, long *valaddr, int stopateol);
void getASSIGNMENT(char *infile2);
void evaluateASSIGNMENT();

long seed;
long init_seed;
int ind[MAX_N];


// MAXSAT
int nclauses;
int nvars;
int clause[MAX_C][MAX_VPC];


int main(int argc, char *argv[])
{
  int i,j,k;
  double r1,r2;
  int f1,fk;
  int d1;
  seed = 23453464;

  if(argc!=3) 
    {
      printf("need MAXSAT file and solution file. Exiting\n");
      exit(1);
    }
  getSAT(argv[1]);
  getASSIGNMENT(argv[2]);
  evaluateASSIGNMENT();
}

void getSAT(char *infile)
{
  char ch;
  char str[20];
  int i,j;
  int num;

  if (fp = fopen(infile, "r"))
    {
      do
	{
	  fscanf(fp,"%c", &ch);
	  if(ch=='c')
	    do
	      {
		ch=fgetc(fp);
		printf("%c", ch);
	      }while(ch!='\n');
	  else if(ch=='p')
	    {
	      printf("%c", ch);
	      break;
	    }
	  else
	    {
	      printf("%c", ch);
	      fprintf(stderr,"Arse error in read\n");
	      exit(-1);
	    }
	}while(1);
      fscanf(fp,"%s", str);
      if(strcmp(str,"cnf")==0)
	{
	  fscanf(fp, "%d %d\n", &nvars, &nclauses);
	  fprintf(stderr, "Instance has %d variables and %d clauses.\n", nvars, nclauses);
	}
      for(i=0;i<nclauses;i++)
	{
	  j=0;
	  printf("%d. ",i+1);
	  do
	    {
	      getint(fp,&num,GDFALSE);
	      clause[i][j++]=num;
	      printf("%d ",num);
	    }while(num!=0);
	  printf("\n");
	}
      fclose(fp);
      
    }
}


/* Copyright Numerical Recipes in C */

double ran0(long *idum)
{
	long k;
	double ans;

	*idum ^= MASK;
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	ans=AM*(*idum);
	*idum ^= MASK;
	return ans;
}
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef MASK

/* End copyright Numerical Recipes in C */


// The following file i/o functions are written by Professor Peter Ross 
// who is at Napier University, Edinburgh

/***************************************************************/
/* Get the next number from the input: put it in the location  */
/* addressed by second argument. This function returns 0 on    */
/* EOF. If stopateol is true, it returns -1 when it hits \n    */
/* (after which some other procedure has to read past the \n), */
/* otherwise it continues looking for the next number.         */
/* A number has an optional sign, perhaps followed by digits,  */
/* perhaps followed by a decimal point, perhaps followed by    */
/* more digits. There must be a digit somewhere for it to count*/
/* as a number. So it would read any of:                       */
/*  -.5                                                        */
/*  -0.5                                                       */
/*  -.5.7                                                      */
/* as minus-a-half. In the last case, it would read .7 next    */
/* time around.                                                */
/*   There doesn't seem to be a neat and reliable way to do    */
/* all this, including stopateol, using scanf?                 */
/***************************************************************/

double getdouble(FILE * file, double *valaddr, int stopateol)

{
  int c;
  int found = GDFALSE, indecimal = GDFALSE;
  int sign = +1;
  double n = 0.0, p = 1.0;

// First find what looks like start of a number - the first digit. 
// And note any sign and whether we just passed a decimal point.   
  do {
    c = fgetc(file);
    if (c == EOF)
      return (0);
    else if (stopateol && c == '\n')
      return (-1);
    else if (c == '+' || c == '-') {
      sign = (c == '+') ? +1 : -1;
      c = fgetc(file);
      if (c == EOF)
	return (0);
      else if (stopateol && c == '\n')
	return (-1);
    }
    if (c == '.') {
      indecimal = GDTRUE;
      c = fgetc(file);
      if (c == EOF)
	return (0);
      else if (stopateol && c == '\n')
	return (-1);
    }
    if (c >= '0' && c <= '9') {
      found = GDTRUE;
    } else {
      sign = +1;
      indecimal = GDFALSE;
    }
  } while (!found);

  // Now we've got digit(s) ...
  do {
    n = 10.0 * n + c - '0';
    p = 10.0 * p;
    c = fgetc(file);

    if ((c < '0') || (c > '9')) {
      found = GDFALSE;
  // We've run out. If we already saw a decimal point, return now 
      if (indecimal) {
	if (c != EOF)
	  ungetc(c, file);
	*valaddr = sign * n / p;
	return (1);
      } else
	p = 1.0;
    }
  } while (found);

  // We ran out and we didn't see a decimal point, so is this a decimal? 
  if (c != '.') {
  // No, give it back to caller  
    if (c != EOF)
      ungetc(c, file);
    *valaddr = sign * n;
    return (1);
  } else {
// It is. Step past it, carry on hoping for more digits 
    c = fgetc(file);
    while (c >= '0' && c <= '9') {
      n = 10.0 * n + c - '0';
      p = p * 10.0;
      c = fgetc(file);
    }
// We've run out of digits but we have a number to give 
    if (c != EOF)
      ungetc(c, file);
    *valaddr = sign * n / p;
    return (1);
  }
}

// Use getdouble() above but convert result to int. 
int getint(FILE * f, int *valaddr, int stopateol)
{
  int r;
  double x;
  r = (int)getdouble(f, &x, stopateol);
  *valaddr = (int) x;
  return (r);
}

// Use getdouble() above but convert result to long. 
long getlong(FILE * f, long *valaddr, int stopateol)
{
  int r;
  double x;
  r = (long)getdouble(f, &x, stopateol);
  *valaddr = (long) x;
  return (r);
}

// end Peter Ross' file-reading functions

void getASSIGNMENT(char *infile2)
{
  int i;

  if(fp=fopen(infile2, "r"))
    {
      for(i=0;i<nvars;i++)
	{
	  getint(fp, &ind[i], GDFALSE);
	  // printf("%d", ind[i]);
	}
      fclose(fp);      
    }
  else
    {
      fprintf(stderr,"Sorry - couldn't read your solution vector file.\n");
      exit(-1);
    }
}

void evaluateASSIGNMENT()
{
  // interpret the solution vector - the assignment - as a boolean truth assignment to each variable

  int c, cl, i, j, sign;
  int ncsat; // number of clauses satisfied (the first objective)
  int nvarstrue; // the number of variables set to true (the second objective)


  // first check if the assignment is feasible (is 0 or 1 for each variable)

  nvarstrue=0;
  for(i=0;i<nvars;i++)
    {
      if((ind[i]!=0)&&(ind[i]!=1))
	{
	  fprintf(stderr,"Input was not a valid assignment\n");
	  exit(-1);
	}
      if(ind[i]==0)
	ind[i]=-1;
      else
	nvarstrue++;
    }


  // then evaluate it
  printf("Solution evaluates to:\n");

  ncsat=0;

  for(c=0;c<nclauses;c++)
    {
      i=0;
      do
	{
	  cl = clause[c][i];

	  if(cl<0)
	    {
	      sign=-1;	      
	    }
	  else
	    sign=1;
	  if(sign *ind[(sign*cl)-1] > 0)
	    {
	      ncsat++;
	      break;
	    }
	  i++;
	}while(cl!=0);
    }
  printf("%d %d OBJECTIVE VECTOR\n", ncsat, nvarstrue);
  
}

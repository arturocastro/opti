
// Steady State Evolutionary Algorithm for the Student-Project Assignment Problem
// (C) Joshua Knowles 2011

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
using namespace std;


#define RN (rand()/(RAND_MAX+1.0))
#define M 801

FILE *fp;

long seed;
int nstu = 158;  // number of students (157) plus one for array-sizing etc
const int popsize=100;
int MAXITERS=500000;
double pergenome=1.0; // mutation rate per genome. So if this is set to 1.0 the expected number of mutations per individual is 1.0
int tsize;  // tournament size
int tsize2;  // tournament size for selection of a poor individual to be replaced

typedef struct solution
{
  int x[158];  // the assignment
  int f;       // the overall fitness value
  int v;       // the number of hard constraint violations (if this is nonzero then the assignment is infeasible)
  int dist[6]; // the distribution: number of people who got 1st choice, 2nd choice, 3rd choice etc
}sol;

sol P[popsize];
sol mutant;
sol be;  // holds best ever solution

struct project
{
  char ID[30];  // the ID number of the project
  int max_p;      // the maximum number of students that can be allocated this project
  int alloc;     // the number actually allocated
};

struct student
{
  int num;               // student ID: just a number in the range 1 to 157
  char choices[6][30];   // the (up to) five choices of project ID the student made. choices[1] holds the first
  int nc;                // the total number of choices the student made
};

struct project a[M+1];
struct student st[200];


// A hash table is used to store the projects so we can access them by their ID number
unsigned hash(char *v)
{
  int h;
  for(h=0; *v !='\0'; v++)
    h=(64*h+ *v)%M;
  return h;
}

void hashinitialize()
{
  int i;
  for(i=0;i<=M;i++)
    {
      //      a[i].ID = " "; a[i].max_p=-1;
      sprintf(a[i].ID," ") ; a[i].max_p=-1;

    }
}

void hashinsert(char *v, int max_p)
{
  int x = hash(v);
  while (strcmp(" ", a[x].ID))
    x=(x+1)%M;
  // printf("%d\n",x);
  sprintf(a[x].ID,"%s", v);
  a[x].max_p=max_p;
}

int hashsearch(char *v)
{
  int x = hash(v);
  int tries=0;
  while((strcmp(v, a[x].ID))&&(tries<2*M))
    {
      tries++;
      x=(x+1)%M;
    }
  if(tries==2*M)
    {
      printf("search for %s failed\n", v);
      return -1;
      // exit(1);
    }

  //  printf("found %d\n",x);
  //  return a[x].max_p;
  return x;
}

void mutate(sol *c)
{
  double mrate=pergenome/2.0;
  int i;
  for(i=1;i<=157;i++)
    {
      if(RN<mrate/157.0)
	c->x[i]++;
      else if(RN<mrate/157.0)
	c->x[i]--;
      if(c->x[i]==6)
	c->x[i]=5;
      if(c->x[i]==0)
	c->x[i]=1;   
    }
}

int tourn_worst()
{
  int tourn_size=tsize2;
  // this function should return the index of a solution that 
  // "wins" a tournament to find a POOR fitness individual.
  // It can be written in the same way as tournament_select().
  // It should have its own tournament size parameter which it
  // takes from the tsize2 command line.

  int i;
  int mem;

  int poorest;
  double worst_fitness=-1.0; //1e20;
  i=0;
  while(i<tourn_size)
    {
      mem=int(RN*popsize);
      if(P[mem].f>worst_fitness)
	{
	  worst_fitness=P[mem].f;
	  poorest=mem;
	}
      i++;
    }
  return(poorest);

  //fprintf(stderr, "You need to write tourn_worst() function first. Exiting\n");
  //exit(1);
  
}


int tournament_select()
{
  int tourn_size=tsize;
  int i;
  int mem;

  int fittest;
  double best_fitness=1e20;
  i=0;
  while(i<tourn_size)
    {
      mem=int(RN*popsize);
      if(P[mem].f<best_fitness)
	{
	  best_fitness=P[mem].f;
	  fittest=mem;
	}
      i++;
    }
  return(fittest);
}

void initialize()
{
  int rbit;

  for(int i=0;i<popsize;i++)
    {
      for(int j=1;j<=nstu;j++)
	{
	  rbit=1+int(5*RN);
	  P[i].x[j]=rbit;
	}
    }

}

void crossover(sol *p1, sol *p2, sol *ch)
{
  // this function should implement uniform crossover 
 
  // p1->x[i] is the ith gene of parent 1
  // p2->x[i] likewise for parent 2
  // ch->x[i] is the ith gene of the offspring chromosome 

    for (int i = 1; i <= nstu; ++i)
    {
	if (int(RN) % 2)
	{
	    ch->x[i] = p1->x[i];
	}
	else
	{
	    ch->x[i] = p2->x[i];
	}
    }

  //fprintf(stderr,"You must first write the crossover function. Exiting.\n");
  //exit(1);
}


int evaluate_sol(sol *s)
{
  int i, ch, fitness;

  for(i=1;i<=M;i++)
    a[i].alloc=0;  // reset all the allocations to zerox

  int proj;
  int pref_score=0;

  for(i=1;i<6;i++)
    s->dist[i]=0;

  
  for(i=1;i<=157;i++)
    {
      ch = s->x[i];  // ch is the choice (rank), s->x[i] this choice (rank) on the chromosome
      if (ch>st[i].nc)  // st[i].nc is the maximum rank for that student
	{
	  ch = st[i].nc; // repair it
	  s->x[i]=ch;  // repair it back on the chromosome (Lamarckian repair)
	}

      s->dist[ch]++;

      pref_score+=(ch-1)*10; // a linear penalty: 10 for second choice, 20 for third etc.
      
      proj=hashsearch(st[i].choices[ch]);
      if(proj==-1)
	{
	  printf("St=%d ch=%d max=%d\n", i, ch, st[i].nc);
	}
      (a[proj].alloc)++;
    }
  
  int v=0;
  for(i=1;i<=M;i++)
    {
      if(a[i].max_p>0)
	{
	  if(a[i].alloc>a[i].max_p)
	    {
	      //	      printf("%s %d %d : %d Violations\n", a[i].ID, a[i].max_p, a[i].alloc, a[i].alloc-a[i].max_p);
	      v+=a[i].alloc-a[i].max_p;
	    }
	}
    }

  

  fitness = 1000*v + pref_score;

  s->f = fitness;
  s->v = v;
  
  return fitness;

}



void print_allocation(sol *s)
{
  int i, proj;
  char id[30];
  evaluate_sol(s);

  printf("STUDENT\tCHOICE\tPRJ_ID\tN_ALLOC\tMAX\n");
  for(i=1;i<=157;i++)
    {
      sprintf(id,"%s",st[i].choices[s->x[i]]);
      proj=hashsearch(id);
      printf("%d\t%d\t%s\t%d\t%d\n", i, s->x[i], id, a[proj].alloc, a[proj].max_p);
    }
  

}

void print_solution_details(sol *s)
{
  int i;

  print_allocation(s);
  printf("Summary details:\n");
  printf("#violations= %d\n", s->v);
  printf("fitness= %d\n", s->f);
  for(i=1;i<6;i++)
    printf("%d students got their choice %d project\n", s->dist[i], i);
  
  
}

int main(int argc, char **argv)
{

  if(argc!=6)
    {
      fprintf(stderr,"Arguments: seed MAXITERS pergenome tsize1 tsize2\n");
      exit(1);
    }

  seed=atol(argv[1]);
  srand(seed);
  MAXITERS=atoi(argv[2]);
  pergenome=atol(argv[3]);
  tsize = atoi(argv[4]);
  tsize2 = atoi(argv[5]);

  int parent_a, parent_b;
  int best=1000000;
  int i,j,f,g,r;
  char proj[30];

  if(fp=fopen("Project-Choices.txt", "r"))
    {
      fscanf(fp, "%*s %*s %*s\n");
      while(!feof(fp))
	{
	  fscanf(fp, "%d %s %d\n", &i, proj, &r);
	  // printf("%d\n", i);
	  sprintf(st[i].choices[r],"%s",proj);
	  st[i].nc=r;
	}
      fclose(fp);
    }
  else
    {
      fprintf(stderr,"Couldn't open project choices file\n");
      exit(1);
    }

  // student choices read in.
  // Next set up hash table and read in project IDs and numbers of participants
  // and store in hash as well

  hashinitialize();
  int n;
  if(fp=fopen("Project-Participants.txt", "r"))
    {
      fscanf(fp, "%*s %*s \n");
      while(!feof(fp))
	{
	  fscanf(fp, "%s %d \n", proj, &n);
	  hashinsert(proj, n);
	}
      
      fclose(fp);
    }
  else
    {
      fprintf(stderr,"Couldn't open project participants file\n");
      exit(1);
    }

  initialize();

  for(i=0;i<popsize;i++)
    {
      P[i].f=evaluate_sol(&P[i]);
      // printf("%d\n",P[i].f);
    }

  int fit, worst_fitness, mean_fitness, k, offset, worst_mem, best_ever=200000000;

  for(i=popsize;i<=MAXITERS;i++)
    {
      parent_a=tournament_select();

      if(RN<0.7)  // this is p_c the crossover probability
	{
	  parent_b=tournament_select();
	  crossover(&P[parent_a],&P[parent_b],&mutant); //sexual reproduction
	}
      else
	mutant=P[parent_a]; // clone the parent 

      mutate(&mutant); // always mutate
      mutant.f=fit=evaluate_sol(&mutant);
      P[tourn_worst()] = mutant;
      
      if(i%10000==1)
      	printf("%d Fitness= %d Best= %d\n", i, fit, best_ever);
      
      if(fit<best_ever)
	{
	  best_ever=fit;
	  be = mutant;
	}
      fflush(stdout);
    }
  
  // optimization ended
  
  printf("%d Fitness= %d Best= %d\n",MAXITERS, fit, best_ever);
  
  // re-evaluate the best ever
  evaluate_sol(&be);
  
  // print out over-allocated projects (i.e. violations) first
  printf("\n\n=== RESULTS (last random solution) ===\n\nOver-allocated projects (violations) :\n");
  printf("PRJ_ID\tN_ALLOC\tMAX\n");
  for(i=1;i<=M;i++)
    {
      
      if(a[i].max_p>0)
	if(a[i].alloc>a[i].max_p)
	  printf("%s\t%d\t%d\n", a[i].ID, a[i].alloc, a[i].max_p);
    }
  print_solution_details(&be);
  
}

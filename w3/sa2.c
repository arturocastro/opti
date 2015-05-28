#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define RN (rand()/(RAND_MAX+1.0))
#define M 801

FILE *fp;

long seed;
int MAXITERS;
double Tstart;
double Tend;
int stop_cooling;

typedef struct solution
{
  int x[158];  // the assignment
  int f;       // the overall fitness value
  int v;       // the number of hard constraint violations (if this is nonzero then the assignment is infeasible)
  int dist[6]; // the distribution: number of people who got 1st choice, 2nd choice, 3rd choice etc
}sol;

sol *s; // current solution
sol *c; // a random neighbour of current
sol *be; // the best ever solution


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
  sprintf(a[x].ID,"%s",v);
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




int evaluate_sol(sol *s)
{
  int i, ch, fitness;

  for(i=1;i<=M;i++)
    a[i].alloc=0;  // reset all the allocations to zero

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

int delta_evaluation(sol *s)
{
  int i, ch, fitness;

  for(i=1;i<=M;i++)
    a[i].alloc=0;  // reset all the allocations to zero

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
  printf("Complete List of Project Allocations Made:\n");
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
      fprintf(stderr,"Arguments: seed MAXITERS Tstart Tend stop_cooling\n");
      exit(1);
    }

  seed=atol(argv[1]);
  srand(seed);
  MAXITERS=atoi(argv[2]);
  Tstart=(double)atof(argv[3]);
  Tend=(double)atof(argv[4]);
  stop_cooling=atoi(argv[5]);
    
 
  s = (sol *)malloc(sizeof(sol));  // current solution
  c = (sol *)malloc(sizeof(sol));  // neighbour solution (or mutant of s)
  be= (sol *)malloc(sizeof(sol));  // best ever solution

  int best=1000000; // fitness of the best ever solution
  char proj[30];
  int i,j,f,g,r;
  double T=Tstart;  // the temperature parameter

  double gc=1.0;  // the geometric cooling coefficient


  if((Tstart<=0)||(Tend<=0))
    {
      Tstart=0,Tend=0;
    }

  else if(Tend<Tstart)
    gc = pow(10.0,((1.0/(stop_cooling))*(log10(Tend/Tstart))));
  printf("gc=%g\n", gc);
  //  getchar();

  if(fp=fopen("Project-Choices.txt", "r"))
    {
      fscanf(fp, "%*s %*s %*s\n");
      while(!feof(fp))
	{
	  fscanf(fp, "%d %s %d\n", &i, proj, &r);
	  printf("%d\n", i);
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
  int x;

  for(i=1;i<=157;i++)    // for every student
    s->x[i]=1+(int)(RN*5);   // give them a project choice between 1 and 5
  f=evaluate_sol(s);

  g = f;

  for(j=0;j<MAXITERS;j++)
    {
      *c=*s;
      for(i=1;i<=157;i++)
	{
	  // Mutate assignments by a little bit!
	    if(RN<1.0/157.0) {
		c->x[i]++;
		
		if(c->x[i]==6)
		{
		    c->x[i]=5;
		}
		else
		{
		    int ch = c->x[i];  // ch is the choice (rank), s->x[i] this choice (rank) on the chromosome
		    if (ch>st[i].nc)  // st[i].nc is the maximum rank for that student
		    {
			ch = st[i].nc; // repair it
			c->x[i]=ch;  // repair it back on the chromosome (Lamarckian repair)
		    }
		    else
		    {
			// Actual change!!!
			g += 10;

			// Decrease previous solution choice allocation
			int proj2=hashsearch(st[i].choices[ch - 1]);
			(a[proj2].alloc)--;

			// if decreasing that last project allocation removes a penalty...
			if (a[proj2].alloc == a[proj2].max_p) {
			    g -= 1000;
			    c->f = g;
			    c->v -= 1;
			}

			// Increase mutated solution choice allocation
			proj2=hashsearch(st[i].choices[ch]);
			(a[proj2].alloc)++;
			
			// Introduces penalty?
			if (a[proj2].max_p > 0 && a[proj2].alloc == a[proj2].max_p + 1) {
			    g += 1000;
			    c->f = g;
			    c->f += 1;
			}
		    }
		}
	    }
	    else if(RN<1.0/157.0)
	    {
		c->x[i]--;
		
		if(c->x[i]==0)
		{
		    c->x[i]=1;
		}
		else
		{
		    int ch = c->x[i];  // ch is the choice (rank), s->x[i] this choice (rank) on the chromosome
		    
                    // Actual change!!!
		    g -= 10;

		    // Decrease previous solution choice allocation
		    int proj2=hashsearch(st[i].choices[ch + 1]);
		    (a[proj2].alloc)--;
		    
		    // if decreasing that last project allocation removes a penalty...
		    if (a[proj2].alloc == a[proj2].max_p) {
			g -= 1000;
			c->f = g;
			c->f -= 1;
			
		    }
		    
		    // Increase mutated solution choice allocation
		    proj2=hashsearch(st[i].choices[ch]);
		    (a[proj2].alloc)++;
		    
		    // Introduces penalty?
		    if (a[proj2].max_p > 0 && a[proj2].alloc == a[proj2].max_p + 1) {
			g += 1000;
			c->f = g;
			c->f += 1;
		    }
		    
		}
	    }
	  
	}
      //g=evaluate_sol(c);
      //g = delta_evaluation(c);
      if((g<f)||((exp((f-g)/T) > RN)))
	{
	  f=g;
	  *s=*c;
	}
      
      if(j%10000==1)
	printf("%d Fitness= %d Best= %d\n",j,f, best);
      if(f<best)
	{
	  best=f;
	  *be=*s;
	}
      if(j<stop_cooling)
	T*=gc;

    }
  // optimization ended
  
  printf("%d Fitness= %d Best= %d\n",MAXITERS,f, best);
  
  // re-evaluate the best ever
  evaluate_sol(be);

  // print out over-allocated projects (i.e. violations) first
  printf("\n\n=== RESULTS (last random solution) ===\n\nOver-allocated projects (violations) :\n");
  printf("PRJ_ID\tN_ALLOC\tMAX\n");
  for(i=1;i<=M;i++)
    {
      
      if(a[i].max_p>0)
	if(a[i].alloc>a[i].max_p)
	  printf("%s\t%d\t%d\n", a[i].ID, a[i].alloc, a[i].max_p);
    }
  print_solution_details(be);
  
}

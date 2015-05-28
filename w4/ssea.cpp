
// Steady State Evolutionary Algorithm for the Student-Project Assignment Problem
// (C) Joshua Knowles 2011

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "hv-1.3-src/hv.h"

#define RN (rand()/(RAND_MAX+1.0))

long seed;
const int pareto_size = 100;
int MAXITERS=500000;
double pergenome=1.0; // mutation rate per genome. So if this is set to 1.0 the expected number of mutations per individual is 1.0
int tsize;  // tournament size
int tsize2;  // tournament size for selection of a poor individual to be replaced

typedef struct solution
{
    char x[1023]; // the boolean vars
    int objs[2]; // 0 is satisfied clauses, 1 is positive vars
    int f; // fitness
} sol;

int n_vars;
int n_clauses;

int ** clauses;

sol pareto[pareto_size];

sol mutant;
sol be[pareto_size];  // holds best ever solution - pareto front!


void print_pareto(void)
{
    int i;
    for (i = 0; i < pareto_size; ++i)
    {
	printf("#%d : %s : n_pos=%d, n_sat_clauses=%d, f=%d\n", i, pareto[i].x, pareto[i].objs[1], pareto[i].objs[0], pareto[i].f);
    }
}


bool myfunction1 (const sol & i, const sol & j) { return (i.objs[1]>j.objs[1]); }
bool myfunction2 (const sol & i, const sol & j) { return (i.objs[0]>j.objs[0]); }
bool myfunction3 (const sol & i, const sol & j) { return (i.f<j.f); }


void solcpy(sol * a, sol * b)
{
    a->f = b->f;
    
    a->objs[0] = b->objs[0];
    a->objs[1] = b->objs[1];

    strcpy(a->x, b->x);
}


int nondominated_rank(int n, int rank)
{
    std::sort(pareto, pareto + n, myfunction1);
    std::stable_sort(pareto, pareto + n, myfunction2);

    int last = -1;

    int ranks_assigned = 0;

    int i;

    for (i = 0; i < n; ++i)
    {
	if (pareto[i].objs[1] > last)
	{
	    pareto[i].f = 9999999 - rank;
	    last = pareto[i].objs[1];

	    ++ranks_assigned;
	}
    }

    std::sort(pareto, pareto + n, myfunction3);
    
    return n - ranks_assigned;
}


void nondominated_sorting()
{
    int n = pareto_size;
    int rank = 1;

    while (n > 0)
    {
	n = nondominated_rank(n, rank);
	++rank;
    }
}


void sol_init(sol * s)
{
    s->objs[0] = 0;
    s->objs[1] = 0;

    s->f = 0;

    s->x[0] = '!';
    s->x[pareto_size] = '\0';

    int i;
    for (i = 1; i <= n_vars; ++i)
    {
	double rn = RN;

	if (rn > 0.5)
	{
	    s->x[i] = '0';
	}
	else
	{
	    s->x[i] = '1';
	}
    }
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

	if(c->x[i]=='2')
	    c->x[i]='1';
	if(c->x[i]=='/')
	    c->x[i]='0';   
    }
}

int tourn_worst()
{
    int tourn_size=tsize2;
    
    int i;
    int mem;
    
    int poorest;
    double worst_fitness=-1e20;
    i=0;
    while(i<tourn_size)
    {
	mem=(int)(RN*pareto_size);
	if(pareto[mem].f<worst_fitness)
	{
	    worst_fitness=pareto[mem].f;
	    poorest=mem;
	}
	++i;
    }
    return(poorest);
}


int tournament_select()
{
    int tourn_size=tsize;
    int i;
    int mem;
    
    int fittest;
    double best_fitness=-1;
    i=0;
    while(i<tourn_size)
    {
	mem=(int)(RN*pareto_size);
	if(pareto[mem].f>best_fitness)
	{
	    best_fitness=pareto[mem].f;
	    fittest=mem;
	}
	++i;
    }
    return(fittest);
}


void initialize(void)
{
    char str[255];

    do
    {
	fgets(str, 254, stdin);
    }
    while (str[0] == 'c');
    
    sscanf(str, "p cnf %d %d", &n_vars, &n_clauses);
    
    int i;

    clauses = (int**)malloc((n_clauses + 1) * sizeof(int *));
    for (i = 1; i <= n_clauses; ++i)
    {
	clauses[i] = (int*)malloc(3 * sizeof(int));
    }

    for (i = 1; i <= n_clauses; ++i)
    {
	int j;
	for (j = 0; j < 3; ++j)
	{
	    scanf("%d", &clauses[i][j]);
	}
	scanf("%d", &j);
    }

    for (i = 0; i < pareto_size; ++i)
    {
	sol_init(&pareto[i]);
	sol_init(&be[i]);
    }

    sol_init(&mutant);
}


void crossover(sol *p1, sol *p2, sol *ch)
{
    int i;
    for (i = 1; i <= n_vars; ++i)
    {
	if (RN > 0.5)
	{
	    ch->x[i] = p1->x[i];
	}
	else
	{
	    ch->x[i] = p2->x[i];
	}
    }
}


void evaluate_sol(sol * s)
{
    int i, ch, fitness;
    
    s->objs[0] = 0;
    s->objs[1] = 0;
    s->f = 0;
     
    for (i = 1; i <= n_vars; ++i)
	if (s->x[i] == '1')
	    s->objs[1]++; // n_positives
    
    for (i = 1; i <= n_clauses; ++i)
    {
	int j = 0;
	for (; j < 3; ++j)
	{
	    int var = clauses[i][j];
	    int ix = abs(var);
	    
	    if ((var > 0 && s->x[ix] == '1') ||
		(var < 0 && s->x[ix] == '0'))
	    {
		s->objs[0]++; // n_sat_clauses 
		break;
	    }
	}
    }
}


void print_clauses(void)
{
    int i;
    for (i = 1; i <= n_clauses; ++i)
    {
	int j;
	printf("c%d: ", i);
	for (j = 0; j < 3; ++j)
	{
	    printf("%d ", clauses[i][j]);
	}
	puts("");
    }
}


double pareto_hv(int * ret)
{
    double ref[] = {0.0, 0.0};
    double front[200];

    int front_ix = 0;

    int front_size = 1;

    int front_f = pareto[pareto_size - 1].f;

    front[front_ix++] = -pareto[pareto_size - 1].objs[0];
    front[front_ix++] = -pareto[pareto_size - 1].objs[1];

    int i = pareto_size - 2;
    for (; i > 0; --i)
    {
	if (pareto[i].f == front_f)
	{
	    front_size++;
	    front[front_ix++] = -pareto[i].objs[0];
	    front[front_ix++] = -pareto[i].objs[1];
	}
	else if ((pareto[i].objs[0] == pareto[i + 1].objs[0]) && (pareto[i].objs[1] == pareto[i + 1].objs[1]))
	{
	    front_size++;
	    front[front_ix++] = -pareto[i].objs[0];
	    front[front_ix++] = -pareto[i].objs[1];
	}
	else
	{
	    break;
	}
    }
    
    *ret = front_ix;
    return fpli_hv(front, 2, front_ix, ref);
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
    pergenome=atof(argv[3]);
    tsize = atoi(argv[4]);
    tsize2 = atoi(argv[5]);
    
    int parent_a, parent_b;
    int i,j;
    
    initialize();
    
    for (i = 0; i < pareto_size; ++i)
	evaluate_sol(&pareto[i]);
    
    nondominated_sorting();
    
    int fit,  best_ever=-1;
    int front_idx;
    
    for(i=pareto_size;i<=MAXITERS;++i)
    {
	parent_a=tournament_select();
	if(RN<0.7)  // this is p_c the crossover probability
	{
	    parent_b=tournament_select();
	    crossover(&pareto[parent_a],&pareto[parent_b],&mutant); //sexual reproduction
	}
	else
	    solcpy(&mutant, &pareto[parent_a]); // clone the parent 
	
	mutate(&mutant); // always mutate
	
	evaluate_sol(&mutant);
	solcpy(&pareto[tourn_worst()], &mutant);
	nondominated_sorting();
	
	fit = pareto_hv(&front_idx);
	
	if(i%1000==1)
	{
	    //print_pareto();
	    printf("%d Fitness= %d Best= %d front_idx= %d\n", i, fit, best_ever, front_idx);
	}
	
	if(fit>best_ever)
	{
	    //printf("%d Fitness= %d Best= %d front_idx= %d\n", i, fit, best_ever, front_idx);
	    best_ever=fit;
	    
	    for (j = 0; j < pareto_size; ++j)
		solcpy(&be[j], &pareto[j]);
	}
	fflush(stdout);
    }

    for (i = 0; i < pareto_size; ++i)
	solcpy(&pareto[i], &be[i]);

    nondominated_sorting();
    
    print_pareto();
    printf("Best ever = %d\n", best_ever);

    FILE * all = fopen("all.dat", "w");

    for (i = 0; i < pareto_size; ++i)
	fprintf(all, "%d %d\n", pareto[i].objs[0], pareto[i].objs[1]);

    fclose(all);

    FILE * nondominated = fopen("nondominated.dat", "w");

    int front_f = pareto[pareto_size - 1].f;    

    fprintf(nondominated, "%d %d\n", pareto[pareto_size -1 ].objs[0], pareto[pareto_size - 1].objs[1]);

    i = pareto_size - 2;
    for (; i > 0; --i)
    {
	if (pareto[i].f == front_f)
	{
	    fprintf(nondominated, "%d %d\n", pareto[i].objs[0], pareto[i].objs[1]);
	}
	else if ((pareto[i].objs[0] == pareto[i + 1].objs[0]) && (pareto[i].objs[1] == pareto[i + 1].objs[1]))
	{
	    fprintf(nondominated, "%d %d\n", pareto[i].objs[0], pareto[i].objs[1]);
	}
	else
	{
	    break;
	}
    }

    fclose(nondominated);
}

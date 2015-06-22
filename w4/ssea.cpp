#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "hv-1.3-src/hv.h"

int get_rand(void)
{
    return rand() / RAND_MAX + 1.0;
}

long seed;
const int population_size = 100;
double pergenome = 1.0; // mutation rate per genome. So if this is set to 1.0 the expected number of mutations per individual is 1.0
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

int** clauses;

sol population[population_size];
sol best[population_size];  // holds best ever solution - population front!


void print_population(void)
{
    for (int i = 0; i < population_size; ++i)
    {
	printf("#%d : %s : n_pos=%d, n_sat_clauses=%d, f=%d\n", i, population[i].x, population[i].objs[1], population[i].objs[0], population[i].f);
    }
}


bool myfunction1 (const sol& i, const sol& j) { return (i.objs[1] > j.objs[1]); }
bool myfunction2 (const sol& i, const sol& j) { return (i.objs[0] > j.objs[0]); }
bool myfunction3 (const sol& i, const sol& j) { return (i.f < j.f); }


int nondominated_rank(int n, int rank)
{
    std::sort(population, population + n, myfunction1);
    std::stable_sort(population, population + n, myfunction2);

    int last = -1;

    int ranks_assigned = 0;

    for (int i = 0; i < n; ++i)
    {
	if (population[i].objs[1] > last)
	{
	    population[i].f = 9999999 - rank;
	    last = population[i].objs[1];

	    ++ranks_assigned;
	}
    }

    std::sort(population, population + n, myfunction3);
    
    return n - ranks_assigned;
}


void nondominated_sorting()
{
    int n = population_size;
    int rank = 1;

    while (n > 0)
    {
	n = nondominated_rank(n, rank);
	++rank;
    }
}


void sol_init(sol* s)
{
    s->objs[0] = 0;
    s->objs[1] = 0;

    s->f = 0;

    s->x[0] = '!';
    s->x[population_size] = '\0';

    for (int i = 1; i <= n_vars; ++i)
    {
	if (get_rand() > 0.5)
	{
	    s->x[i] = '0';
	}
	else
	{
	    s->x[i] = '1';
	}
    }
}


void mutate(sol* c)
{
    double mutation_rate = pergenome / 2.0;
    
    for(int i = 1; i <= 157; ++i)
    {
	if(get_rand() < mutation_rate / 157.0)
	    c->x[i]++;
	else if(get_rand() < mutation_rate / 157.0)
	    c->x[i]--;

	if(c->x[i] == '2')
	    c->x[i] = '1';
	if(c->x[i] == '/')
	    c->x[i] = '0';   
    }
}


int tourn_worst()
{
    int tourn_size = tsize2;
    
    int poorest_idx;
    double worst_fitness = 1e20;
    
    int i = 0;
    while(i < tourn_size)
    {
	int rand_selection = (int)(get_rand() * population_size);

	if(population[rand_selection].f < worst_fitness)
	{
	    worst_fitness = population[rand_selection].f;
	    poorest_idx = rand_selection;
	}

	++i;
    }

    return poorest_idx;
}


int tournament_select()
{
    int tourn_size = tsize;

    int fittest_idx;
    double best_fitness = -1;

    int i = 0;
    while(i < tourn_size)
    {
	int rand_selection = (int)(get_rand() * population_size);

	if(population[rand_selection].f > best_fitness)
	{
	    best_fitness = population[rand_selection].f;
	    fittest_idx = rand_selection;
	}

	++i;
    }

    return fittest_idx;
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
    
    clauses = (int**)malloc((n_clauses + 1) * sizeof(int*));
    
    if (clauses == 0)
    {
	perror("Unable to allocate memory.");
	exit(2);
    }

    for (int i = 1; i <= n_clauses; ++i)
    {
	clauses[i] = (int*)malloc(3 * sizeof(int));

	if (clauses[i] == 0)
	{
	    perror("Unable to allocate memory.");
	    exit(2);
	}
    }

    for (int i = 1; i <= n_clauses; ++i)
    {
	int j = 0;

	for (; j < 3; ++j)
	{
	    scanf("%d", &clauses[i][j]);
	}

	scanf("%d", &j);
    }

    for (int i = 0; i < population_size; ++i)
    {
	sol_init(&population[i]);
	sol_init(&best[i]);
    }
}


void finalize(void)
{
    for (int i = 1; i <= n_clauses; ++i)
    {
	free(clauses[i]);
	clauses[i] = 0;
    }

    free(clauses);
    clauses = 0;
}


void crossover(sol* p1, sol* p2, sol* ch)
{
    for (int i = 1; i <= n_vars; ++i)
    {
	if (get_rand() > 0.5)
	{
	    ch->x[i] = p1->x[i];
	}
	else
	{
	    ch->x[i] = p2->x[i];
	}
    }
}


void evaluate_sol(sol* s)
{
    s->objs[0] = 0;
    s->objs[1] = 0;
    s->f = 0;
     
    for (int i = 1; i <= n_vars; ++i)
	if (s->x[i] == '1')
	    s->objs[1]++; // n_positives
    
    for (int i = 1; i <= n_clauses; ++i)
    {
	for (int j = 0; j < 3; ++j)
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
    for (int i = 1; i <= n_clauses; ++i)
    {
	printf("c%d: ", i);

	for (int j = 0; j < 3; ++j)
	{
	    printf("%d ", clauses[i][j]);
	}

	puts("");
    }
}


double population_hv(int* front_idx_ret)
{
    double ref[] = {0.0, 0.0};
    double front[200];

    int front_idx = 0;

    int front_size = 1;

    int front_f = population[population_size - 1].f;

    front[front_idx++] = -population[population_size - 1].objs[0];
    front[front_idx++] = -population[population_size - 1].objs[1];

    for (int i = population_size - 2; i > 0; --i)
    {
	if (population[i].f == front_f ||
	    (population[i].objs[0] == population[i + 1].objs[0]) && (population[i].objs[1] == population[i + 1].objs[1]))
	{
	    ++front_size;
	    front[front_idx++] = -population[i].objs[0];
	    front[front_idx++] = -population[i].objs[1];
	}
	else
	{
	    break;
	}
    }
    
    *front_idx_ret = front_idx;
    return fpli_hv(front, 2, front_idx, ref);
}


int main(int argc, char** argv)
{
    if(argc != 6)
    {
	perror("Arguments: seed max_iters pergenome tsize1 tsize2");
	exit(1);
    }
    
    long seed = atol(argv[1]);
    int max_iters = atoi(argv[2]);
    pergenome = atof(argv[3]);
    tsize = atoi(argv[4]);
    tsize2 = atoi(argv[5]);
    
    srand(seed);
    initialize();
    
    for (int i = 0; i < population_size; ++i)
	evaluate_sol(&population[i]);
    
    nondominated_sorting();
    
    int  best_ever = -1;
    int front_idx;

    sol mutant;
    
    for(int i = population_size; i <= max_iters; ++i)
    {
	int parent_a=tournament_select();

	if(get_rand() < 0.7)  // this is p_c the crossover probability
	{
	    int parent_b = tournament_select();
	    crossover(&population[parent_a], &population[parent_b], &mutant); //sexual reproduction
	}
	else
	    mutant = population[parent_a]; // clone the parent 
	
	mutate(&mutant); // always mutate
	
	evaluate_sol(&mutant);
	population[tourn_worst()] = mutant;
	nondominated_sorting();
	
	int fit = population_hv(&front_idx);
	
	if(i % 1000 == 1)
	{
	    //print_population();
	    printf("%d Fitness= %d Best= %d front_idx= %d\n", i, fit, best_ever, front_idx);
	}
	
	if(fit > best_ever)
	{
	    //printf("%d Fitness= %d Best= %d front_idx= %d\n", i, fit, best_ever, front_idx);
	    best_ever = fit;
	    
	    for (int j = 0; j < population_size; ++j)
		best[j] = population[j];
	}

	fflush(stdout);
    }

    for (int i = 0; i < population_size; ++i)
	population[i] = best[i];

    nondominated_sorting();
    
    print_population();
    printf("Best ever = %d\n", best_ever);

    FILE* all = fopen("all.dat", "w");

    for (int i = 0; i < population_size; ++i)
	fprintf(all, "%d %d\n", population[i].objs[0], population[i].objs[1]);

    fclose(all);

    FILE* nondominated = fopen("nondominated.dat", "w");

    int front_f = population[population_size - 1].f;    

    fprintf(nondominated, "%d %d\n", population[population_size -1 ].objs[0], population[population_size - 1].objs[1]);

    for (int i = population_size - 2; i > 0; --i)
    {
	if (population[i].f == front_f)
	{
	    fprintf(nondominated, "%d %d\n", population[i].objs[0], population[i].objs[1]);
	}
	else if ((population[i].objs[0] == population[i + 1].objs[0]) && (population[i].objs[1] == population[i + 1].objs[1]))
	{
	    fprintf(nondominated, "%d %d\n", population[i].objs[0], population[i].objs[1]);
	}
	else
	{
	    break;
	}
    }

    fclose(nondominated);

    finalize();
}

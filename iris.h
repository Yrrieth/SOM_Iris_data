#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NUMBER_DATA 4

typedef struct iris {
	double* value;
	double size_line;
	char* name;
	int index;
} irisData_t;

// La structure irisRand_t est utilisée pour obtenir un tableau d'adresse d'irisData_t
typedef struct irisRand {
	int index;
	irisData_t* irisDataTab;
} irisRand_t;

typedef struct net {
	double *capteur;
	double alpha;
	irisData_t **map;
	int neighborhood; // Rayon de voisinage
} net_t;

typedef struct dist_eucli {
	double distance;
	int x;
	int y;
} dist_eucli_t;

// Prototype iris.c
irisData_t* allocIrisData_t(int size);
int count_number_line(FILE* file_opened);
void print_data(irisData_t* iris_tab, int number_line);
void print_shuffle_data(irisRand_t *iris_shuffled, int number_line);
void take_line(FILE* file_opened, irisData_t* iris_tab);
void normalize(irisData_t *iris_norm, int number_line);
void shuffle_data(irisData_t *iris_tab, irisRand_t *iris_shuffled, int number_line);
irisData_t* average (irisData_t *iris_tab, int number_line);
irisData_t* interval_bound (irisData_t *iris_tab, int boolean, double interval);

net_t* allocNet_t (int size_horiz, int size_verti);
void print_net_map_node(net_t *net, int number_node_horiz, int number_node_verti);
net_t* random_in_interval (irisData_t *iris_interval_lower, irisData_t *iris_interval_upper, int number_node_horiz, int number_node_verti);
void take_one_random_data(irisRand_t *iris_shuffled, net_t *net, int number_line);

dist_eucli_t** allocdist_eucli_t (int size_horiz, int size_verti);
dist_eucli_t** find_bmu (net_t *net, int number_node_horiz, int number_node_verti);
void apprentissage (int iteration_max, irisRand_t *iris_shuffled, net_t *net, int number_line, int number_node_horiz, int number_node_verti);
void voisin (net_t *net, dist_eucli_t **bmu, int number_node_horiz, int number_node_verti, int alpha);
void etiquettage (net_t *net, irisRand_t *iris_shuffled, int number_line, int number_node_horiz, int number_node_verti, int** resultat);

// Prototype main.c
int main();
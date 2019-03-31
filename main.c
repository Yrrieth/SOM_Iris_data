#include "iris.h"

int main() {
	irisData_t *iris_tab;
	irisRand_t *iris_shuffled;
	irisData_t *iris_average;
	int number_line;

	irisData_t *iris_interval_upper;
	irisData_t *iris_interval_lower;
	double interval = 0.1f;

	net_t *net;
	irisData_t *nodes;

	int number_node = 10;

	FILE* file_opened = fopen("iris.data", "r");
	if (!file_opened) {
		printf("Can't open this file !\n");
		exit(1);
	}

	number_line = count_number_line(file_opened);

	iris_tab = allocIrisData_t(number_line);

	take_line(file_opened, iris_tab);

	print_data(iris_tab, number_line);

	printf("\n/////////////////////////////////////////////\n\nNorm of the vector:\n\n");

	normalize(iris_tab, number_line);

	print_data(iris_tab, number_line);

	printf("\n/////////////////////////////////////////////\n\nShuffle the vector:\n\n");

	iris_shuffled = malloc(number_line * sizeof(irisRand_t));

	srand(time(NULL));
	
	shuffle_data(iris_tab, iris_shuffled, number_line);

	print_shuffle_data(iris_shuffled, number_line);

	iris_average = average(iris_tab, number_line);

	printf("\nVecteur moyen\n");
	print_data(iris_average, 1);

	iris_interval_upper = interval_bound(iris_average, 1, interval);
	iris_interval_lower = interval_bound(iris_average, 0, interval);

	printf("\nLimite inférieure\n");
	print_data(iris_interval_lower, 1);
	printf("\nLimite supérieure\n");
	print_data(iris_interval_upper, 1);

	printf("a\n");

	printf("\nVecteur de neurones\n");
	net = random_in_interval(iris_interval_lower, iris_interval_upper, number_node);
	net->neighborhood = 3; // la variable net est alloué dans la fonction rand_in_interval, donc on peut met cette assignation ici

	print_net_map_node(net, number_node);

	printf("\nOrdonnancement\n");

	int iteration_max = 500;

	apprentissage (iteration_max, iris_shuffled, net, number_line, number_node);

	int** resultat;
	resultat = malloc(number_node * sizeof(int*));
	for (int i = 0; i < number_node; i++) {
		resultat[i] = malloc(number_node * sizeof(int));
	}

	etiquettage (net, iris_shuffled, number_line, number_node, resultat);

	printf("\nAffinage\n");
	apprentissage (1500, iris_shuffled, net, number_line, number_node);

	etiquettage (net, iris_shuffled, number_line, number_node, resultat);



	fclose(file_opened);
	free(iris_tab);
	free(iris_shuffled);
	return 0;
}
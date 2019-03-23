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

	print_net_map_node(net, number_node);

	printf("\nVecteur de donnée aléatoire\n");
	irisData_t* test_bmu;
	test_bmu = take_one_random_data(iris_shuffled, number_line);
	print_data(test_bmu, 1);

	printf("\nbmu\n");
	bmu(test_bmu, net, number_node);


	printf("z\n");
	fclose(file_opened);
	free(iris_tab);
	free(iris_shuffled);
	return 0;
}
#include "iris.h"

irisData_t* allocIrisData_t(int size) {
	irisData_t* tmp;
	int i;
	tmp = malloc(size * sizeof(irisData_t));
	for(i = 0; i < size; i++) {
		tmp[i].value = malloc(4 * sizeof(double));
		tmp[i].name = malloc(100 * sizeof(char));
	}
	return tmp;
}

int count_number_line(FILE* file_opened) {
	int current_line = 0;
	int i = 0;
	while (1) {
		current_line = fgetc(file_opened);
		if (current_line == '\n') {
			i++;
		}
		if (current_line == EOF) {
			i++;
			break;
		}
	}
	return i;
}

void print_data(irisData_t* iris_tab, int number_line) {
	int i, j = 0;
	for(i = 0; i < number_line; i++) { // General print loop
		printf("%d : ", iris_tab[i].index);
		for (j = 0; j < 4; j++) { // print value
			printf("%f ", iris_tab[i].value[j]);
		}
		printf(" %s\n", iris_tab[i].name); // print name
	}
}

void print_shuffle_data(irisRand_t *iris_shuffled, int number_line) {
	int i, j;
	for (i = 0; i < number_line; i++) {
		printf("%d : ", iris_shuffled[i].index);
		for (j = 0; j < 4; j++) { // print value
			printf("%f ", iris_shuffled[i].irisDataTab->value[j]);
		}
		printf(" %s\n", iris_shuffled[i].irisDataTab->name); // print name
	}
}

void take_line(FILE* file_opened, irisData_t* iris_tab) {
	int size_current_line = 100;
	char * current_line = malloc(size_current_line * sizeof(char));
	fseek(file_opened, 0, SEEK_SET); // Return to the first character of the file

	int i = 0, j = 0;

	while (fgets(current_line, size_current_line, file_opened) != NULL) {
		iris_tab[i].size_line = strlen(current_line);
		iris_tab[i].index = i;

		if (iris_tab[i].size_line > 1) {
			j = 0; // Reset j
			iris_tab[i].value[j] = atof(strtok(current_line, ","));
			for(j = 1; j < 4; j++) {
				iris_tab[i].value[j] = atof(strtok(NULL, ","));
			}
			iris_tab[i].name = strdup(strtok(NULL, "\n"));
		}
		i++;
	}
}

void normalize(irisData_t *iris_norm, int number_line) {
	double norm_vector = 0;
	int i, j;

	for (i = 0; i < 4; i++) {
		norm_vector += pow(iris_norm[0].value[i], 2);
	}

	norm_vector = sqrt(norm_vector); // Ici, j'ai la norme du vecteur

	for (i = 0; i < number_line; i++) {
		for (j = 0; j < 4; j++) {
			iris_norm[i].value[j] = iris_norm[i].value[j] / norm_vector; 
		}
	}
}

void shuffle_data(irisData_t *iris_tab, irisRand_t *iris_shuffled, int number_line) {
	irisRand_t tmp;
	int i, j;
	int num; // Random number

	// Initialize the vector iris_shuffled
	for (int i = 0; i < number_line; i++) {
		iris_shuffled[i].index = i;
		iris_shuffled[i].irisDataTab = &iris_tab[i]; // Get the adress of the pointer
	}

	// Shuffle the data of the vector
	for (i = 0; i < number_line; i++) {
		num = (rand() % (number_line - i)) + i; // On choisit un nombre aléatoire en excluant les index déjà remplacés
		for(j = 0; j <= num; j++) {
			if (j == num) {
				tmp = iris_shuffled[i];
				iris_shuffled[i] = iris_shuffled[j];
				iris_shuffled[j] = tmp;
			}
		}
	}
}

irisData_t* average (irisData_t *iris_tab, int number_line) {
	irisData_t *iris_average = allocIrisData_t(1);
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < number_line; j++) {
			iris_average->value[i] += iris_tab[j].value[i];
		}
		iris_average->value[i] /= number_line;
	}
	return iris_average;
}

irisData_t* interval_bound (irisData_t *iris_tab, int boolean, double interval) {
	int i;
	irisData_t* iris_interval = allocIrisData_t(1);
	for (i = 0; i < 4; i++) {
		iris_interval->value[i] = iris_tab->value[i];
		if (boolean == 1) {
			iris_interval->value[i] += interval;
		} else {
			iris_interval->value[i] -= interval;
		}
	}
	return iris_interval;
}

net_t* allocNet_t (int size) {
	net_t* tmp;
	int i;
	tmp = malloc(sizeof(net_t));
	tmp->pointer = malloc(4 * sizeof(double));
	tmp->map = malloc(size * sizeof(irisData_t*));
	for (i = 0; i < size; i++) {
		tmp->map[i] = malloc(size * sizeof(irisData_t));
	}
	return tmp;
}

void print_net_map_node(net_t *net, int number_node) {
	int i, j , k;
	for (i = 0; i < number_node; i++) {
		for (j = 0; j < number_node; j++) {
			for(k = 0; k < 4; k++) {
				printf("i : %d, j : %d, k : %d = %f\n",i, j, k, net->map[i][j].value[k]);
			}
		}
	}
}

net_t* random_in_interval (irisData_t *lower, irisData_t *upper, int number_node) {
	int i, j, k;
	double num; // Random number
	net_t* net = allocNet_t(number_node); // Contient la variable map qui est un tab à 2 dimensions
	int number_node_total = number_node * number_node;
	irisData_t *node = allocIrisData_t(number_node_total); // Tableau à 1 dimension

	for (i = 0; i < number_node_total; i++) {
		for (j = 0; j < 4; j++) {
			num = (rand() / (double)RAND_MAX) * (upper->value[j] - lower->value[j]) + lower->value[j];
			node[i].value[j] = num;
		}
	}

	for (i = 0; i < number_node; i++) {
		for (j = 0; j < number_node; j++) {
			for(k = 0; k < 4; k++) {
				net->map[i][j] = node[i * number_node + j];
			}
		}
	}
	return net;
}

void take_one_random_data(irisRand_t *iris_shuffled, net_t *net, int number_line) {
	int i, j;
	int num = rand() % number_line;

	for (i = 0; i < number_line; i++) {
		for (j = 0; j < 4; j++) {
			if (num == i) {
				net->pointer[j] = iris_shuffled[i].irisDataTab->value[j];
			}
		}
	}
}

bmu_t** allocBmu_t (int size) {
	bmu_t **tmp;
	int i;
	tmp = malloc(size * sizeof(bmu_t));
	for (i = 0; i < size; i++) {
		tmp[i] = malloc(size * sizeof(bmu_t));
	}
	return tmp;
}

bmu_t** find_bmu (net_t *net, int number_node) {
	int i, j, k;
	double distance;

	bmu_t **distance_tab;
	distance_tab = allocBmu_t(number_node);

	bmu_t **bmu;
	bmu = allocBmu_t(1);
	bmu[0][0].distance = 100.0;
	

	/**
	 * Calcule la distance euclidienne entre un vecteur de donnée (net->pointer) 
	 * et chacun des vecteurs de neurone de la map
	 */
	for (i = 0; i < number_node; i++) { 
		for (j = 0; j < number_node; j++) {
			distance = 0.0;
			for (k = 0; k < 4; k++) {
				distance = distance + pow(fabs(net->map[i][j].value[k] - net->pointer[k]), 2);
			}
			distance = sqrt(distance);
			//printf("i : %d, j: %d, Distance euclidienne : %f\n", i, j, distance);
			distance_tab[i][j].distance = distance;
			distance_tab[i][j].x = i;
			distance_tab[i][j].y = j;
		}
	}
	for (i = 0; i < number_node; i++) {
		for (j = 0; j < number_node; j++) {
			if (distance_tab[i][j].distance < bmu[0][0].distance) {
				bmu[0][0].distance = distance_tab[i][j].distance;
				bmu[0][0].x = distance_tab[i][j].x;
				bmu[0][0].y = distance_tab[i][j].y;
			} else if (distance_tab[i][j].distance == bmu[0][0].distance) {
				printf("crée un autre tab\n\n\n\n\n");
			}
		}
	}
	//printf("bmu = %f ", bmu[0][0].distance); // Renvoyer aussi les coordonées ? Struct coord ??
	return bmu;
}

void voisin (net_t *net, bmu_t **bmu, int number_node) {
	int i, j, k;
	double voisin; // Distance euclidienne entre un noeud et le bmu

	irisData_t *bmu_value = allocIrisData_t(1);

	for (i = 0; i < number_node; i++) {
		for (j = 0; j < number_node; j++) {
			if (i == bmu[0][0].x && j == bmu[0][0].y) {
				for (k = 0; k < 4; k++) {
					bmu_value[0].value[k] = net->map[i][j].value[k];
					printf("%f ", bmu_value[0].value[k]);
				}
			}
		}
	}

	for (i = 0; i < number_node; i++) {
		for (j = 0; j < number_node; j++) {
			voisin = 0.0;
			for (k = 0; k < 4; k++) {
				voisin += pow(fabs(net->map[i][j].value[k] - bmu_value[0].value[k]), 2);
			}
			voisin = sqrt(voisin);
			if (voisin < net->neighborhood && voisin != 0)
				printf(" voisin = %f \n", voisin);
		}
	}
}

void apprentissage (int iteration_max, irisRand_t *iris_shuffled, net_t *net, int number_line, int number_node) {
	int i;
	double alpha = 0.0;
	bmu_t **bmu;
	for (i = 0; i < iteration_max; i++) {
		printf("%d : ", i);
		take_one_random_data(iris_shuffled, net, number_line);
		bmu = find_bmu (net, number_node);
		voisin(net, bmu, number_node);

		alpha = 1.0 - ((double)i / (double)iteration_max);
		//printf("bmu = %f, x = %d, y = %d, a = %f\n", bmu[0][0].distance, bmu[0][0].x, bmu[0][0].y, alpha);
	}
}
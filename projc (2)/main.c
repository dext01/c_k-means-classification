#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmeans.h"

void visualize_with_gnuplot(Point* points, int* labels, Point* centroids, int n, int k);
void generate_random_points(Point* points, int n);

unsigned int seed; // глобальная переменная seed

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <number_of_points> <number_of_clusters> <seed>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // количество точек
    int k = atoi(argv[2]); // количество кластеров
    seed = (unsigned int)atoi(argv[3]); // seed передаётся через аргументы

    if (n <= 0 || k <= 0 || k > n) {
        printf("Invalid values: ensure n > 0, k > 0, and k <= n\n");
        return 1;
    }

    // выделяем память
    Point *points = malloc(n * sizeof(Point));
    if (points == NULL) {
        printf("Memory allocation failed for points\n");
        return 1;
    }

    int *labels = malloc(n * sizeof(int));
    if (labels == NULL) {
        printf("Memory allocation failed for labels\n");
        free(points);
        return 1;
    }

    Point *centroids = malloc(k * sizeof(Point));
    if (centroids == NULL) {
        printf("Memory allocation failed for centroids\n");
        free(points);
        free(labels);
        return 1;
    }

    // генерация точек
    generate_random_points(points, n);

    // K-means алгоритм
    kmeans(points, n, k, 100, labels, centroids);

    // запись результатов
    FILE *outputFile = fopen("clustering_result.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening output file\n");
        free(points);
        free(labels);
        free(centroids);
        return 1;
    }

    fprintf(outputFile, "Final Centroids:\n");
    for (int i = 0; i < k; i++) {
        fprintf(outputFile, "Cluster %d: (%.2f, %.2f)\n", i, centroids[i].x, centroids[i].y);
    }

    fprintf(outputFile, "\nPoints in each cluster:\n");
    for (int i = 0; i < k; i++) {
        fprintf(outputFile, "Cluster %d: ", i);
        for (int j = 0; j < n; j++) {
            if (labels[j] == i) {
                fprintf(outputFile, "(%.2f, %.2f) ", points[j].x, points[j].y);
            }
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    // визуализация
    visualize_with_gnuplot(points, labels, centroids, n, k);

    // освобождаем память
    free(points);
    free(labels);
    free(centroids);

    return 0;
}

// функция генерации точек с использованием глобального seed
void generate_random_points(Point* points, int n) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        points[i].x = (double)rand() / RAND_MAX * 100.0;
        points[i].y = (double)rand() / RAND_MAX * 100.0;
    }
}

// визуализация через gnuplot
void visualize_with_gnuplot(Point* points, int* labels, Point* centroids, int n, int k) {
    FILE* gp = _popen("gnuplot -persistent", "w");

    if (gp == NULL) {
        printf("Gnuplot not found or not working\n");
        return;
    }

    fprintf(gp, "set title 'K-Means Clustering'\n");
    fprintf(gp, "set xlabel 'X'\n");
    fprintf(gp, "set ylabel 'Y'\n");
    fprintf(gp, "set key outside\n");
    fprintf(gp, "set grid\n");

    fprintf(gp, "plot ");
    for (int i = 0; i < k; i++) {
        if (i > 0) fprintf(gp, ", ");
        fprintf(gp, "'-' with points pointtype 7 title 'Cluster %d'", i);
    }
    fprintf(gp, ", '-' with points pointtype 2 lc rgb 'black' pointsize 2 title 'Centroids'\n");

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            if (labels[j] == i) {
                fprintf(gp, "%f %f\n", points[j].x, points[j].y);
            }
        }
        fprintf(gp, "e\n");
    }

    for (int i = 0; i < k; i++) {
        fprintf(gp, "%f %f\n", centroids[i].x, centroids[i].y);
    }
    fprintf(gp, "e\n");

    _pclose(gp);
}
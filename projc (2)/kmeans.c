#include "kmeans.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

double distance(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

Point compute_centroid(Point* cluster_points, int count) {
    double sum_x = 0.0, sum_y = 0.0;
    for (int i = 0; i < count; i++) {
        sum_x += cluster_points[i].x;
        sum_y += cluster_points[i].y;
    }
    Point centroid = {sum_x / count, sum_y / count};
    return centroid;
}

void kmeans(Point* data, int n_points, int k, int max_iters, int* labels, Point* centroids) {
    // инициализация центров случайными точками
    for (int i = 0; i < k; i++) {
        centroids[i] = data[i];
    }

    for (int iter = 0; iter < max_iters; iter++) {
        // распределение точек по кластерам
        for (int i = 0; i < n_points; i++) {
            double min_dist = distance(data[i], centroids[0]);
            labels[i] = 0;
            for (int j = 1; j < k; j++) {
                double dist = distance(data[i], centroids[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    labels[i] = j;
                }
            }
        }

        // пересчёт центров кластеров
        Point new_centroids[k];
        int count[k];
        for (int i = 0; i < k; i++) {
            new_centroids[i] = (Point){0.0, 0.0};
            count[i] = 0;
        }

        for (int i = 0; i < n_points; i++) {
            int cluster = labels[i];
            new_centroids[cluster].x += data[i].x;
            new_centroids[cluster].y += data[i].y;
            count[cluster]++;
        }

        for (int i = 0; i < k; i++) {
            if (count[i] > 0) {
                new_centroids[i].x /= count[i];
                new_centroids[i].y /= count[i];
            }
        }

        // если центры не изменились, то завершаем
        int converged = 1;
        for (int i = 0; i < k; i++) {
            if (new_centroids[i].x != centroids[i].x || new_centroids[i].y != centroids[i].y) {
                converged = 0;
                break;
            }
        }

        if (converged) break;

        // обновляем центры
        for (int i = 0; i < k; i++) {
            centroids[i] = new_centroids[i];
        }
    }
}
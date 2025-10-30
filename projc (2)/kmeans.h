#ifndef KMEANS_H
#define KMEANS_H

typedef struct {
    double x;
    double y;
} Point;

void kmeans(Point* data, int n_points, int k, int max_iters, int* labels, Point* centroids);
double distance(Point a, Point b);
Point compute_centroid(Point* cluster_points, int count);

#endif // KMEANS_H

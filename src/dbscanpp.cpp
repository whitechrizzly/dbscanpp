#include "dbscanpp.h"

/**
 * Runs the DBSCAN algorithm.
 *
 * A cluster will be created if there is a minimum number of points `minPts`
 * which have a distance less or equal than `eps` to one of its neighbors.
 *
 * @param clusters_p Vector which contains the found clusters.
 * @param dataset The input data set.
 * @param eps The maximum allowed distance between two points.
 * @param minPts The minimum number of points needed to create a cluster.
 */
void DBSCAN::process(std::vector<Cluster>* const clusters_p, std::vector<Point<float>>& dataset, const float& eps, const size_t& minPts) {
    this->dataset_p = &dataset;
    this->eps_p = &eps;
    this->minPts_p = &minPts;
    int clusterId = NO_CLUSTER;
    for (Point<float>& testPoint: *dataset_p) {
        if (testPoint.visited) {
            continue;
        }
        testPoint.visited = true;
        std::vector<Point<float>*> neighbors;
        regionQuery(&neighbors, testPoint);
        if (neighbors.size() < *minPts_p) {
            testPoint.noise = true;
        } else {
            clusters_p->push_back(Cluster(++clusterId));
            expandCluster(testPoint, neighbors, clusters_p->back());
        }
    }
}

/**
 * Adds all matching points to the given cluster.
 *
 * @param p The selected point.
 * @param neighbors All neighbors of the selected point including the point itself.
 * @param c The cluster to which the points will be added.
 */
void DBSCAN::expandCluster(Point<float>& p, std::vector<Point<float>*>& neighbors, Cluster& c) {
    p.clusterId = c.id;
    c.push_back(&p);
    for (size_t i = 0; i < neighbors.size(); ++i) {
        Point<float>& testPoint = *neighbors.at(i);
        if (!testPoint.visited) {
            testPoint.visited = true;
            std::vector<Point<float>*> testNeighbors;
            regionQuery(&testNeighbors, testPoint);
            if (testNeighbors.size() >= *minPts_p) {
                neighbors.insert(neighbors.end(), testNeighbors.cbegin(), testNeighbors.cend());
            }
        }
        if (testPoint.clusterId == NO_CLUSTER) {
            testPoint.clusterId = c.id;
            c.push_back(&testPoint);
        }
    }
}

/**
 * Returns all neighbors of the selected point.
 *
 * @param neighbors_p All neighbors of the selected point including the point itself.
 * @param p The selected point.
 */
void DBSCAN::regionQuery(std::vector<Point<float>*>* const neighbors_p, const Point<float>& p) {
    for (Point<float>& testPoint : *dataset_p) {
        if (p.getDistance(testPoint) <= *eps_p) {
            neighbors_p->push_back(&testPoint);
        }
    }
}

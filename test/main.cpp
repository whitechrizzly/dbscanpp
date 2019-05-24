#include "dbscanpp.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv)
{
    /** The input data for DBSCAN. */
    std::vector<Point<float>> dataset;
    /** The core points. Some random neigbors will be created afterwards. */
    std::vector<Point<float>> corePoints({
                                             Point<float>({0.f, 0.f}),
                                             Point<float>({2.f, 3.f}),
                                             Point<float>({7.f, 4.f}),
                                         });
    /** For randomness. */
    srand(time(NULL));

    /** Create ten random neighbors for each core point. `*/
    for (Point<float>& corePoint : corePoints) {
        dataset.push_back(corePoint);
        for (size_t i = 0; i < 10; ++i) {
            dataset.push_back(Point<float>({
                                               corePoint.x + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f,
                                               corePoint.y + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f
                                           }));
        }
    }

    /** The DBSCAN parameters. */
    const float eps = 0.3f;
    const size_t minPts = 3;

    /** Create cluster vector which will be filled by DBSCAN. */
    std::vector<Cluster> clusters;

    /** The DBSCAN instance. */
    DBSCAN dbScan;

    /** Run DBSCAN algorithm. */
    dbScan.process(&clusters, dataset, eps, minPts);

    /** Print the found clusters and its member points. */
    for (const Cluster& cluster : clusters) {
        std::cout << "Cluster " << cluster.id << std::endl;
        for (const Point<float>* point : cluster) {
            std::cout << "  (" << point->x << "," << point->y << "," << point->z << ")" << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }

    /** Print points which were classified as noise. */
    std::cout << "Noise" << std::endl;
    for (const Point<float>& noisyPoint : dataset) {
        if (noisyPoint.noise) {
            std::cout << "  (" << noisyPoint.x << "," << noisyPoint.y << "," << noisyPoint.z << ")" << std::endl;
        }
    }

    return 0;
}

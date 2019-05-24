#include <vector>
#include <ctype.h>
#include <cmath>
#include <cassert>

/** Indicates that a point does not belong to any cluster. */
static const int NO_CLUSTER = -1;

/** Definition of a 3D point. */
template <typename T>
struct Point {
    T x = 0;
    T y = 0;
    T z = 0;
    int clusterId = NO_CLUSTER;
    bool visited = false;
    bool noise = false;

    /**
     * Constructs point from std::vector.
     *
     * @param vec The x,y,z position of the point.
     */
    Point(std::vector<T> vec) {
        assert(vec.size() == 2 || vec.size() == 3);
        this->x = vec[0];
        this->y = vec[1];
        if (vec.size() == 3) {
            this->z = vec[2];
        }
    }

    /**
     * Calculates the distance between this and another point.
     *
     * @param p The other point.
     * @return Euclidean distance between this and the other point.
     */
    float getDistance(const Point& p) const {
        return std::sqrt(std::pow(p.x-x, 2) + std::pow(p.y-y, 2) + std::pow(p.z-z, 2));
    }
};

/**
 * Definition of a cluster.
 */
struct Cluster : public std::vector<Point<float>*> {
    /** The cluster id. */
    int id;

    /**
     * Constructs cluster with the given id.
     *
     * @param The cluster id.
     */
    Cluster(const int& id) {
        this->id = id;
    }
};

/**
 * Declaration of the DBSCAN class.
 */
class DBSCAN {
public:
    /** The input data set which contains the points to be clustered. */
    std::vector<Point<float>>* dataset_p;
    /** The maximum allowed distance between two neighbors. */
    const float* eps_p;
    /** The minimum number of points to create a cluster. */
    const size_t* minPts_p;

    /** Runs the DBSCAN algorithm. */
    void process(std::vector<Cluster>* const clusters_p, std::vector<Point<float>>& dataset, const float& eps, const size_t& minPts);

private:
    /** Adds all matching points to the given cluster. */
    void expandCluster(Point<float>& p, std::vector<Point<float>*>& neighbors, Cluster& c);
    /** Returns all neighbors of the selected point. */
    void regionQuery(std::vector<Point<float>*>* const neighbors_p, const Point<float>& p);
};

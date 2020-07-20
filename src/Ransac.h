#ifndef RANSACCUSTOM_H_
#define RANSACCUSTOM_H_

#include <unordered_set>
#include <pcl/common/common.h>

template<typename PointT>
class Ransac {
private:
    int maxIterations;
    float distanceTol;
    int num_points;

public:
    //constructor:
    Ransac();
    ~Ransac();
    std::unordered_set<int> Ransac3D(typename pcl::PointCloud<PointT>::Ptr cloud, int maxIterations, float distanceTol);
};
#endif
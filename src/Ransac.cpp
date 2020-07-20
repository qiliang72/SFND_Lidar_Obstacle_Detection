#include "Ransac.h"

template<typename PointT>
Ransac<PointT>::Ransac() {}


template<typename PointT>
Ransac<PointT>::~Ransac() {}


// custom helper function for segmentation
template<typename PointT>
std::unordered_set<int> Ransac<PointT>::Ransac3D(typename pcl::PointCloud<PointT>::Ptr cloud, int maxIterations, float distanceTol)
{
  auto startTime = std::chrono::steady_clock::now();

  std::vector<int> inliersMax;
  std::unordered_set<int> inliersResult;
  srand(time(NULL));
  
  // TODO: Fill in this function

  // For max iterations 
  for (int i = 0; i < maxIterations; i++)
  {
    // Randomly sample subset and fit line
    /*std::vector<int> inliers;
    while (inliers.size() < 3)
      inliers.push_back(rand()%(cloud->points.size()));*/
    std::unordered_set<int> inliers;
    while (inliers.size() < 3)
      inliers.insert(rand()%(cloud->points.size()));

    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    auto iter = inliers.begin();
    x1 = cloud->points[*iter].x;
    y1 = cloud->points[*iter].y;
    z1 = cloud->points[*iter].z;
    iter++;
    x2 = cloud->points[*iter].x;
    y2 = cloud->points[*iter].y;
    z2 = cloud->points[*iter].z;
    iter++;
    x3 = cloud->points[*iter].x;
    y3 = cloud->points[*iter].y;
    z3 = cloud->points[*iter].z;

    float x21 = x2 - x1;
    float y21 = y2 - y1;
    float z21 = z2 - z1;
    float x31 = x3 - x1;
    float y31 = y3 - y1;
    float z31 = z3 - z1;

    float a = y21 * z31 - z21 * y31;
    float b = z21 * x31 - x21 * z31;
    float c = x21 * y31 - y21 * x31;
    float d = - (a * x1 + b * y1 + c * z1);
    float e = 1 / sqrt(a * a + b * b + c * c);

    // Measure distance between every point and fitted plane
    for (int index = 0; index < cloud->points.size(); index++)
    {
      if (inliers.count(index)>0)
        continue;

      float x4 = cloud->points[index].x;
      float y4 = cloud->points[index].y;
      float z4 = cloud->points[index].z;

      float dist = fabs(a * x4 + b * y4 + c * z4 + d) * e ;  // e = 1 / sqrt(a * a + b * b + c * c);

      // If distance is smaller than threshold count it as inlier
      if (dist < distanceTol)
        //inliers.push_back(index);
        inliers.insert(index);
    }

    // Return indicies of inliers from fitted line with most inliers
    if (inliers.size() > inliersResult.size())
      //inliersMax = inliers;
      inliersResult = inliers;
  }

  /*for (int i = 0; i < inliersMax.size(); i++)
  {
    inliersResult.insert(inliersMax[i]);
  }*/

  auto endTime = std::chrono::steady_clock::now();
  auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
  std::cout << "Ransac took " << elapsedTime.count() << " milliseconds" << std::endl;
  
  return inliersResult;
}

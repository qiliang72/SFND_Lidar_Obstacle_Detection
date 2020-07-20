#include "kdtree_cluster.h"


template<typename PointT>
class ClusterCustom {
public:
    ClusterCustom();
    ~ClusterCustom();

    std::vector<typename pcl::PointCloud<PointT>::Ptr> EuclideanCluster(typename pcl::PointCloud<PointT>::Ptr cloud, float distanceTol, int minSize, int maxSize);
    void clusterHelper(int i, typename pcl::PointCloud<PointT>::Ptr cloud, std::vector<int>& cluster_idx, std::vector<bool>& processed, KdTree<PointT>* tree, float distanceTol);
};


template<typename PointT>
ClusterCustom<PointT>::ClusterCustom() {}


template<typename PointT>
ClusterCustom<PointT>::~ClusterCustom() {}


template<typename PointT>
void ClusterCustom<PointT>::clusterHelper(int i, typename pcl::PointCloud<PointT>::Ptr cloud, std::vector<int>& cluster_idx, std::vector<bool>& processed, KdTree<PointT>* tree, float distanceTol)
{
    processed[i] = true;
    cluster_idx.push_back(i);

    std::vector<int> nearest = tree->search(cloud->points[i], distanceTol);

    for (int id : nearest)
    {
        if (processed[id])
            continue;

        clusterHelper(id, cloud, cluster_idx, processed, tree, distanceTol);
    }
}


template<typename PointT>
std::vector<typename pcl::PointCloud<PointT>::Ptr> ClusterCustom<PointT>::EuclideanCluster(typename pcl::PointCloud<PointT>::Ptr cloud, float distanceTol, int minSize, int maxSize)
{
    int num_points = cloud->points.size();

	  // TODO: Fill out this function to return list of indices for each cluster
    std::vector<typename pcl::PointCloud<PointT>::Ptr> clusters;

    KdTree<PointT>* tree = new KdTree<PointT>;

    for (int i=0; i<num_points; i++)
    {
        tree->insert(cloud->points[i],i);
    }

    // processed parameter
    std::vector<bool> processed(num_points, false);
    for (int i = 0; i < num_points; i++)
    {
        if (processed[i])
            continue;

        std::vector<int> cluster_idx;
        clusterHelper(i, cloud, cluster_idx, processed, tree, distanceTol);


        if ((cluster_idx.size()>=minSize) && (cluster_idx.size()<=maxSize))
        {
            typename pcl::PointCloud<PointT>::Ptr cloud_cluster (new pcl::PointCloud<PointT>);

            for(int indice: cluster_idx)
                cloud_cluster->points.push_back(cloud->points[indice]);

            cloud_cluster->width = cloud_cluster->points.size ();
            cloud_cluster->height = 1;
            cloud_cluster->is_dense = true;

            clusters.push_back(cloud_cluster);
            std::cout << "PointCloud representing the Cluster: " << cloud_cluster->points.size () << " data points." << std::endl;
        }
    }

	return clusters;

}

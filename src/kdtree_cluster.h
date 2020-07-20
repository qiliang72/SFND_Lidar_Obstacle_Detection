/* \author Aaron Brown */
// Quiz on implementing kd tree
#ifndef KDTREECLUSTER_H_
#define KDTREECLUSTER_H_

#include "render/render.h"

// Structure to represent node of kd tree
template<typename PointT>
struct Node
{
	PointT point;
	int id;
	Node<PointT>* left;
	Node<PointT>* right;

	Node(PointT arr, int setId)
	:	point(arr), id(setId), left(NULL), right(NULL)
	{}
};


template<typename PointT>
struct KdTree
{
	Node<PointT>* root;

	KdTree()
	: root(NULL)
	{}

    void insertHelper(Node<PointT>* &node, uint depth, PointT point, int id)
    {
        // tree is empty
        if (node == NULL)
        	node = new Node<PointT>(point, id);
        else
        {
        	// calculate current dim
        	int cd = depth % 3;

        	if (cd == 0)
        	{
        		if (point.x < (node->point.x))
        		    insertHelper(node->left, depth+1, point, id);
        	    else
        		    insertHelper(node->right, depth+1, point, id);
        	}
        	else if (cd == 1)
        	{
        		if (point.y < (node->point.y))
        		    insertHelper(node->left, depth+1, point, id);
        	    else
        		    insertHelper(node->right, depth+1, point, id);
        	}
        	else
        	{
        		if (point.z < (node->point.z))
        		    insertHelper(node->left, depth+1, point, id);
        	    else
        		    insertHelper(node->right, depth+1, point, id);
        	}
        }
    }

	void insert(PointT point, int id)
	{
		// TODO: Fill in this function to insert a new point into the tree
		// the function should create a new node and place correctly with in the root 
		insertHelper(root, 0, point, id);

	}

    void searchHelper(Node<PointT>* node, int depth, PointT target, float distanceTol, std::vector<int>& ids)
    {
    	// node is not empty
    	if (node != NULL)
    	{
    		float delta_x = node->point.x-target.x;
    		float delta_y = node->point.y-target.y;
    		float delta_z = node->point.z-target.z;
    		// check area near target
    		if ((delta_x >= -distanceTol && delta_x <= distanceTol) &&
    		    (delta_y >= -distanceTol && delta_y <= distanceTol) &&
    		    (delta_z >= -distanceTol && delta_z <= distanceTol))
    		{
    			// calculate distance
    			float dist = sqrt(delta_x*delta_x + delta_y*delta_y + delta_z*delta_z);
    			if (dist <= distanceTol)
    				ids.push_back(node->id);
    		}

    		// check sub node
    		if (depth % 3 == 0)
    		{
    			if (delta_x > -distanceTol)
    			    searchHelper(node->left, depth+1, target, distanceTol, ids);
    		    if (delta_x < distanceTol)
    			    searchHelper(node->right, depth+1, target, distanceTol, ids);
    		}
    		else if (depth % 3 == 1)
    		{
    			if (delta_y > -distanceTol)
    			    searchHelper(node->left, depth+1, target, distanceTol, ids);
    		    if (delta_y < distanceTol)
    			    searchHelper(node->right, depth+1, target, distanceTol, ids);
    		}
    		else
    		{
    			if (delta_z > -distanceTol)
    			    searchHelper(node->left, depth+1, target, distanceTol, ids);
    		    if (delta_z < distanceTol)
    			    searchHelper(node->right, depth+1, target, distanceTol, ids);
    		}
    	}
    }

	// return a list of point ids in the tree that are within distance of target
	std::vector<int> search(PointT target, float distanceTol)
	{
		std::vector<int> ids;

		// TODO
		searchHelper(root, 0, target, distanceTol, ids);

		return ids;
	}
	

};

#endif



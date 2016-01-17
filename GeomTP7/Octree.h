#pragma once

#include "Global.h"
#include <vector>

using namespace std;

class Octree
{
private:
	int depth;
	Octree *childrens;
	OctreeState state;
	
	point3* GetBounds(const Voxel &v);
	Voxel* Subdivise(const Voxel &v);
protected:
	Voxel voxel;
public:
	Octree();
	Octree(const point3& origin, const int size);
	~Octree();
	void createNodes(Shape& s, const float size_min, std::vector<Voxel> &data);
};


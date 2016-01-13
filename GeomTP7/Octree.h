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
	void createNodes(const Sphere& s, std::vector<Voxel> &data);
	bool isIn(const Sphere& s, point3 point);
	float* getVoxelData(point3 origin, const float size);
};


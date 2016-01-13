#include "Octree.h"

Octree::Octree()
{
	//childrens = new Octree[8];
}

Octree::Octree(const point3& origin, const int size)
{
	voxel.origin = origin;
	voxel.edge_size = size;
	//childrens = new Octree[8];
}

Octree::~Octree()
{

}

void Octree::createNodes(const Sphere& s, Voxel* data)
{
	if (isIn(s) && voxel.edge_size >= 1 ) {
		Voxel* fils = Subdivise(voxel);

		for (int i = 0; i < 8; ++i) {
			Octree o = Octree(fils[i].origin, voxel.edge_size / 2);
			o.createNodes(s, data);
		}
	}
}

bool Octree::isIn(const Sphere& s) {
	float dist = sqrtf((s.origin.x - voxel.origin.x)*(s.origin.x - voxel.origin.x) + (s.origin.y - voxel.origin.y)*(s.origin.y - voxel.origin.y) + (s.origin.z - voxel.origin.z)*(s.origin.z - voxel.origin.z));
	return dist <= s.radius;
}

point3* Octree::GetBounds(const Voxel &v)
{
	float delta = v.edge_size * 0.5f;

	return new point3[8]
	{
		point3(v.origin.x + delta, v.origin.y + delta, v.origin.z + delta),
		point3(v.origin.x - delta, v.origin.y + delta, v.origin.z + delta),
		point3(v.origin.x + delta, v.origin.y - delta, v.origin.z + delta),
		point3(v.origin.x - delta, v.origin.y - delta, v.origin.z + delta),
		point3(v.origin.x + delta, v.origin.y + delta, v.origin.z - delta),
		point3(v.origin.x - delta, v.origin.y + delta, v.origin.z - delta),
		point3(v.origin.x + delta, v.origin.y - delta, v.origin.z - delta),
		point3(v.origin.x - delta, v.origin.y - delta, v.origin.z - delta)
	};
}

Voxel* Octree::Subdivise(const Voxel &v)
{
	float size = v.edge_size * 0.5f;
	//float size = 2.f * delta;

	return new Voxel[8]
	{ 
		Voxel { point3(v.origin.x + size, v.origin.y + size, v.origin.z + size), size },
		Voxel { point3(v.origin.x - size, v.origin.y + size, v.origin.z + size), size },
		Voxel { point3(v.origin.x + size, v.origin.y - size, v.origin.z + size), size },
		Voxel { point3(v.origin.x - size, v.origin.y - size, v.origin.z + size), size },
		Voxel { point3(v.origin.x + size, v.origin.y + size, v.origin.z - size), size },
		Voxel { point3(v.origin.x - size, v.origin.y + size, v.origin.z - size), size },
		Voxel { point3(v.origin.x + size, v.origin.y - size, v.origin.z - size), size },
		Voxel { point3(v.origin.x - size, v.origin.y - size, v.origin.z - size), size }
	};
}

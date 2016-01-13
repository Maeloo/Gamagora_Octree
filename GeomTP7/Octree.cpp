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

void Octree::createNodes(const Sphere& s, std::vector<Voxel> &data)
{
	point3* vertex = GetBounds(voxel);

	int res = 0;
	for (int i = 0; i < 8; ++i) {
		if (isIn(s, vertex[i]))
			res++;
	}

	if (res == 8)
		data.push_back(voxel);
	else if (res > 0) {
		Voxel* fils = Subdivise(voxel);

		for (int i = 0; i < 8; ++i) {
			Octree o = Octree(fils[i].origin, voxel.edge_size / 2);
			o.createNodes(s, data);
		}
	}
}

bool Octree::isIn(const Sphere& s, point3 point) {
	float dist = sqrtf((s.origin.x - point.x)*(s.origin.x - point.x) + (s.origin.y - point.y)*(s.origin.y - point.y) + (s.origin.z - point.z)*(s.origin.z - point.z));
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

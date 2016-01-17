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

void Octree::createNodes(Shape& s, const float size_min, std::vector<Voxel> &data)
{
	if (voxel.edge_size >= size_min)
	{
		Voxel* children = Subdivise(voxel);

		for (int i = 0; i < 8; ++i) {
			point3* vertex = GetBounds(children[i]);

			int res = 0;
			for (int j = 0; j < 8; ++j) 
			{
				point3 p = vertex[j];
				if (s.IsIn(p))
					res++;
			}

			if (res == 8) {
				data.push_back(children[i]);
			}
			else if (res > 0) {
				Octree o = Octree(children[i].origin, children[i].edge_size);
				o.createNodes(s, size_min, data);
			}
		}
	}
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
	float delta = v.edge_size * 0.25f;
	float size = 2.f * delta;

	Voxel* ret = new Voxel[8]
	{ 
		Voxel { point3(v.origin.x + delta, v.origin.y + delta, v.origin.z + delta), size, 255 },
		Voxel { point3(v.origin.x - delta, v.origin.y + delta, v.origin.z + delta), size, 255 },
		Voxel { point3(v.origin.x + delta, v.origin.y - delta, v.origin.z + delta), size, 255 },
		Voxel { point3(v.origin.x - delta, v.origin.y - delta, v.origin.z + delta), size, 255 },
		Voxel { point3(v.origin.x + delta, v.origin.y + delta, v.origin.z - delta), size, 255 },
		Voxel { point3(v.origin.x - delta, v.origin.y + delta, v.origin.z - delta), size, 255 },
		Voxel { point3(v.origin.x + delta, v.origin.y - delta, v.origin.z - delta), size, 255 },
		Voxel { point3(v.origin.x - delta, v.origin.y - delta, v.origin.z - delta), size, 255 }
	};

	return ret;
}
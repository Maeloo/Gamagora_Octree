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
	if (voxel.edge_size >= 2.f)
	{
		Voxel* childrens = Subdivise(voxel);

		for (int i = 0; i < 8; ++i) {
			point3* vertex = GetBounds(childrens[i]);

			int res = 0;
			for (int j = 0; j < 8; ++j) {
				if (isIn(s, vertex[j]))
					res++;
			}

			if (res == 8)
				data.push_back(voxel);
			else if (res > 0) {
				Voxel* fils = Subdivise(voxel);

				for (int k = 0; k < 8; ++k) {
					Octree o = Octree(fils[k].origin, voxel.edge_size / 2);
					o.createNodes(s, data);
				}
			}
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

	Voxel* ret = new Voxel[8]
	{ 
		Voxel { point3(v.origin.x + size, v.origin.y + size, v.origin.z + size), size, 255 },
		Voxel { point3(v.origin.x - size, v.origin.y + size, v.origin.z + size), size, 255 },
		Voxel { point3(v.origin.x + size, v.origin.y - size, v.origin.z + size), size, 255 },
		Voxel { point3(v.origin.x - size, v.origin.y - size, v.origin.z + size), size, 255 },
		Voxel { point3(v.origin.x + size, v.origin.y + size, v.origin.z - size), size, 255 },
		Voxel { point3(v.origin.x - size, v.origin.y + size, v.origin.z - size), size, 255 },
		Voxel { point3(v.origin.x + size, v.origin.y - size, v.origin.z - size), size, 255 },
		Voxel { point3(v.origin.x - size, v.origin.y - size, v.origin.z - size), size, 255 }
	};

	for (int i = 0; i < 8; ++i) {
		ret[i].vertices = getVoxelData(ret[i].origin, ret[i].edge_size);
	}

	return ret;
}

float* Octree::getVoxelData(point3 origin, const float size) {
	float *data = new float[108];
	for (int i = 0; i < 107; i += 3) {
		data[i] = (origin.x + g_vertex_buffer_data[i] * size);
		data[i + 1] = (origin.y + g_vertex_buffer_data[i + 1] * size);
		data[i + 2] = (origin.z + g_vertex_buffer_data[i + 2] * size);
	}
	return data;
}

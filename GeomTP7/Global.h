#pragma once
#include "struct.h"

static const float g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

struct Shape
{
	virtual bool IsIn(const point3 &p) const = 0;
};

struct Sphere : Shape
{
	point3 origin;
	float radius;

	Sphere() : origin(point3(0.f, 0.f, 0.f)), radius(1.f)
	{

	}

	Sphere(const point3 &_origin, const float _radius) : origin(_origin), radius(_radius)
	{

	}

	bool IsIn(const point3 &p) const
	{
		float dist = sqrtf((origin.x - p.x) * (origin.x - p.x) 
			+ (origin.y - p.y) * (origin.y - p.y) 
			+ (origin.z - p.z) * (origin.z - p.z));

		return dist <= radius;
	}
};

struct Voxel : Shape
{
	point3 origin;
	float edge_size;
	int potentiel;
	float* vertices;

	Voxel() : origin(point3(0.f, 0.f, 0.f)), edge_size(1.f), potentiel(255)
	{
		vertices = getVoxelData(origin, edge_size);
	}

	Voxel(const point3 &_origin, const float _edge_size, const int _potentiel) : origin(_origin), edge_size(_edge_size), potentiel(_potentiel)
	{
		vertices = getVoxelData(_origin, _edge_size);
	}

	bool IsIn(const point3 &p) const
	{
		float size(edge_size * 0.5f);

		return (p.x > origin.x - size && p.x < origin.x + size &&
			p.y > origin.y - size && p.y < origin.y + size &&
			p.z < origin.z + size && p.z > origin.z - size);
	}

	bool operator==(Voxel &v) {
		return origin == v.origin;
	}

	float* getVoxelData(const point3 &origin, const float size) {
		float *data = new float[108];
		for (int i = 0; i < 107; i += 3) {
			data[i] = (origin.x + g_vertex_buffer_data[i] * size);
			data[i + 1] = (origin.y + g_vertex_buffer_data[i + 1] * size);
			data[i + 2] = (origin.z + g_vertex_buffer_data[i + 2] * size);
		}
		return data;
	}
};
#pragma once
#include "struct.h"

struct Sphere
{
	point3 origin;
	float radius;
};

struct Voxel
{
	point3 origin;
	float edge_size;
	float* vertices;
	int potentiel;
};
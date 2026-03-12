#pragma once
#include "../Utilities/Math.h"

struct Vertex 
{
	Vector3 pos;
	Vector3 color;
	Vector3 norm, binorm, tgt;
	Vector2 uv;
	Vector2 uv2;

	public:
	Vertex(const Vertex &v) : pos(v.pos), color(v.color), norm(v.norm), binorm(v.binorm), tgt(v.tgt), uv(v.uv), uv2(v.uv2) {}
	Vertex() {} 
};

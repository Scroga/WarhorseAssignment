#pragma once

using DWORD = unsigned long;

struct CustomVertex
{
	float x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
	float u, v;
};
#pragma once
 
#ifndef __SunH__
#define __SunH__

#include "d3dUtility_terrain.h"
#include <string>
#include <vector>
struct Sun_Vertex
{
	float	x, y, z;
	float	u, v;
	static const DWORD FVF;
	Sun_Vertex(float _x,float _y, float _z, float _u,float _v) {
		x = _x; y = _y; z = _z; u = _u; v = _v;
	}
};

//#define D3DFVF_Sun D3DFVF_XYZ|D3DFVF_TEX1
class Sun {

public:
	Sun(IDirect3DDevice9* _device);

	bool  loadTexture(const char * fileName);
	bool draw(D3DXMATRIX* world, bool drawTris);
private:
	IDirect3DDevice9* _device;
	IDirect3DTexture9 * _tex;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9* _ib;
	 


};

#endif
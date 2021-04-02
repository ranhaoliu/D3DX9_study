#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: terrain.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a 3D terrain.       
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __terrainH__
#define __terrainH__

#include "d3dUtility_terrain.h"
#include <string>
#include <vector>

class Terrain
{
public:
	Terrain(
		IDirect3DDevice9* device,
		std::string heightmapFileName,
		int numVertsPerRow,
		int numVertsPerCol,
		int cellSpacing,    // space between cells
		float heightScale);

	~Terrain();

	int  getHeightmapEntry(int row, int col);
	void setHeightmapEntry(int row, int col, int value);

	float getHeight(float x, float z);

	bool  loadTexture(std::string fileName);
	bool  genTexture(D3DXVECTOR3* directionToLight);
	bool  draw(D3DXMATRIX* world, bool drawTris);
public:
	int getWidth();
	int getDepth();
private:
	IDirect3DDevice9* _device;
	IDirect3DTexture9* _tex;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9* _ib;

	//一行的顶点数量
	int _numVertsPerRow;
	//一列的顶点数量
	int _numVertsPerCol;
	//每隔小格子的大小或者说是间距
	int _cellSpacing;
	//每一行的格子数量
	int _numCellsPerRow;
	//每一列的格子数量
	int _numCellsPerCol;
	int _width;
	int _depth;
	//总的顶点数
	int _numVertices;
	//总的三角形数
	int _numTriangles;
	//实施比例变换的高度比例因子
	float _heightScale;
	//高度图
	std::vector<int> _heightmap;

	// helper methods
	bool  readRawFile(std::string fileName);
	bool  myreadRawFile(std::string fileName, int imgWidth, int imgHeight);
	bool  computeVertices();
	bool  computeIndices();
	bool  lightTerrain(D3DXVECTOR3* directionToLight);
	float computeShade(int cellRow, int cellCol, D3DXVECTOR3* directionToLight);

	struct TerrainVertex
	{
		TerrainVertex() {}
		TerrainVertex(float x, float y, float z, float u, float v)
		{
			_x = x; _y = y; _z = z; _u = u; _v = v;
		}
		float _x, _y, _z;
		float _u, _v;

		static const DWORD FVF;
	};
};

#endif // __terrainH__

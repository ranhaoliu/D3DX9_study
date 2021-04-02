#include "Sun.h"
const DWORD Sun_Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;
Sun::Sun(IDirect3DDevice9* device) {
	_device = device;
	HRESULT hr = 0;
	//创建 6个顶点
	  _device->CreateVertexBuffer(
		6 * sizeof(Sun_Vertex),
		D3DUSAGE_WRITEONLY,
		Sun_Vertex::FVF,
		D3DPOOL_MANAGED,
		&_vb,
		0);	 

	//_device->CreateIndexBuffer(
	//	6 * sizeof(WORD), // 3 indices per triangle
	//	D3DUSAGE_WRITEONLY,
	//	D3DFMT_INDEX16,
	//	D3DPOOL_MANAGED,
	//	&_ib,
	//	0);

	Sun_Vertex* vertices = 0;
	_vb->Lock(0, 0, (void**)&vertices, 0);

	float Length = 500;
	vertices[0] = Sun_Vertex(0.0f, Length, 0.0f, 0.0f, 0.0f );
	vertices[1] = Sun_Vertex(Length, Length, 0.0f, 1.0f, 0.0f );
	vertices[2] = Sun_Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f );

	vertices[3] = Sun_Vertex(Length, Length, 0.0f, 1.0f, 0.0f );
	vertices[4] = Sun_Vertex(Length, 0.0f, 0.0f, 1.0f, 1.0f );
	vertices[5] = Sun_Vertex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
 
	_vb->Unlock();


	/*WORD* indices = 0;
	_ib->Lock(0, 0, (void**)&indices, 0);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;
	_ib->Unlock();*/
}
bool Sun::loadTexture(const char * fileName) {

	D3DXCreateTextureFromFile(_device, fileName, &_tex);  //上面


	return true;
}
bool Sun::draw(D3DXMATRIX* world, bool drawTris)
{
	 

	 
		_device->SetTransform(D3DTS_WORLD, world);
		_device->SetTexture(0, _tex);
		_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		_device->SetRenderState(D3DRS_LIGHTING, false);

		_device->SetStreamSource(0, _vb, 0, sizeof(Sun_Vertex));
		_device->SetFVF(Sun_Vertex::FVF);
		_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,2);
		 

		 

		 
		return true;
	 

}
//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: terrainDriver.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Renders a terrain and allows you to walk around it. 
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility_terrain.h"
#include "terrain.h"
#include "camera.h"
#include "fps.h"
#include "SkyBoxClass.h"
#include "Mouse.h"
#include "Sun.h"
//
// Globals
//

IDirect3DDevice9* Device = 0;

const int Width = 640;
const int Height = 480;

Terrain* TheTerrain = 0;
Camera   TheCamera(Camera::AIRCRAFT);
Mouse mouse;
FPSCounter* FPS = 0;
//创建天空盒子指针
SkyBoxClass* g_pSkyBox = NULL;  
Sun* sun;
D3DLIGHT9 light;
//天空盒类的指针实例
//
// Framework Functions
//
bool Setup()
{
	//
	// Create the terrain.
	//
	D3DXVECTOR3 lightDirection(0.0f, 1.0f, 0.0f);
	//TheTerrain = new Terrain(Device, "coastMountain64.raw", 64, 64, 10, 0.5f);
	TheTerrain = new Terrain(Device, "MyTerrainMap4.raw", 256, 256, 10, 1.8f);
	TheTerrain->genTexture(&lightDirection);

	//创建并初始化天空对象
	g_pSkyBox = new SkyBoxClass(Device);
	g_pSkyBox->LoadSkyTextureFromFile("posz.jpg", "negz.jpg","negx.jpg", "posx.jpg","posy.jpg" );//从文件加载前、后、左、右、顶面5个面的纹理图
	g_pSkyBox->InitSkyBox(1000000);  //设置天空盒的边长
	 

	// ---创建一个太阳
	sun = new Sun(Device);
	sun->loadTexture("Grass2.jpg");
	
	//-----------
	//
	// Create the font.
	//

	FPS = new FPSCounter(Device);

	//
	// Set texture filters.
	//

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	//
	// Set projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 45 - degree
		(float)Width / (float)Height,
		1.0f,
		1000000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	//D3DXVECTOR3 a = D3DXVECTOR3(0, 10, 0);
	//TheCamera.setPosition(&a);
	// 
	
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;//聚光灯光源
	light.Position = D3DXVECTOR3(1000,1000,0);
	
	light.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Range = 300.0f;
	light.Falloff = 0.1f;
	light.Phi = D3DX_PI / 3.0f;
	light.Theta = D3DX_PI / 6.0f;
	
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	 
	return true;
}

void Cleanup()
{
	d3d::Delete<Terrain*>(TheTerrain);
	d3d::Delete<FPSCounter*>(FPS);
	d3d::Delete<SkyBoxClass*>(g_pSkyBox);
	d3d::Delete< Sun*>(sun);
}
 

bool Display(float timeDelta)
{
	//
	// Update the scene:
	//

	if (Device)
	{
		 
				//Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
				//Device->BeginScene();
				////绘制天空
				//D3DXMATRIX matSky, matTransSky, matRotSky;
				//D3DXMatrixTranslation(&matTransSky, 0.0f, -3500.0f, 0.0f);
				//D3DXMatrixRotationY(&matRotSky, -0.000005f * timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果
				//matSky = matTransSky * matRotSky;
				//g_pSkyBox->RenderSkyBox(&matSky, false);
				//Device->EndScene();
				//Device->Present(0, 0, 0, 0);
		
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			TheCamera.walk(1000.0f * timeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			TheCamera.walk(-1000.0f * timeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			TheCamera.yaw(-1.0f * timeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			TheCamera.yaw(1.0f * timeDelta);

		if (::GetAsyncKeyState('N') & 0x8000f)
			TheCamera.strafe(-100.0f * timeDelta);

		if (::GetAsyncKeyState('M') & 0x8000f)
			TheCamera.strafe(100.0f * timeDelta);

		if (::GetAsyncKeyState('W') & 0x8000f)
			TheCamera.pitch(1.0f * timeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			//TheCamera.pitch(-1.0f * timeDelta);
		 TheCamera.fly(1000.0f * timeDelta);
		if (::GetAsyncKeyState('Q') & 0x8000f)
			//TheCamera.pitch(-1.0f * timeDelta);
		 TheCamera.fly(-1000.0f * timeDelta);
	//if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
			//TheCamera.pitch(-1.0f * timeDelta);
		//每加载依次都让他设置成false


		bool LmouseMoveFlag = false;
		bool RmouseMoveFlag = false;
		mouse.startpos = mouse.pos;

		if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f) {
			LmouseMoveFlag = true;
		}
		if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000f) {
			RmouseMoveFlag = true;
		}
		//得到光标的位置
		GetCursorPos(&mouse.pos);
		if (LmouseMoveFlag) {
			TheCamera.strafe((mouse.pos.x - mouse.startpos.x) * (1.0f));//移动的距离是现在的位置减去上一次的位置
			TheCamera.walk((mouse.pos.y - mouse.startpos.y) * (-1.0f));
			light.Direction = D3DXVECTOR3(mouse.pos.x, mouse.pos.y, 0.0f);
		}
		if (RmouseMoveFlag) {
			TheCamera.yaw((mouse.pos.x - mouse.startpos.x) * (0.1f / D3DX_PI));
			TheCamera.pitch((mouse.pos.y - mouse.startpos.y) * (0.1f / D3DX_PI));
		}

		D3DXVECTOR3 pos;
		TheCamera.getPosition(&pos);
		pos.x = (pos.x < -TheTerrain->getWidth() / 2) ? -TheTerrain->getWidth() / 2 +0.001: pos.x;
		pos.x = (pos.x > TheTerrain->getWidth() / 2) ? TheTerrain->getWidth() / 2 - 0.001 : pos.x;
		pos.z = (pos.z < -TheTerrain->getDepth() / 2) ? -TheTerrain->getDepth() / 2 + 0.001: pos.z;
		pos.z = (pos.z > TheTerrain->getDepth() / 2) ? TheTerrain->getDepth() / 2 - 0.001 : pos.z;
		//线性插值得到高度值
		float height = TheTerrain->getHeight(pos.x, pos.z);
		//pos.y = height + 5.0f; // add height because we're standing up
		TheCamera.setPosition(&pos);

		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);

		Device->SetTransform(D3DTS_VIEW, &V);

		// Draw the scene:
		//
		//Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0X87CEEB, 1.0f, 0);

		Device->BeginScene();

		Device->SetLight(0, &light); //设置光源
		Device->LightEnable(0, true); //启用光照
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);


		
		/*D3DXMATRIX sc;
		D3DXMatrixScaling(&sc, 10, 0, 10);
		I = I * sc;*/
		if (TheTerrain)
			TheTerrain->draw(&I, true);
		//绘制天空
		D3DXMATRIX matSky, matTransSky, matRotSky;
		D3DXMatrixTranslation(&matTransSky, 0.0f, -280000.0f, 0.0f);
		//D3DXMatrixRotationY(&matRotSky, -0.000005f * timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果
		//matSky = matTransSky * matRotSky;

		//g_pSkyBox->RenderSkyBox(&matSky, false);
		g_pSkyBox->RenderSkyBox(&matTransSky, false);
		D3DXMatrixRotationX(&matRotSky, -0.0005f * timeGetTime());
		//-----
		D3DXMATRIXA16 matWorld;
		D3DXMATRIX matTran;
		D3DXMATRIX matRot0;
		D3DXMATRIX matRot1;
		D3DXMATRIX matScal;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matTran);
		D3DXMatrixIdentity(&matRot0);
		D3DXMatrixIdentity(&matRot1);
		D3DXMatrixIdentity(&matScal);
		//缩放
		D3DXMatrixScaling(&matScal, 0.5f, 0.5f, 0.5f);
		//自转
		//D3DXMatrixRotationY(&matRot0, 2 * D3DX_PI * (timeGetTime() % 2000) / 2000);
		D3DXMatrixRotationY(&matRot0,  D3DX_PI /2);
		//平移到轨道位置
		D3DXMatrixTranslation(&matTran, -850, 1000, 0);
		//在轨道上绕太阳公转
		D3DXMatrixRotationX(&matRot1, 2 * D3DX_PI * (timeGetTime() % 10000) / 10000);
		matWorld = matScal * matRot0 * matTran * matRot1;
		 
		 
		//-----
		sun->draw(&matWorld,true);

		if (FPS)
			FPS->render(0xffffffff, timeDelta);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	 
	D3DXMATRIX mv;
	D3DXMATRIX gt;
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	////按鼠标左键,摄像机向左转动
	/*case WM_LBUTTONDOWN:

		GetCursorPos(&mouse.pos);
		TheCamera.strafe(10.0f);
		TheCamera.getViewMatrix(&mv);
		Device->SetTransform(D3DTS_VIEW, &mv);
		break;*/
	//按右键，摄像机向右转动
	//case WM_RBUTTONDOWN:
	//	TheCamera.yaw(0.45f);
	//	
	//	TheCamera.getViewMatrix(&gt);
	//	Device->SetTransform(D3DTS_VIEW, &gt);
	//	break;
	
	}

	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}
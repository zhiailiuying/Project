#pragma once
#include"global.h"
#include"Light.h"
#include"Texture.h"
#include"Buffer.h"
#include"Vertex.h"

struct Phong_Vertex
{
	std::vector<DirectionLight> v_directionlight;          //存放灯光的容器
	std::vector<PointLight> v_pointlight;
	Vector3f cameraPosition;                               //相机位置
	Vector3f lightPos0, lightPos1, lightPos2;       //Light Projectuon Space position(光源投射空间位置)：意思：在光源视角(光源即摄像机)下顶点的位置
	Matrix m, v, p;                                //m,v,p变换
	Vertex vert_v0, vert_v1, vert_v2;                           //三角形的三个顶点
	Matrix Light_View, Light_Ortho;                //光源视角(光源即摄像机),光源正交视图，即光源看到的正交视图
};

class Phong_fragment
{
public:

	Phong_fragment() {};
	Phong_fragment(Texture* baseTex, std::unique_ptr<DepthBuffer> &depthbuffer)
	{
		this->Tex = baseTex;                                    //获取材质
		this->depthBuffer = depthbuffer.get();                         //获取深度值
	}

	~Phong_fragment() {};

	Texture* Tex;
	DepthBuffer* depthBuffer;
};

struct Shadow_Vertex
{
	Matrix lightV_View, light_Ortho;    //光源视角和光源空间矩阵
	Matrix m;                       //透视投影矩阵
};
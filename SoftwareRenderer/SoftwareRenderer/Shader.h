#pragma once
#include"global.h"
#include"Uniform.h"
#include"Vertex.h"
#include"Light.h"

class Shader
{
public:

	virtual void VertexShader(Vertex &v0,Vertex &v1,Vertex&v2) = 0;
	virtual bool FragmentShader(Vertex &v,Vector3f G) = 0;

};

//phong��ɫ
class PhongShader :public Shader
{
public:

	PhongShader();
	PhongShader(Phong_Vertex phongVertex,Phong_fragment phongfragment);
	virtual void VertexShader(Vertex& v_0, Vertex& v_1, Vertex& v_2);
	virtual bool FragmentShader(Vertex& v, Vector3f G);
	float CalcutelationShadows(Vector3f LightSpace ,float bais);          //������Ӱ�������տռ��ƫ����
	~PhongShader();

	Phong_Vertex phong_vertex;
	Phong_fragment phong_fragment;
	int count;
};

//��Ӱ��ɫ
class Shadow :public Shader
{
public:
	Shadow();
	virtual void VertexShader(Vertex &v0,Vertex &v1,Vertex &v2);
	virtual bool FragmentShader(Vertex &v,Vector3f G);
	~Shadow();

	Shadow_Vertex shadow_vertex;
};
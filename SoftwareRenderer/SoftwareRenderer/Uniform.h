#pragma once
#include"global.h"
#include"Light.h"
#include"Texture.h"
#include"Buffer.h"
#include"Vertex.h"

struct Phong_Vertex
{
	std::vector<DirectionLight> v_directionlight;          //��ŵƹ������
	std::vector<PointLight> v_pointlight;
	Vector3f cameraPosition;                               //���λ��
	Vector3f lightPos0, lightPos1, lightPos2;       //Light Projectuon Space position(��ԴͶ��ռ�λ��)����˼���ڹ�Դ�ӽ�(��Դ�������)�¶����λ��
	Matrix m, v, p;                                //m,v,p�任
	Vertex vert_v0, vert_v1, vert_v2;                           //�����ε���������
	Matrix Light_View, Light_Ortho;                //��Դ�ӽ�(��Դ�������),��Դ������ͼ������Դ������������ͼ
};

class Phong_fragment
{
public:

	Phong_fragment() {};
	Phong_fragment(Texture* baseTex, std::unique_ptr<DepthBuffer> &depthbuffer)
	{
		this->Tex = baseTex;                                    //��ȡ����
		this->depthBuffer = depthbuffer.get();                         //��ȡ���ֵ
	}

	~Phong_fragment() {};

	Texture* Tex;
	DepthBuffer* depthBuffer;
};

struct Shadow_Vertex
{
	Matrix lightV_View, light_Ortho;    //��Դ�ӽǺ͹�Դ�ռ����
	Matrix m;                       //͸��ͶӰ����
};
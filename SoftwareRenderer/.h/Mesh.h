#pragma once
#include"global.h"
#include"Matrix.h"
#include"Transform.h"
#include"Vertex.h"

//������
class Mesh
{
public:

	Mesh();
	Transform transform;                   //�任����
	std::vector<Vertex>   VertexBuffer;    //���㻺��
	std::vector<Vector3f> PositionBuffer;   //λ�û���
	std::vector<Vector3f> NormalBuffer;    //����������
	std::vector<Vector2>  UVBuffer;         //uv����
	std::vector<Vector3i> IndexBuffer;         //����ֵ����

	void SetTransform(const Transform t);           //���ñ任����
	Transform GetTansform();                  //��ȡ�任����
	int GetIndexBufferLength();                 //��ȡ�����������Ĵ�С
	void SetObjectToWorld(const Matrix objectToWorld);    //���ö���任���������  
	Matrix GetObjectToWorld();                 //��ȡ����任���������
	void AddVertexData(const Vector3f pos,float u,float v,Color color);            //��vector����pos

	//����main�����ĵط�����ճ���õģ�ģ���Ǹ���������������������ճ��������
	void AddVertexData(float pos_x, float pos_y, float pos_z, float u, float v, const Vector3f normal, Color color);  
	void AddvertexData(const Vector3f pos, float u, float v,const Vector3f normal, Color color);          //�������������VertexBuffer

	void PrintTest();

	~Mesh();
	
};
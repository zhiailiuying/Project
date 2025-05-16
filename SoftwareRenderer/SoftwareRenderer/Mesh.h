#pragma once
#include"global.h"
#include"Matrix.h"
#include"Transform.h"
#include"Vertex.h"

//网格类
class Mesh
{
public:

	Mesh();
	Transform transform;                   //变换矩阵
	std::vector<Vertex>   VertexBuffer;    //顶点缓冲
	std::vector<Vector3f> PositionBuffer;   //位置缓冲
	std::vector<Vector3f> NormalBuffer;    //法向量缓冲
	std::vector<Vector2>  UVBuffer;         //uv缓冲
	std::vector<Vector3i> IndexBuffer;         //索引值缓冲

	void SetTransform(const Transform t);           //设置变换矩阵
	Transform GetTansform();                  //获取变换矩阵
	int GetIndexBufferLength();                 //获取索引缓冲区的大小
	void SetObjectToWorld(const Matrix objectToWorld);    //设置对象变换到世界矩阵  
	Matrix GetObjectToWorld();                 //获取对象变换到世界矩阵
	void AddVertexData(const Vector3f pos,float u,float v,Color color);            //用vector保存pos

	//方便main函数的地方复制粘贴用的，模型是浮点数，不是向量，复制粘贴不方便
	void AddVertexData(float pos_x, float pos_y, float pos_z, float u, float v, const Vector3f normal, Color color);  
	void AddvertexData(const Vector3f pos, float u, float v,const Vector3f normal, Color color);          //将顶点数组放入VertexBuffer

	void PrintTest();

	~Mesh();
	
};
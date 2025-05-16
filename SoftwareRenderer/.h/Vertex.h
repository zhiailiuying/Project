#pragma once
#include"global.h"
#include"Vector3.h"
#include"Color.h"
#include"Matrix.h"

//顶点类
class Vertex
{
public:

	Vector3f Position;                //顶点位置
	Vector3f Normal;                  //顶点法线
	Color  v_Color;                   //顶点颜色
	Vector2  uv;                      //顶点uv
	Vector3i  VertexIndex;            //顶点索引    

	Vertex();                      //默认构造
	//这里两个构造函数要解决两种不同情况下顶点的初始化
	Vertex(const Vector3f &p ,const Color &c,Vector2 uv);    //这里的默认构造用来初始化 p , c ,uv;
	Vertex(const Vector3f& p, const Color& c, const Vector3f &normal, Vector2 uv); //初始化 p ,c,n,uv;

	void VertexLerpData(Vertex& left, Vertex& right, float t);          //对左右顶点进行插值
	static float Lerp(float v1, float v2, float t);                 //定义插值的表达式 //static的作用：Lerp只能本文件内使用
	Vertex &operator*(const Matrix& m);                              //不会被其他文件连接，防止多个文件有重名函数  
	void VertexPrint();
	~Vertex();

	//小tips：以上static可以用extern替换，这样不用#include也可以直接用其他文件的func，避免了引入头文件大部分不需要的func
};
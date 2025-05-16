#pragma once
#include"global.h"
#include"Vector3.h"
#include"Matrix.h"

class Transform
{
public:

	Transform();               //默认变换构造函数
	Transform(Vector3f pos, Vector3f rotate, Vector3f scale);       //初始化变换矩阵
	Vector3f position;         //顶点位置向量
	Vector3f Rotation;         //旋转
	Vector3f Scale;             //缩放
	Matrix ObjectToWorld;        //将对象坐标转化到世界坐标
	
	Matrix Translate(const Vector3f &v);         //平移矩阵
	Matrix RotateX(float angle);                 //绕x轴旋转
	Matrix RotateY(float angle);                 //绕y轴旋转
	Matrix RotateZ(float angle);                 //绕z轴旋转
	Matrix Rotate(const Vector3f& rot);            //绕任意轴旋转
	Matrix ScaleMatrix(const Vector3f &s);                    //缩放矩阵

	//引用传递带const时，1、可以防止引用对象在函数内部被修改
	//          2、有void test(string &str)定义，不能直接这样子传参test("hello"),加了const就可以了，因为"hello"相当于常量值
	//        3、这里用const修饰，即可以防止引用的对象被修改，还可以直接传向量值(如(0,0,1)这种)
};
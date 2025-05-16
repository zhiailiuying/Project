#pragma once
#include"global.h"
#include"Vector3.h"
#include"Matrix.h"

//灯光
class Light
{
public:

	float L_Intensity;             //光强
	Vector3f L_Position;           //位置

};

//日光
class DirectionLight : public Light
{
public:

	DirectionLight();                   //默认构造

	DirectionLight(const Vector3f& dir, const Vector3f& pos, const float& I, const bool &forVertex);    //有参构造

	Vector3f GetInverstDirection();                  //取反

	Matrix LookAt(const Vector3f &upAxis);                 //看向的地方

	~DirectionLight();

private:

	Vector3f L_Direction;                            //日光方向              
	bool L_forVertex;                                 //逐像素、顶点着色
};

//点光
class PointLight:public Light
{
public:

	PointLight();

	PointLight(const Vector3f& pos, const float& I);           //点光源的有参构造

	~PointLight();

};


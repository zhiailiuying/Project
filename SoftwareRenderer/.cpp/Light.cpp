#include"Light.h"

//日光
DirectionLight::DirectionLight()
{
	this->L_Intensity = 1.0f;
	this->L_forVertex = true;
}

DirectionLight::DirectionLight(const Vector3f& dir, const Vector3f& pos, const float& I, const bool& forVertex)
{
	this->L_Direction = dir;
	this->L_Position = pos;
	this->L_Intensity = I;
	this->L_forVertex = forVertex;
}

Vector3f DirectionLight::GetInverstDirection()                   //向量取反
{
	return Vector3f(0,0,0) - this->L_Direction; 
}

Matrix DirectionLight::LookAt(const Vector3f &upAxis)             //获取看向的方向
{
	Vector3f LookDirection = this->L_Direction;
	LookDirection.Normalize();

	Vector3f rightDirection = rightDirection.Cross(upAxis, LookDirection);       //里向量叉乘上(暂时固定）得右向量
	rightDirection.Normalize();

	Vector3f upDirection = upDirection.Cross(LookDirection, rightDirection);       // 得到固定的上向量
	upDirection.Normalize();                          //这样子做的原因是因为要保持三个坐标轴始终垂直，因为upAxis可能会歪

	//存放灯光坐标轴的矩阵
	Matrix m;
	m.value[0][0] = rightDirection.x;  m.value[0][1] = rightDirection.y; m.value[0][2] = rightDirection.z;  m.value[0][3] = -this->L_Position.x;
	m.value[1][0] = upDirection.x;     m.value[1][1] = upDirection.y;    m.value[1][2] = upDirection.z;     m.value[1][3] = -this->L_Position.y;
	m.value[2][0] = LookDirection.x;   m.value[2][1] = LookDirection.y;  m.value[2][2] = LookDirection.z;   m.value[2][3] = -this->L_Position.z;
	m.value[3][0] = 0;                 m.value[3][1] = 0;                m.value[3][2] = 0;                 m.value[3][3] = 1;

	//m.Print();
	//system("pause");
	return m;
} 

DirectionLight::~DirectionLight()
{

}

//点光
PointLight::PointLight()
{
	this->L_Intensity = 1.0f;
}

PointLight::PointLight(const Vector3f &pos,const float &I)
{
	this->L_Position = pos;
	this->L_Intensity = I;
}

PointLight::~PointLight()
{

}
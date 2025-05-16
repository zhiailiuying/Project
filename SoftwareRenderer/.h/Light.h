#pragma once
#include"global.h"
#include"Vector3.h"
#include"Matrix.h"

//�ƹ�
class Light
{
public:

	float L_Intensity;             //��ǿ
	Vector3f L_Position;           //λ��

};

//�չ�
class DirectionLight : public Light
{
public:

	DirectionLight();                   //Ĭ�Ϲ���

	DirectionLight(const Vector3f& dir, const Vector3f& pos, const float& I, const bool &forVertex);    //�вι���

	Vector3f GetInverstDirection();                  //ȡ��

	Matrix LookAt(const Vector3f &upAxis);                 //����ĵط�

	~DirectionLight();

private:

	Vector3f L_Direction;                            //�չⷽ��              
	bool L_forVertex;                                 //�����ء�������ɫ
};

//���
class PointLight:public Light
{
public:

	PointLight();

	PointLight(const Vector3f& pos, const float& I);           //���Դ���вι���

	~PointLight();

};


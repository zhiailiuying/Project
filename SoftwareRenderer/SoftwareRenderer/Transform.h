#pragma once
#include"global.h"
#include"Vector3.h"
#include"Matrix.h"

class Transform
{
public:

	Transform();               //Ĭ�ϱ任���캯��
	Transform(Vector3f pos, Vector3f rotate, Vector3f scale);       //��ʼ���任����
	Vector3f position;         //����λ������
	Vector3f Rotation;         //��ת
	Vector3f Scale;             //����
	Matrix ObjectToWorld;        //����������ת������������
	
	Matrix Translate(const Vector3f &v);         //ƽ�ƾ���
	Matrix RotateX(float angle);                 //��x����ת
	Matrix RotateY(float angle);                 //��y����ת
	Matrix RotateZ(float angle);                 //��z����ת
	Matrix Rotate(const Vector3f& rot);            //����������ת
	Matrix ScaleMatrix(const Vector3f &s);                    //���ž���

	//���ô��ݴ�constʱ��1�����Է�ֹ���ö����ں����ڲ����޸�
	//          2����void test(string &str)���壬����ֱ�������Ӵ���test("hello"),����const�Ϳ����ˣ���Ϊ"hello"�൱�ڳ���ֵ
	//        3��������const���Σ������Է�ֹ���õĶ����޸ģ�������ֱ�Ӵ�����ֵ(��(0,0,1)����)
};
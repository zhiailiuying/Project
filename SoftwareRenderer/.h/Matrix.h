/*************************************
 * \file Matrix.h
 * \date 2021/08/25 15:14
 *
 * \author XueLiuXing
 *
 * TODO: long description
 *
*************************************/
#pragma once
#include"Vector3.h"
//#include "Vertex.h"
class Matrix
{
public:

	float value[4][4];

	Matrix();
	~Matrix();

	Matrix operator + (const Matrix& right) const;
	Matrix operator - (const Matrix& right) const;
	Matrix operator * (const Matrix& right) const;
	Matrix operator * (float k) const;

	Vector3f MultiplyVector3(const Vector3f& v) const;                  //矩阵的乘法
	
	void Identity();                                                    //矩阵单位化

	Matrix transpose() const                                           //矩阵的转置                              
	{
		Matrix trans;
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				trans.value[i][j] = value[j][i];
		return trans;
	}
	void Print();

};


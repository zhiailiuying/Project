#pragma once
#include"global.h"

class Buffer
{
public:

	int m_Width;                          //���������

	int m_Height;                         //�������߶�

};


class DepthBuffer :public Buffer
{
public:

	DepthBuffer();

	DepthBuffer(int width,int height);

	float Clamp(float max,float min,float value);                      //ǯ��,�ø������������ȷ

	float Sample(float u,float v);                                         //ӳ��

	void PrintDepthBuffer();

	~DepthBuffer();

	float** depthBuffer;               //�����൱��depthBuffer[width][height],��ά����Ҳ��**���͵�
};
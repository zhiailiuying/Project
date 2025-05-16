#pragma once
#include"global.h"

class Buffer
{
public:

	int m_Width;                          //缓冲区宽度

	int m_Height;                         //缓冲区高度

};


class DepthBuffer :public Buffer
{
public:

	DepthBuffer();

	DepthBuffer(int width,int height);

	float Clamp(float max,float min,float value);                      //钳制,用浮点数计算更精确

	float Sample(float u,float v);                                         //映射

	void PrintDepthBuffer();

	~DepthBuffer();

	float** depthBuffer;               //这里相当于depthBuffer[width][height],二维数组也是**类型的
};
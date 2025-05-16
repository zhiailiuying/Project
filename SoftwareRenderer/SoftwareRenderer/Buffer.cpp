#include"Buffer.h"

DepthBuffer::DepthBuffer()
{
	this->depthBuffer = nullptr;
}

DepthBuffer::DepthBuffer(int width,int height)               //��ʼ��������
{
	this->m_Width = width;
	this->m_Height = height;
	//std::cout << this->m_Width << std::endl;
	//std::cout << this->m_Height << std::endl;

	this->depthBuffer = new float* [this->m_Height];         //��newһ�У�Ȼ��һ���е�ÿ��Ԫ��new��Ӧ����

	for (int i = 0; i < this->m_Height; i++)
	{
		this->depthBuffer[i] = new float[this->m_Width];     //��new��Ӧ��ÿһ��
	}

	for (int i = 0; i < this->m_Height; i++)
	{
		for (int j = 0; j < this->m_Width; j++)
		{
			this->depthBuffer[i][j] = 1;                       //��ʼ��������
		}
	}
}

float DepthBuffer::Clamp(float min, float max, float value)    //ǯ��
{
	if (value > max)
	{
		return max;
	}
	else if (value < min)
	{
		return min;
	}
	return value;                                             //ǯ�ƺ󷵻ظ�ֵ
}

float DepthBuffer::Sample(float u, float v)                      //ǯ��֮����ӳ��
{
	int y = this->Clamp(0, this->m_Height - 1, u);            //ͼƬ����ԭ�������Ͻ�,u�����£�v������
	int x = this->Clamp(0, this->m_Width - 1, v);             //�����оͶ�Ӧ������y(v),�����оͱ�����x(u)
	return this->depthBuffer[y][x];                     //�жϷ���������һ��x������y������һ��y������x,��֪����Ӧ�����й�ϵ��                                          
}

DepthBuffer::~DepthBuffer()
{
	for (int i = 0; i < this->m_Height; i++)
	{
		delete[] depthBuffer[i];                    //int[][]������ָ����int**����ָ��ÿһ��Ԫ�ص�int*���������ͷ�int*
	}
	delete[] depthBuffer;                           //�����ͷ�int**
	depthBuffer = nullptr;
}

void DepthBuffer::PrintDepthBuffer()
{
	for (int i = 0; i < this->m_Height; i++)
	{
		for (int j = 0; j < this->m_Width; j++)
		{
			std::cout << this->depthBuffer[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
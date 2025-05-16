#include"Buffer.h"

DepthBuffer::DepthBuffer()
{
	this->depthBuffer = nullptr;
}

DepthBuffer::DepthBuffer(int width,int height)               //初始化缓冲区
{
	this->m_Width = width;
	this->m_Height = height;
	//std::cout << this->m_Width << std::endl;
	//std::cout << this->m_Height << std::endl;

	this->depthBuffer = new float* [this->m_Height];         //先new一列，然后一列中的每个元素new对应的行

	for (int i = 0; i < this->m_Height; i++)
	{
		this->depthBuffer[i] = new float[this->m_Width];     //再new对应的每一行
	}

	for (int i = 0; i < this->m_Height; i++)
	{
		for (int j = 0; j < this->m_Width; j++)
		{
			this->depthBuffer[i][j] = 1;                       //初始化缓冲区
		}
	}
}

float DepthBuffer::Clamp(float min, float max, float value)    //钳制
{
	if (value > max)
	{
		return max;
	}
	else if (value < min)
	{
		return min;
	}
	return value;                                             //钳制后返回该值
}

float DepthBuffer::Sample(float u, float v)                      //钳制之后做映射
{
	int y = this->Clamp(0, this->m_Height - 1, u);            //图片坐标原点在左上角,u轴向下，v轴向右
	int x = this->Clamp(0, this->m_Width - 1, v);             //遍历行就对应遍历的y(v),遍历列就遍历的x(u)
	return this->depthBuffer[y][x];                     //判断方法：钉死一个x，遍历y，钉死一个y，遍历x,就知道对应的行列关系了                                          
}

DepthBuffer::~DepthBuffer()
{
	for (int i = 0; i < this->m_Height; i++)
	{
		delete[] depthBuffer[i];                    //int[][]里面是指的是int**里面指向每一个元素的int*，这里先释放int*
	}
	delete[] depthBuffer;                           //这里释放int**
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
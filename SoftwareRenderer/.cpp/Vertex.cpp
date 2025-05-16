#include"Vertex.h"

Vertex::Vertex()                        //默认构造
{

}

Vertex::Vertex(const Vector3f& p, const Color& c, const Vector2 uv)   //对顶点和颜色、uv初始化
{
	this->Position = p;
	this->v_Color = c;
	this->uv = uv;
}
Vertex::Vertex(const Vector3f &p, const Color& c, const Vector3f &normal, Vector2 uv)  //对顶点和颜色、法线、uv初始化
{
	this->Position = p;
	this->v_Color = c;
	this->Normal = normal;
	this->uv = uv;
}

void Vertex::VertexLerpData(Vertex& left, Vertex& right, float t)         //顶点插值函数
{
	this->Position.z = this->Lerp(left.Position.z, right.Position.z, t);   //这里只需要z插值就可以了,x和y在之后画三角形时会插值
	this->v_Color = Color::Lerp(left.v_Color, right.v_Color, t);           //对颜色插值
	this->uv.x = this->Lerp(left.uv.x, right.uv.x, t);                     //对uv的u坐标插值
	this->uv.y = this->Lerp(left.uv.y, right.uv.y, t);                     //对uv的v坐标插值
	this->Normal = left.Normal + (right.Normal - left.Normal) * t;         //对法线进行插值
	//这里不用Lerp是因为Normal是Vector类型，不能强转为float
}

float Vertex::Lerp(float v1, float v2, float t)                         //顶点插值函数
{
	return v1 + (v2 - v1) * t;
}

Vertex &Vertex::operator*(const Matrix& m)
{
	Position.x = Position.x * m.value[0][0] + Position.y * m.value[0][1] + Position.z * m.value[0][2] + Position.w * m.value[0][3];
	Position.y = Position.x * m.value[1][0] + Position.y * m.value[1][1] + Position.z * m.value[1][2] + Position.w * m.value[1][3];
	Position.z = Position.x * m.value[2][0] + Position.y * m.value[2][1] + Position.z * m.value[2][2] + Position.w * m.value[2][3];
	Position.w = Position.x * m.value[3][0] + Position.y * m.value[3][1] + Position.z * m.value[3][2] + Position.w * m.value[3][3];

	//ע�⣬ֻ����ת��ƽ�Ƶ���ֱ��ʹ��MVP���󣬱�����Ҫ�����������ת��
	Normal.x = Normal.x * m.value[0][0] + Normal.y * m.value[0][1] + Normal.z * m.value[0][2] + Normal.w * m.value[0][3];
	Normal.y = Normal.x * m.value[1][0] + Normal.y * m.value[1][1] + Normal.z * m.value[1][2] + Normal.w * m.value[1][3];
	Normal.z = Normal.x * m.value[2][0] + Normal.y * m.value[2][1] + Normal.z * m.value[2][2] + Normal.w * m.value[2][3];
	Normal.w = Normal.x * m.value[3][0] + Normal.y * m.value[3][1] + Normal.z * m.value[3][2] + Normal.w * m.value[3][3];

	return *this;
}

void Vertex::VertexPrint()
{
	std::cout << "z位置：" << this->Position.z << std::endl;
	std::cout << "uv的x: " << this->uv.x << std::endl;
	std::cout << "uv的y：" << this->uv.y << std::endl;
}

Vertex::~Vertex()
{

}
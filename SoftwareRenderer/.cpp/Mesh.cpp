#include"Mesh.h"

Mesh::Mesh()                      //默认构造函数
{

}

void Mesh::SetObjectToWorld(const Matrix objectToWorld)      //设置对象变换到世界矩阵
{
	this->transform.ObjectToWorld = objectToWorld;
}

void Mesh::SetTransform(const Transform t)      //设置变换矩阵
{
	this->transform = t;
}

int Mesh::GetIndexBufferLength()          //获取顶点缓冲的大小
{
	return this->IndexBuffer.size();
}
  
Matrix Mesh::GetObjectToWorld()          //获取对象到世界的变换矩阵
{
	return this->transform.ObjectToWorld;
}

Transform Mesh::GetTansform()              //获取变换矩阵
{
	return this->transform;
}

void Mesh::AddVertexData(const Vector3f pos, float u, float v, Color color)       //将顶点保存到缓冲区中
{
	Vertex p(pos, color, Vector2(u, v));
	this->VertexBuffer.push_back(p);
}

//方便复制粘贴用
void Mesh::AddVertexData(float pos_x, float pos_y, float pos_z, float u, float v, const Vector3f normal, Color color)
{
	this->AddvertexData(Vector3f(pos_x, pos_y, pos_z), u, v, normal, color);
}

void Mesh::AddvertexData(const Vector3f pos, float u, float v, const Vector3f normal, Color color) //将顶点保存到缓冲区中
{
	Vertex p(pos, color, normal, Vector2(u, v));
	this->VertexBuffer.push_back(p);
}

void Mesh::PrintTest()
{
	//for (const auto& v : this->VertexBuffer)
	//{
	//	std::cout << "位置：(" << v.Position.x << "," << v.Position.y << "," << v.Position.z << ") ";
	//	std::cout << "颜色：(" << v.v_Color.r << "," << v.v_Color.g << "," << v.v_Color.b << "," << v.v_Color.a << ") ";
	//	std::cout << "法向量: (" << v.Normal.x << "," << v.Normal.y << "," << v.Normal.z << ")";
	//	std::cout << "uv坐标: (" << v.uv.x << "," << v.uv.y << ")";
	//	std::cout << std::endl;
	//}
}

Mesh::~Mesh()
{

}

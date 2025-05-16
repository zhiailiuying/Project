#include"Mesh.h"

Mesh::Mesh()                      //Ĭ�Ϲ��캯��
{

}

void Mesh::SetObjectToWorld(const Matrix objectToWorld)      //���ö���任���������
{
	this->transform.ObjectToWorld = objectToWorld;
}

void Mesh::SetTransform(const Transform t)      //���ñ任����
{
	this->transform = t;
}

int Mesh::GetIndexBufferLength()          //��ȡ���㻺��Ĵ�С
{
	return this->IndexBuffer.size();
}
  
Matrix Mesh::GetObjectToWorld()          //��ȡ��������ı任����
{
	return this->transform.ObjectToWorld;
}

Transform Mesh::GetTansform()              //��ȡ�任����
{
	return this->transform;
}

void Mesh::AddVertexData(const Vector3f pos, float u, float v, Color color)       //�����㱣�浽��������
{
	Vertex p(pos, color, Vector2(u, v));
	this->VertexBuffer.push_back(p);
}

//���㸴��ճ����
void Mesh::AddVertexData(float pos_x, float pos_y, float pos_z, float u, float v, const Vector3f normal, Color color)
{
	this->AddvertexData(Vector3f(pos_x, pos_y, pos_z), u, v, normal, color);
}

void Mesh::AddvertexData(const Vector3f pos, float u, float v, const Vector3f normal, Color color) //�����㱣�浽��������
{
	Vertex p(pos, color, normal, Vector2(u, v));
	this->VertexBuffer.push_back(p);
}

void Mesh::PrintTest()
{
	//for (const auto& v : this->VertexBuffer)
	//{
	//	std::cout << "λ�ã�(" << v.Position.x << "," << v.Position.y << "," << v.Position.z << ") ";
	//	std::cout << "��ɫ��(" << v.v_Color.r << "," << v.v_Color.g << "," << v.v_Color.b << "," << v.v_Color.a << ") ";
	//	std::cout << "������: (" << v.Normal.x << "," << v.Normal.y << "," << v.Normal.z << ")";
	//	std::cout << "uv����: (" << v.uv.x << "," << v.uv.y << ")";
	//	std::cout << std::endl;
	//}
}

Mesh::~Mesh()
{

}

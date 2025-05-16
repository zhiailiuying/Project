#pragma once
#include"global.h"
#include"Vector3.h"
#include"Color.h"
#include"Matrix.h"

//������
class Vertex
{
public:

	Vector3f Position;                //����λ��
	Vector3f Normal;                  //���㷨��
	Color  v_Color;                   //������ɫ
	Vector2  uv;                      //����uv
	Vector3i  VertexIndex;            //��������    

	Vertex();                      //Ĭ�Ϲ���
	//�����������캯��Ҫ������ֲ�ͬ����¶���ĳ�ʼ��
	Vertex(const Vector3f &p ,const Color &c,Vector2 uv);    //�����Ĭ�Ϲ���������ʼ�� p , c ,uv;
	Vertex(const Vector3f& p, const Color& c, const Vector3f &normal, Vector2 uv); //��ʼ�� p ,c,n,uv;

	void VertexLerpData(Vertex& left, Vertex& right, float t);          //�����Ҷ�����в�ֵ
	static float Lerp(float v1, float v2, float t);                 //�����ֵ�ı��ʽ //static�����ã�Lerpֻ�ܱ��ļ���ʹ��
	Vertex &operator*(const Matrix& m);                              //���ᱻ�����ļ����ӣ���ֹ����ļ�����������  
	void VertexPrint();
	~Vertex();

	//Сtips������static������extern�滻����������#includeҲ����ֱ���������ļ���func������������ͷ�ļ��󲿷ֲ���Ҫ��func
};
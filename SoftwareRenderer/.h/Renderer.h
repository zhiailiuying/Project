#pragma once
#include"Windows.h"
#include"texture.h"
#include"Light.h"
#include"Camera.h"
#include"Vertex.h"
#include"Mesh.h"
#include"Buffer.h"
#include"Shader.h"
#include"MyMath.h"
#include"Texture.h"

class Renderer
{
public:

	int deviceWidth, deviceHeight;             //�豸��Ⱥ��豸�߶�
	HDC screen;                                //��Ļ�������֤

	Texture* texture;                          //����
	std::vector<DirectionLight> v_DirectionLight;            //�չ�
	std::vector<PointLight> v_PointLight;                     //���
	Camera *camera;                             //���
	int Count;

	Renderer();
	Renderer(HDC hdc,int deviceWidth,int deviceHeight,Camera *cam);       //��ʼ��HDC���豸��ȡ��߶ȡ����
	void Zbuffer_Clear(DepthBuffer *depthbuffer);                      //���z������   
	void DrawByIndex(Mesh *m,Shader *shader,DepthBuffer *depthbuffer);                 //��ȡf
	void DrawByArray(Mesh *m,Shader *shader,DepthBuffer *depthbuffer);                        //���ã�����
	void DrawPixelColor(int x,int y,const Color &color);                       //(inline),������ɫʱ�в�ɫ������
	bool SimpleCVVcullCheck(const Vertex &vertex);                 //��CVV�޳�
	void Draw_Pixel(int x,int y);                           //��������(����ֱ��ֻ�кڰ�ʱ��ֻ��Ĭ����ɫ����)
	void DrawLine2D(int x0,int y0,int x1,int y1);            //���ƶ�άֱ��
	void DrawTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2);      //���ƶ�ά������
	void DrawBottomFlatTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2);   //���ƶ�ά��������
	void DrawTopFlatTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2);     //���ƶ�ά��������
	void DrawMesh(Mesh* mesh,Shader *shader,DepthBuffer *depthbuffer);               //��������
	void DrawPrimitive(Vertex v0,Vertex v1,Vertex v2,Shader *shader,DepthBuffer *depthbuffer);      //����ͼԪ
	void PrepareRasterization(Vertex &vertex,DepthBuffer *buffer);                     //׼����դ��
	void RasterizeTrangle(Vertex v0,Vertex v1,Vertex v2,Shader *shader,DepthBuffer *depthbuffer);    //��ɺ��������
	bool DrawZTestAndWrite(int x,int y,float depth,DepthBuffer *zbuffer);                                   //��z��Ȳ���
	void DrawLine3D(Vertex v0,Vertex v1,Shader *shader,Vector3f p0,Vector3f p1,Vector3f p2,DepthBuffer *buffer);      //������άֱ��
	void DrawTrangle3D(const Vector3f &v0,const Vector3f &v1,const Vector3f &v2,const Matrix &mvp);  //������ά������
	void DrawBottomTrangle3D(Vertex v0,Vertex v1,Vertex v2,Vertex v3,Shader *shader,DepthBuffer *depthbuffer,int swapIndex);  //������ά��������
	void DrawTopTrangle3D(Vertex v0,Vertex v1,Vertex v2,Vertex v3,Shader *shader,DepthBuffer *depthbuffer,int swapIndex);                                         //������ά��������
	Vector3f GetSreenTranform(const Vector3f &v);                                 //�ӿڱ任
	~Renderer();
};
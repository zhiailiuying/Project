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

	int deviceWidth, deviceHeight;             //设备宽度和设备高度
	HDC screen;                                //屏幕绘制许可证

	Texture* texture;                          //纹理
	std::vector<DirectionLight> v_DirectionLight;            //日光
	std::vector<PointLight> v_PointLight;                     //点光
	Camera *camera;                             //相机
	int Count;

	Renderer();
	Renderer(HDC hdc,int deviceWidth,int deviceHeight,Camera *cam);       //初始化HDC、设备宽度、高度、相机
	void Zbuffer_Clear(DepthBuffer *depthbuffer);                      //清空z缓冲区   
	void DrawByIndex(Mesh *m,Shader *shader,DepthBuffer *depthbuffer);                 //读取f
	void DrawByArray(Mesh *m,Shader *shader,DepthBuffer *depthbuffer);                        //作用：存疑
	void DrawPixelColor(int x,int y,const Color &color);                       //(inline),处理着色时有彩色的像素
	bool SimpleCVVcullCheck(const Vertex &vertex);                 //做CVV剔除
	void Draw_Pixel(int x,int y);                           //绘制像素(绘制直线只有黑白时，只用默认颜色即可)
	void DrawLine2D(int x0,int y0,int x1,int y1);            //绘制二维直线
	void DrawTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2);      //绘制二维三角形
	void DrawBottomFlatTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2);   //绘制二维底三角形
	void DrawTopFlatTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2);     //绘制二维顶三角形
	void DrawMesh(Mesh* mesh,Shader *shader,DepthBuffer *depthbuffer);               //绘制网格
	void DrawPrimitive(Vertex v0,Vertex v1,Vertex v2,Shader *shader,DepthBuffer *depthbuffer);      //绘制图元
	void PrepareRasterization(Vertex &vertex,DepthBuffer *buffer);                     //准备光栅化
	void RasterizeTrangle(Vertex v0,Vertex v1,Vertex v2,Shader *shader,DepthBuffer *depthbuffer);    //光珊化三角形
	bool DrawZTestAndWrite(int x,int y,float depth,DepthBuffer *zbuffer);                                   //做z深度测试
	void DrawLine3D(Vertex v0,Vertex v1,Shader *shader,Vector3f p0,Vector3f p1,Vector3f p2,DepthBuffer *buffer);      //绘制三维直线
	void DrawTrangle3D(const Vector3f &v0,const Vector3f &v1,const Vector3f &v2,const Matrix &mvp);  //绘制三维三角形
	void DrawBottomTrangle3D(Vertex v0,Vertex v1,Vertex v2,Vertex v3,Shader *shader,DepthBuffer *depthbuffer,int swapIndex);  //绘制三维底三角形
	void DrawTopTrangle3D(Vertex v0,Vertex v1,Vertex v2,Vertex v3,Shader *shader,DepthBuffer *depthbuffer,int swapIndex);                                         //绘制三维顶三角形
	Vector3f GetSreenTranform(const Vector3f &v);                                 //视口变换
	~Renderer();
};
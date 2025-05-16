#include"Renderer.h"

Renderer::Renderer()
{

}

//有参构造
Renderer::Renderer(HDC hdc,int devicewidth,int deviceheight,Camera *cam)
{
	this->screen = hdc;
	this->deviceWidth = devicewidth;
	this->deviceHeight = deviceheight;
	this->camera = cam;
	this->Count = 0;

	/*Texture* tex = new Texture();
	tex->LoadTexture(L"gezi.bmp");*/
	//this->texture->LoadTexture(L"D:\\project\\cpp\\SoftwareRenderer\\SoftwareRenderer\\gezi.bmp");            //加载纹理
}

//清空缓冲区
void Renderer::Zbuffer_Clear(DepthBuffer *depthbuffer)
{
	//这个函数地作用就是将整个屏幕涂黑（从一个小矩形扩张成大矩形),类似于ps工具里面新建一个小矩形，然后拉右下角扩大差不多(一开始新建的为源矩形，扩大后为目的矩形)
	//1、一开始地两个0,0，意思是从屏幕(0,0）位置盖到（this->deviceWidth, this->deviceHeight）位置
	//2、NULL为源矩形地句柄.没有就不填后面为两个0
	//3、BLACKNESS为全填黑
	BitBlt(this->screen, 0, 0, this->deviceWidth, this->deviceHeight, NULL, NULL, NULL, BLACKNESS);

	for (int i = 0; i < depthbuffer->m_Height; i++)
	{
		for (int j = 0; j < depthbuffer->m_Width; j++)
		{
			depthbuffer->depthBuffer[i][j] = 1;
		}
	}
}

//绘制网格
void Renderer::DrawMesh(Mesh* m, Shader* shader, DepthBuffer* depthbuffer)
{
	//cout << m->IndexBuffer.size() << endl;
	if (m->IndexBuffer.size() > 0)
	{
		this->DrawByIndex(m, shader, depthbuffer);
	}
	else
	{
		this->DrawByArray(m, shader, depthbuffer);
	}
}

//获取f
void Renderer::DrawByIndex(Mesh *m,Shader *shader,DepthBuffer *depthbuffer)
{
	for (int i = 0; i < m->IndexBuffer.size(); i = i + 4)            //读取f索引，一次度4个顶点
	{
		Vertex p1;
		p1.Position = m->PositionBuffer[m->IndexBuffer[i].x - 1];  //x存放的是顶点位置索引
		p1.uv = m->UVBuffer[m->IndexBuffer[i].y - 1];             //y存放的是uv索引
		p1.Normal = m->NormalBuffer[m->IndexBuffer[i].z - 1];         //z存放的是法向量的索引

		Vertex p2;
		p2.Position = m->PositionBuffer[m->IndexBuffer[i + 1].x - 1];
		p2.uv = m->UVBuffer[m->IndexBuffer[i + 1].y - 1];
		p2.Normal = m->NormalBuffer[m->IndexBuffer[i + 1].z - 1];

		Vertex p3;
		p3.Position = m->PositionBuffer[m->IndexBuffer[i + 2].x - 1];
		p3.uv = m->UVBuffer[m->IndexBuffer[i + 2].y - 1];
		p3.Normal = m->NormalBuffer[m->IndexBuffer[i + 2].z - 1];

		Vertex p4;
		p4.Position = m->PositionBuffer[m->IndexBuffer[i + 3].x - 1];
		p4.uv = m->UVBuffer[m->IndexBuffer[i + 3].y - 1];
		p4.Normal = m->NormalBuffer[m->IndexBuffer[i + 3].z - 1];

		this->DrawPrimitive(p1, p2, p3, shader, depthbuffer);          //绘制三角面 
		this->DrawPrimitive(p1, p3, p4, shader, depthbuffer);           //绘制三角面，两个三角面形成一个顺时针排列p1,p2,p3,p4的平面
	}
}

//作用：存疑,直接读取三个顶点
void Renderer::DrawByArray(Mesh *m,Shader *shader,DepthBuffer *depthbuffer)
{
	//std::cout << m->VertexBuffer.size() << std::endl;
	if (m->VertexBuffer.empty())
	{
		return;
	}
	//system("pause");
	//int count = 0;
	for (int i = 0; i < m->VertexBuffer.size() - 1 ; i = i + 3)
	{
		Vertex p1 = m->VertexBuffer[i];
		Vertex p2 = m->VertexBuffer[i + 1];
		Vertex p3 = m->VertexBuffer[i + 2];

		//p1.Position.Print();
		//p2.Position.Print();
		//p3.Position.Print();
		//system("pause");
		//this->Count++;
		this->DrawPrimitive(p1, p2, p3, shader, depthbuffer);
	}
}

//绘制图元
void Renderer::DrawPrimitive(Vertex v0, Vertex v1, Vertex v2, Shader* shader, DepthBuffer* buffer)
{
	//做顶点着色
	shader->VertexShader(v0, v1, v2);

	//透视剔除
	this->PrepareRasterization(v0, buffer);
    this->PrepareRasterization(v1, buffer);
	 this->PrepareRasterization(v2, buffer);
	//视锥剔除,如果三个顶点在视锥外，则不渲染
	//if (this->SimpleCVVcullCheck(v0) && this->SimpleCVVcullCheck(v1) && this->SimpleCVVcullCheck(v2))
	//{
	//	return;
	//}

	//做三角形剔除，当三个顶点相差过小无法覆盖一个像素时，则不渲染
	if (((int)v0.Position.y == (int)v1.Position.y && abs((int)v1.Position.y - (int)v2.Position.y) <= 1) ||
		((int)v0.Position.y == (int)v2.Position.y && abs((int)v2.Position.y - (int)v1.Position.y) <= 1) ||
		((int)v1.Position.y == (int)v2.Position.y && abs((int)v2.Position.y - (int)v0.Position.y) <= 1))
	{
		return;
	}

	//if (this->Count < 50)
	//{
	//	cout << v0.uv.x << "  " << v0.uv.y << endl;
	//	cout << v1.uv.x << "  " << v1.uv.y << endl;
	//	std::cout << "-----------------------------" << std::endl;
	//}
	//else
	//{
	//	system("pause");
	//}

	this->RasterizeTrangle(v0, v1, v2, shader, buffer);
}

//光栅化三角形
void Renderer::RasterizeTrangle(Vertex v0, Vertex v1, Vertex v2, Shader* shader, DepthBuffer* depthbuffer)
{
	/*cout << v0.v_Color.r << "  " << v0.v_Color.g << "  " << v0.v_Color.b << endl;
    cout << v1.v_Color.r << "  " << v1.v_Color.g << "  " << v1.v_Color.b << endl;
    cout << v2.v_Color.r << "  " << v2.v_Color.g << "  " << v2.v_Color.b << endl;
    system("pause");*/
	//cout << "------------------------" << endl;
	//system("pause");
	int ty0 = v0.Position.y;
	int ty1 = v1.Position.y;
	int ty2 = v2.Position.y;

	if (ty0 == ty1)                   //上三角形
	{
		this->DrawTopTrangle3D(v0, v1, v2, Vertex(), shader, depthbuffer, -1);
	}
	else if (ty1 == ty2)               //下三角形
	{
		this->DrawBottomTrangle3D(v0, v1, v2, Vertex(), shader, depthbuffer, -1);
	}
	else                               //划分三角形
	{
		float x_3 = v0.Position.x + (v2.Position.x - v0.Position.x) * (v1.Position.y - v0.Position.y) / (v2.Position.y - v0.Position.y);
		float y_3 = v1.Position.y;
		float t = (y_3 - v0.Position.y) / (v2.Position.y - v0.Position.y);
		Vertex v3(Vector3f(x_3, y_3, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		v3.VertexLerpData(v0, v2, t);
	    //v3.Position.Print();
		//system("pause");
		this->DrawBottomTrangle3D(v0, v1, v3, v2, shader, depthbuffer, 3);
		this->DrawTopTrangle3D(v3, v1, v2, v0, shader, depthbuffer, 1);
	}
}

//绘制三维底三角形
void Renderer::DrawBottomTrangle3D(Vertex v0, Vertex v1, Vertex v2, Vertex v3, Shader* shader, DepthBuffer* depthbuffer, int swapIndex)
{
	//cout << v1.Position.x << endl;
	//cout << swapIndex << endl;
	/*cout << v0.v_Color.r << "  " << v0.v_Color.g << "  " << v0.v_Color.b << endl;
	cout << v1.v_Color.r << "  " << v1.v_Color.g << "  " << v1.v_Color.b << endl;
	cout << v2.v_Color.r << "  " << v2.v_Color.g << "  " << v2.v_Color.b << endl;*/
	/*cout << v0.Position.x << endl;
	cout << v0.Position.y << endl;
	cout << v2.Position.x << endl;
	cout << v2.Position.y << endl;*/
	//v0.Position.Print();
	//v1.Position.Print();
	//v2.Position.Print();
	//v3.Position.Print();
	//system("pause");
	float x0 = v0.Position.x;
	float y0 = v0.Position.y;
	float x1 = v1.Position.x;
	float y1 = v1.Position.y;
	float x2 = v2.Position.x;
	float y2 = v2.Position.y;

	for (float y = y0; y <= y1; y++)
	{
		float t = (y - y0) / (y2 - y0);       //t是y的插值，画个底三角形就知道了，只需计算一次
		int x_l = (y - y0) * (x1 - x0) / (y1 - y0) + x0;
		Vertex v_l(Vector3f(x_l, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		v_l.VertexLerpData(v0, v1, t);

		int x_r = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		Vertex v_r(Vector3f(x_r, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		v_r.VertexLerpData(v0, v2, t);

		//v_l.Position.Print();
		//v_r.Position.Print();
		//system("pause");
		switch (swapIndex)
		{
		case -1:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v1.Position, v2.Position, depthbuffer);    //下
			break;
		case 1:
			this->DrawLine3D(v_l, v_r, shader, v3.Position, v1.Position, v2.Position, depthbuffer);    //上
			break;
		case 2:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v3.Position, v2.Position, depthbuffer);    //全
			break;
		case 3:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v1.Position, v3.Position, depthbuffer);    //线
			break;
		default:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v1.Position, v2.Position, depthbuffer);   //默认为下
			break;
		}
	}
}

//绘制三维顶三角形
void Renderer::DrawTopTrangle3D(Vertex v0, Vertex v1, Vertex v2, Vertex v3, Shader* shader, DepthBuffer* depthbuffer, int swapIndex)
{
	float x0 = v0.Position.x;
	float y0 = v0.Position.y;
	float x1 = v1.Position.x;
	float y1 = v1.Position.y;
	float x2 = v2.Position.x;
	float y2 = v2.Position.y;

	for (float y = y0; y <= y2; y++)
	{
		float t = (y - y0) / (y2 - y0);

		int x_l = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		Vertex v_l(Vector3f(x_l, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		v_l.VertexLerpData(v0, v2, t);

		int x_r = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
		Vertex v_r(Vector3f(x_r, y, 0), Color(0, 0, 0, 0), Vector2(0, 0));
		v_r.VertexLerpData(v1, v2, t);

		//this->Count++;
		//if (this->Count < 50)
		//{
		//	//cout << x_r << endl;
		//	cout << v_l .uv.x << "  " << v_l.uv.y << endl;
		//	cout << v_r.uv.x << "  " << v_r.uv.y << endl;
		//	std::cout << "-----------------------------" << std::endl;
		//}
		//else
		//{
		//	system("pause");
		//}

		//v_l.Position.Print();
		//v_r.Position.Print();
		//system("pause");
		switch (swapIndex)
		{
		case -1:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v1.Position, v2.Position, depthbuffer);    //下
			break;
		case 1:
			this->DrawLine3D(v_l, v_r, shader, v3.Position, v1.Position, v2.Position, depthbuffer);    //上
			break;
		case 2:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v3.Position, v2.Position, depthbuffer);    //全
			break;
		case 3:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v1.Position, v3.Position, depthbuffer);    //线
			break;
		default:
			this->DrawLine3D(v_l, v_r, shader, v0.Position, v1.Position, v2.Position, depthbuffer);   //默认为下
			break;
		}
	}
}

//绘制三维直线(y值永远相等的线)
void Renderer::DrawLine3D(Vertex v0, Vertex v1, Shader* shader, Vector3f p0, Vector3f p1, Vector3f p2, DepthBuffer* depthbuffer)
{
	float x0 = v0.Position.x;
	float y0 = v0.Position.y;
	float x1 = v1.Position.x;
	float y1 = v1.Position.y;
	int dx = x1 - x0;
	int stepx = 1;

	if (dx < 0)
	{
		stepx = -1;
		dx = -dx;
	}
	
	int x = x0;
	int y = y0;

	Vertex frag;
	if (dx == 0)
	{
		if (this->DrawZTestAndWrite(x, y, (v0.Position.z + 1.0f) / 2, depthbuffer));
		{
			Vector3f G = CenterOfGravity(p0, p1, p2, Vector2(x, y));

			if (shader->FragmentShader(v0, G))
			{
				this->DrawPixelColor(x, y, v0.v_Color);
			}
		}
	}
	else
	{
		for (int i = 0; i <= dx; i++)
		{
			float s = static_cast<float>(x - x0) / (x1 - x0);
			float t = s * v0.Position.z / (s * v0.Position.z + (1 - s) * v1.Position.z);
			float z = Vertex::Lerp(v0.Position.z, v1.Position.z, t);
			z = (z + 1.0f) / 2;
			if (this->DrawZTestAndWrite(x, y, z, depthbuffer))
			{
				Color color = Color::Lerp(v0.v_Color, v1.v_Color, t);
				float u = Vertex::Lerp(v0.uv.x, v1.uv.x,t);
				float v = Vertex::Lerp(v0.uv.y, v1.uv.y, t);
				frag.Position = Vector3f(x, y, z);
				frag.v_Color = color;
				frag.uv = Vector2(u, v);

				Vector3f G = CenterOfGravity(p0, p1, p2, Vector2(x, y));
				if (shader->FragmentShader(frag, G))
				{
					this->DrawPixelColor(x, y, frag.v_Color);
				}
			}
			x += stepx;
		}
	}

}

//处理着色时的顶点
void Renderer::DrawPixelColor(int x,int y,const Color &color)
{
	SetPixel(this->screen, x, y, RGB(255 * color.r, 255 * color.g, 255 * color.b));
}

//剔除
bool Renderer::SimpleCVVcullCheck(const Vertex &vertex)
{
	//开始说明一下w的几何意义：表示顶点的深度值，即w = -z;
	//z从0-w,x,y区间+-w共同构成视锥体的可视范围
	float w = vertex.Position.w;
	if (vertex.Position.x < -w || vertex.Position.x > w)
		return true;
	if (vertex.Position.y < -w || vertex.Position.y > w)
		return true;
	if (vertex.Position.z < -w || vertex.Position.z > w)
		return true;
	return false;
}

//绘制像素(纯黑白)
void Renderer::Draw_Pixel(int x,int y)
{
	SetPixel(this->screen, x, y, RGB(0, 0, 0));
}

//绘制2d直线(Bresenham画线算法）
void Renderer::DrawLine2D(int x0,int y0,int x1,int y1)
{
	int x, y, dx, dy, p;                                   //其中p为误差系数，其中 p >= 0时，p = p + 2 * dy - 2 *dx
	dx = x1 - x0;                                          //                当 p < 0 时， p = p + 2 * dy
	dy = y1 - y0;
	//std::cout << "dx: " << dx << "   " << "dy: " << dy << std::endl;
	bool steep = false;
	//1、该画线算法针对的时0 < k < 1 时的情况，当 k > 1 时，将直线翻转到 k < 1 ，然后反过来绘制
	if (dx < dy)
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		dx = x1 - x0;
		dy = y1 - y0;
		steep = true;
	}

	if (dx < 0)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	//第一步p0 为 2 * dy - dx;
	p = 2 * dy - dx;
	x = x0;
	y = y0;
	//std::cout << "p: " << p << "  " << "x: " << x << "  " << "y: " << y << std::endl;
	this->Draw_Pixel(x, y);
	while (x < x1)
	{
		x++;
		if (p < 0)
		{
			p += 2 * dy;
		}
		else
		{
			y += 1;
			p += 2 * dy - 2 * dx;
		}
		if (steep)
		{
			//std::cout << "x : " << x << "  " << "y: " << y << std::endl;
			this->Draw_Pixel(y,x);
		}
		else
		{
			//std::cout << "x : " << x << "  " << "y: " << y << std::endl;
			this->Draw_Pixel(x, y);
		}
	}

}

//绘制2d三角形(划分三角形)
void Renderer::DrawTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2)
{
	//排序,规定y0 < y1 < y2
	//类似与选择排序，先确定y0为最小值，后续比较y1和y2即可
	if (y1 < y0)
	{
		std::swap(x1, x0);
		std::swap(y1, y0);
	}
	if (y2 < y0)
	{
		std::swap(x2, x0);
		std::swap(y2, y0);
	}
	if (y2 < y1)
	{
		std::swap(x2, x1);
		std::swap(y2, y1);
	}

	int ty0 = y0;
	int ty1 = y1;
	int ty2 = y2;

 	if (ty0 == ty1)               //上三角形，因为已经y2一定是最大的，通过其余两条信息可以确定一个三角形
	{
		this->DrawTopFlatTrangle2D(x0, y0, x1, y1, x2, y2);
	}
	if (ty2 == ty1)              //下三角形
	{
		this->DrawBottomFlatTrangle2D(x0, y0, x1, y1, x2, y2);
	}
	else                         //划分三角形
	{
		int y3 = y1;
		int x3 = (y1 - y0) * (x2 - x0) / (y2 - y0) + x0;
		//std::cout << "x3：" << x3 << " " << "y3: " << y3 << std::endl;
		//保证x1一定在x3的左边，如果没有if的话x1有可能在x3的左边/右边，保证了x1和x3在一次绘制中的绝对位置，不然可能一左一右
		if (x1 > x3)
		{
			std::swap(x1, x3);
			std::swap(y1, y3);
		}

		this->DrawBottomFlatTrangle2D(x0, y0, x1, y1, x3, y3);
		this->DrawTopFlatTrangle2D(x1, y1, x3, y3, x2, y2);
	}

}

//处理底三角形(用扫描线一步一步填充)
void Renderer::DrawBottomFlatTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2)
{
	for (int y = y0; y <= y1; y++)
	{
		int x01 = (y - y0) * (x1 - x0) / (y1 - y0) + x0;
		int x02 = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		//std::cout << "x01: " << x01 << "  " << "x02: " << x02 << std::endl;
		this->DrawLine2D(x01, y, x02, y);
	}
}

//处理顶三角形
void Renderer::DrawTopFlatTrangle2D(int x0,int y0,int x1,int y1,int x2,int y2)
{
	for (int y = y0; y <= y2; y++)
	{
		int x02 = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		int x12 = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
		//std::cout << "x02: " << x02 << "  " << "x12: " << x12 << std::endl;
		this->DrawLine2D(x02, y, x12, y);
	}
}




//准备光栅化,将顶点坐标对齐到像素中点
inline void Renderer::PrepareRasterization(Vertex &vertex,DepthBuffer *buffer)
{
	//透视除法,对齐到归一化设备坐标
	float v_w = 1.0f / vertex.Position.w;
	/*cout << buffer->m_Width - 1 << endl;
	cout << buffer->m_Height - 1 << endl;*/
	//system("pause");
	vertex.Position.x = (vertex.Position.x * v_w + 1.0f) * 0.5f * (buffer->m_Width - 1) + 0.5f;
	vertex.Position.y = (vertex.Position.y * v_w + 1.0f) * 0.5f * (buffer->m_Height - 1) + 0.5f;
}


//z深度测试
bool Renderer::DrawZTestAndWrite(int x,int y,float depth,DepthBuffer *zbuffer)
{
	if  (x >= 0 && x < zbuffer->m_Width && y >= 0 && y < zbuffer->m_Height)
	{
		if (zbuffer->depthBuffer[x][y] >= depth)
		{
			zbuffer->depthBuffer[x][y] = depth;
			return true;
		} 
		return false;
	}
	return false;
	//return true;
}



//绘制三维三角形(用于测试mvp变换后和视口变换后)
void Renderer::DrawTrangle3D(const Vector3f &v0,const Vector3f &v1,const Vector3f &v2,const Matrix &mvp)
{
	//做mvp变换
	Vector3f v_0 = mvp.MultiplyVector3(v0);
	Vector3f v_1 = mvp.MultiplyVector3(v1);
	Vector3f v_2 = mvp.MultiplyVector3(v2);

	v_0 = this->GetSreenTranform(v_0);
	v_1 = this->GetSreenTranform(v_1);
	v_2 = this->GetSreenTranform(v_2);


	this->DrawTrangle2D(v_0.x, v_0.y, v_1.x, v_1.y, v_2.x, v_2.y);
}

//视口变换
Vector3f Renderer::GetSreenTranform(const Vector3f &v)
{
	float v_w = 1.0f / v.w;                   //透视除法获得归一化设备坐标
	float x = (v.x * v.w + 1.0f) * 0.5 * this->deviceWidth;
	float y = (1.0f - v.y * v_w) * 0.5 * this->deviceHeight;   //归一化坐标系与屏幕坐标系不同，屏幕坐标系原点在左上，应该y是映射到[0,-1],y轴朝下
	float z = 1 / v.z;
	return Vector3f(x, y, z);
}

Renderer::~Renderer()
{

}

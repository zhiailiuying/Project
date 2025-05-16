#include"Shader.h"

PhongShader::PhongShader()
{

}

//有参构造
PhongShader::PhongShader(Phong_Vertex phongVertex, Phong_fragment phongfragment)
{
	this->phong_vertex = phongVertex;
	this->phong_fragment = phongfragment;
}

//在mvp变换期间做顶点着色，即顶点处理阶段
void PhongShader::VertexShader(Vertex& v_0,Vertex &v_1,Vertex &v_2)
{
	this->phong_vertex.vert_v0 = v_0;
	this->phong_vertex.vert_v1 = v_1;
	this->phong_vertex.vert_v2 = v_2;

	Vertex *shadingPoint = &v_0;                //用指针指向v0,这样对shadingPoint的操作都会直接修改到对应顶点的地址

	//给三个顶点做变换,用于做Shadowmapping
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			shadingPoint = &v_0;
			this->phong_vertex.lightPos0 = v_0.Position;
			//将模型从模型转换到世界坐标（从自己的小天地变为世界大舞台)
			this->phong_vertex.lightPos0 = this->phong_vertex.m.MultiplyVector3(this->phong_vertex.lightPos0);
			////将视图转换到光源视角下的视图
			this->phong_vertex.lightPos0 = this->phong_vertex.Light_View.MultiplyVector3(this->phong_vertex.lightPos0);
			////在光源视图下的正交投影，用来判断顶点位置
			this->phong_vertex.lightPos0 = this->phong_vertex.Light_Ortho.MultiplyVector3(this->phong_vertex.lightPos0);
			break;
		case 1:
			shadingPoint = &v_1;
			this->phong_vertex.lightPos1= v_1.Position;
			this->phong_vertex.lightPos1 = this->phong_vertex.m.MultiplyVector3(this->phong_vertex.lightPos1);
			this->phong_vertex.lightPos1 = this->phong_vertex.Light_View.MultiplyVector3(this->phong_vertex.lightPos1);
			this->phong_vertex.lightPos1 = this->phong_vertex.Light_Ortho.MultiplyVector3(this->phong_vertex.lightPos1);
			break;
		case 2:
			shadingPoint = &v_2;
			this->phong_vertex.lightPos2 = v_2.Position;
			this->phong_vertex.lightPos2 = this->phong_vertex.m.MultiplyVector3(this->phong_vertex.lightPos2);
			this->phong_vertex.lightPos2 = this->phong_vertex.Light_View.MultiplyVector3(this->phong_vertex.lightPos2);
			this->phong_vertex.lightPos2 = this->phong_vertex.Light_Ortho.MultiplyVector3(this->phong_vertex.lightPos2);
			break;
		}

		//做m变换(在灯光空间下)
		shadingPoint->Position = phong_vertex.m.MultiplyVector3(shadingPoint->Position);


		//将顶点的法向量从模型转换到世界空间
		//去掉平移，获取模型本身的局部坐标，这里在不考虑切变变换和缩放变换，如果考虑的话要求模型的逆然后转置矩阵和法向量相乘
		//这里为什么不求逆矩阵的转置矩阵，因为这里的没给求逆矩阵的方法。。。。。。
		Matrix NormalMatrix = phong_vertex.m;
		NormalMatrix.value[0][3] = 0;
		NormalMatrix.value[1][3] = 0;
		NormalMatrix.value[2][3] = 0;
		shadingPoint->Normal = NormalMatrix.MultiplyVector3(shadingPoint->Normal).Normalize();

		//着色
		float diffuse = 0.0;
		float specular = 0.0;
		//环境光
		float ambient = 0.1;
		//Vector3f alpha(50, 100, 150);
		//Vector3f kd(0.5, 0.5, 0.5);                //三个坐标仅做测试使用
		//Vector3f ks(0.5, 0.5, 0.5);
		//Vector3f ka(0.5, 0.5, 0.5);

		//太阳光
		for (auto v_directionLight : this->phong_vertex.v_directionlight)
		{
			Vector3f l = v_directionLight.GetInverstDirection().Normalize();
			//Vector3f r_2 = shadingPoint->Position - v_directionLight.L_Position ;

			//漫反射
			diffuse += max(0, shadingPoint->Normal.Normalize().Dot(l)) * v_directionLight.L_Intensity;

			//镜面反射
			Vector3f V = this->phong_vertex.cameraPosition - shadingPoint->Position;
			Vector3f H = (l + V).Normalize();
			specular += pow(max(0, shadingPoint->Normal.Normalize().Dot(H)),1) * v_directionLight.L_Intensity;

		}

		//将颜色乘到着色点
		shadingPoint->v_Color = shadingPoint->v_Color * min(1, diffuse + specular + ambient);

		//继续做v、p变换
		shadingPoint->Position = phong_vertex.v.MultiplyVector3(shadingPoint->Position);

		shadingPoint->Position = phong_vertex.p.MultiplyVector3(shadingPoint->Position);

		//把着色点的w分量剃掉,计算完了，就可以不用了
		shadingPoint->Position.standardization();

	}

	//排序顶点，按y从小到大排列，保证v2在最高点(y最大) ,确定v2,v1,v0顺时针的三角形
	//1、方便之后划分平顶三角形和平底三角形(排序后就有统一标准了，三个点唯一确定了三角形顶点的顺序)
	//2、确保了扫描线的正确遍历，扫描线需要从最高点开始扫描

	//system("pause");
	if (v_0.Position.y > v_1.Position.y)
	{
		std::swap(this->phong_vertex.lightPos0, this->phong_vertex.lightPos1);
		std::swap(v_0, v_1);
	}
	if (v_0.Position.y > v_2.Position.y)
	{
		std::swap(this->phong_vertex.lightPos0, this->phong_vertex.lightPos2);
		std::swap(v_0, v_2);
	}
	if (v_1.Position.y > v_2.Position.y)
	{
		std::swap(this->phong_vertex.lightPos1, this->phong_vertex.lightPos2);
		std::swap(v_1, v_2);
	}
}

bool PhongShader::FragmentShader(Vertex &v,Vector3f G)
{
	
	//将基础颜色与顶点颜色混合,同时对纹理的uv进行采样
	v.v_Color = v.v_Color * this->phong_fragment.Tex->T_Sample(v.uv.x, v.uv.y);
	//对三个顶点的法向量利用重心坐标G计算,得到当前片元法向量方向
	Vector3f Normal = this->phong_vertex.vert_v0.Normal * G.x + this->phong_vertex.vert_v1.Normal * G.y + this->phong_vertex.vert_v2.Normal * G.z;
	//Normal.Print();

	//计算当前片元在光源中的位置
	Vector3f LightSpace = this->phong_vertex.lightPos0 * G.x + this->phong_vertex.lightPos1 * G.y + this->phong_vertex.lightPos2 * G.z;
	//LightSpace.Print();

	//计算偏移量bias，即当光源正对物体时，阴影偏移量为0.光源非正对物体时，有阴影偏移量为bias；
	float bias = 0.005;
	//计算光源和法向量的角度
	float costheta = 0.0;
	//这里只考虑了一个灯光

	costheta = abs(this->phong_vertex.v_directionlight[0].GetInverstDirection().Normalize().Dot(Normal.Normalize()));
	//偏移因子
	if (this->phong_vertex.v_directionlight.size() > 0)
	{
		//偏移量,0.02f和0.005为经验系数,效果不好的话之后调
		//cout << "test" << endl;
		float Scale = 1.0f - costheta;
		bias = max(0.02f * Scale, 0.005);
	}
	//this->count++;
	//计算阴影贴图，阴影贴图如果在阴影范围内则为1，不在则为0
	float depth = this->CalcutelationShadows(LightSpace, bias);

	v.v_Color = v.v_Color * (1 - depth);              //着色,如果为0则会阴影，给黑色

	return v.v_Color.a > 0;                 //保留不透明像素，做透明度测试
}

//计算阴影贴图
float PhongShader::CalcutelationShadows(Vector3f LightSpace,float bias)
{
	float reciprocalW = 1.0f / LightSpace.w;         //将齐次坐标转换为三维世界的坐标

	//映射阴影贴图(LightSpace为正交空间下，x,y为[-1,1])
	//计算公式:LightSpace.x * reciprocalW + 1.0f) * 0.5f ,将正交空间[-1,1]映射到[0,1],
	//(this->phong_fragment.depthBuffer->m_Width - 1)映射到贴图大小，+ 0.5是因为要将纹理纹素对齐
	//例子：如果是在0-1的情况下,对纹素采样时：0为第一个纹素的左边界、1还是第一个纹素的右边界，采样了两次在同一个纹素就有问题了。
    LightSpace.x = (LightSpace.x * reciprocalW + 1.0f) * 0.5f * (this->phong_fragment.depthBuffer->m_Width - 1) + 0.5f;
	LightSpace.y = (LightSpace.y * reciprocalW + 1.0f) * 0.5f * (this->phong_fragment.depthBuffer->m_Height - 1) + 0.5f;
	//计算深度、 
	//同样是计算从[-1,1]
	////这里为什么么会不一样？因为深度贴图没有z轴类似这个的this->phong_fragment.depthBuffer->m_Height - 1)用不了
	float depth = (LightSpace.z * reciprocalW + 1) / 2.0f;    

	//计算阴影
	float shadow = 0.0f;

	//PCF先留着，解决阴影贴图的锯齿问题有用
	//PCF(a,b);

	//普通阴影的计算方法
	//1、获取最近的深度图，不知道为什么传y,x的打开定义
	//2、利用深度(片元当前在灯光的z位置) - 偏移量 ?> 最近判断阴影，如果大于，这说明该处为阴影
	float closest = this->phong_fragment.depthBuffer->Sample(LightSpace.x, LightSpace.y);

	//cout << depth << endl;
	//cout << closest << endl;
	//cout << bias << endl;
	shadow = depth - bias > closest ? 1 : 0;
	//cout << shadow << endl;
	return shadow;
}

Shadow::Shadow()
{

}

//给shadowmap贴图做mvp变换
void Shadow::VertexShader(Vertex& v0, Vertex& v1, Vertex& v2)
{

	Vertex *v = &v0;

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			v = &v0;
			break;
		case 1:
			v = &v1;
			break;
		case 2:
			v = &v2;
			break;
		}

		//做mvp变换,变换到灯光坐标系下
		v->Position = this->shadow_vertex.m.MultiplyVector3(v->Position);
		v->Position = this->shadow_vertex.lightV_View.MultiplyVector3(v->Position);
		v->Position = this->shadow_vertex.light_Ortho.MultiplyVector3(v->Position);
		v->Position.standardization();                //将齐次坐标转换为三维坐标

	}
	//排序，保证v2在高处(y最大处)
	if (v0.Position.y > v1.Position.y)
	{
		std::swap(v0, v1);
	}
	if (v0.Position.y > v2.Position.y)
	{
		std::swap(v0, v2);
	}
	if (v1.Position.y > v2.Position.y)
	{
		std::swap(v1, v2);
	}
}

bool Shadow::FragmentShader(Vertex &v,Vector3f G)
{
	return false;                                //始终返回false的原因是不需要绘制颜色，只要深度缓冲区拿到了深度信息即可
}


PhongShader::~PhongShader()
{

}

Shadow::~Shadow()
{

}
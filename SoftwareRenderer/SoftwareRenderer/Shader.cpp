#include"Shader.h"

PhongShader::PhongShader()
{

}

//�вι���
PhongShader::PhongShader(Phong_Vertex phongVertex, Phong_fragment phongfragment)
{
	this->phong_vertex = phongVertex;
	this->phong_fragment = phongfragment;
}

//��mvp�任�ڼ���������ɫ�������㴦��׶�
void PhongShader::VertexShader(Vertex& v_0,Vertex &v_1,Vertex &v_2)
{
	this->phong_vertex.vert_v0 = v_0;
	this->phong_vertex.vert_v1 = v_1;
	this->phong_vertex.vert_v2 = v_2;

	Vertex *shadingPoint = &v_0;                //��ָ��ָ��v0,������shadingPoint�Ĳ�������ֱ���޸ĵ���Ӧ����ĵ�ַ

	//�������������任,������Shadowmapping
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			shadingPoint = &v_0;
			this->phong_vertex.lightPos0 = v_0.Position;
			//��ģ�ʹ�ģ��ת�����������꣨���Լ���С��ر�Ϊ�������̨)
			this->phong_vertex.lightPos0 = this->phong_vertex.m.MultiplyVector3(this->phong_vertex.lightPos0);
			////����ͼת������Դ�ӽ��µ���ͼ
			this->phong_vertex.lightPos0 = this->phong_vertex.Light_View.MultiplyVector3(this->phong_vertex.lightPos0);
			////�ڹ�Դ��ͼ�µ�����ͶӰ�������ж϶���λ��
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

		//��m�任(�ڵƹ�ռ���)
		shadingPoint->Position = phong_vertex.m.MultiplyVector3(shadingPoint->Position);


		//������ķ�������ģ��ת��������ռ�
		//ȥ��ƽ�ƣ���ȡģ�ͱ���ľֲ����꣬�����ڲ������б�任�����ű任��������ǵĻ�Ҫ��ģ�͵���Ȼ��ת�þ���ͷ��������
		//����Ϊʲô����������ת�þ�����Ϊ�����û���������ķ���������������
		Matrix NormalMatrix = phong_vertex.m;
		NormalMatrix.value[0][3] = 0;
		NormalMatrix.value[1][3] = 0;
		NormalMatrix.value[2][3] = 0;
		shadingPoint->Normal = NormalMatrix.MultiplyVector3(shadingPoint->Normal).Normalize();

		//��ɫ
		float diffuse = 0.0;
		float specular = 0.0;
		//������
		float ambient = 0.1;
		//Vector3f alpha(50, 100, 150);
		//Vector3f kd(0.5, 0.5, 0.5);                //���������������ʹ��
		//Vector3f ks(0.5, 0.5, 0.5);
		//Vector3f ka(0.5, 0.5, 0.5);

		//̫����
		for (auto v_directionLight : this->phong_vertex.v_directionlight)
		{
			Vector3f l = v_directionLight.GetInverstDirection().Normalize();
			//Vector3f r_2 = shadingPoint->Position - v_directionLight.L_Position ;

			//������
			diffuse += max(0, shadingPoint->Normal.Normalize().Dot(l)) * v_directionLight.L_Intensity;

			//���淴��
			Vector3f V = this->phong_vertex.cameraPosition - shadingPoint->Position;
			Vector3f H = (l + V).Normalize();
			specular += pow(max(0, shadingPoint->Normal.Normalize().Dot(H)),1) * v_directionLight.L_Intensity;

		}

		//����ɫ�˵���ɫ��
		shadingPoint->v_Color = shadingPoint->v_Color * min(1, diffuse + specular + ambient);

		//������v��p�任
		shadingPoint->Position = phong_vertex.v.MultiplyVector3(shadingPoint->Position);

		shadingPoint->Position = phong_vertex.p.MultiplyVector3(shadingPoint->Position);

		//����ɫ���w�������,�������ˣ��Ϳ��Բ�����
		shadingPoint->Position.standardization();

	}

	//���򶥵㣬��y��С�������У���֤v2����ߵ�(y���) ,ȷ��v2,v1,v0˳ʱ���������
	//1������֮�󻮷�ƽ�������κ�ƽ��������(��������ͳһ��׼�ˣ�������Ψһȷ���������ζ����˳��)
	//2��ȷ����ɨ���ߵ���ȷ������ɨ������Ҫ����ߵ㿪ʼɨ��

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
	
	//��������ɫ�붥����ɫ���,ͬʱ�������uv���в���
	v.v_Color = v.v_Color * this->phong_fragment.Tex->T_Sample(v.uv.x, v.uv.y);
	//����������ķ�����������������G����,�õ���ǰƬԪ����������
	Vector3f Normal = this->phong_vertex.vert_v0.Normal * G.x + this->phong_vertex.vert_v1.Normal * G.y + this->phong_vertex.vert_v2.Normal * G.z;
	//Normal.Print();

	//���㵱ǰƬԪ�ڹ�Դ�е�λ��
	Vector3f LightSpace = this->phong_vertex.lightPos0 * G.x + this->phong_vertex.lightPos1 * G.y + this->phong_vertex.lightPos2 * G.z;
	//LightSpace.Print();

	//����ƫ����bias��������Դ��������ʱ����Ӱƫ����Ϊ0.��Դ����������ʱ������Ӱƫ����Ϊbias��
	float bias = 0.005;
	//�����Դ�ͷ������ĽǶ�
	float costheta = 0.0;
	//����ֻ������һ���ƹ�

	costheta = abs(this->phong_vertex.v_directionlight[0].GetInverstDirection().Normalize().Dot(Normal.Normalize()));
	//ƫ������
	if (this->phong_vertex.v_directionlight.size() > 0)
	{
		//ƫ����,0.02f��0.005Ϊ����ϵ��,Ч�����õĻ�֮���
		//cout << "test" << endl;
		float Scale = 1.0f - costheta;
		bias = max(0.02f * Scale, 0.005);
	}
	//this->count++;
	//������Ӱ��ͼ����Ӱ��ͼ�������Ӱ��Χ����Ϊ1��������Ϊ0
	float depth = this->CalcutelationShadows(LightSpace, bias);

	v.v_Color = v.v_Color * (1 - depth);              //��ɫ,���Ϊ0�����Ӱ������ɫ

	return v.v_Color.a > 0;                 //������͸�����أ���͸���Ȳ���
}

//������Ӱ��ͼ
float PhongShader::CalcutelationShadows(Vector3f LightSpace,float bias)
{
	float reciprocalW = 1.0f / LightSpace.w;         //���������ת��Ϊ��ά���������

	//ӳ����Ӱ��ͼ(LightSpaceΪ�����ռ��£�x,yΪ[-1,1])
	//���㹫ʽ:LightSpace.x * reciprocalW + 1.0f) * 0.5f ,�������ռ�[-1,1]ӳ�䵽[0,1],
	//(this->phong_fragment.depthBuffer->m_Width - 1)ӳ�䵽��ͼ��С��+ 0.5����ΪҪ���������ض���
	//���ӣ��������0-1�������,�����ز���ʱ��0Ϊ��һ�����ص���߽硢1���ǵ�һ�����ص��ұ߽磬������������ͬһ�����ؾ��������ˡ�
    LightSpace.x = (LightSpace.x * reciprocalW + 1.0f) * 0.5f * (this->phong_fragment.depthBuffer->m_Width - 1) + 0.5f;
	LightSpace.y = (LightSpace.y * reciprocalW + 1.0f) * 0.5f * (this->phong_fragment.depthBuffer->m_Height - 1) + 0.5f;
	//������ȡ� 
	//ͬ���Ǽ����[-1,1]
	////����Ϊʲôô�᲻һ������Ϊ�����ͼû��z�����������this->phong_fragment.depthBuffer->m_Height - 1)�ò���
	float depth = (LightSpace.z * reciprocalW + 1) / 2.0f;    

	//������Ӱ
	float shadow = 0.0f;

	//PCF�����ţ������Ӱ��ͼ�ľ����������
	//PCF(a,b);

	//��ͨ��Ӱ�ļ��㷽��
	//1����ȡ��������ͼ����֪��Ϊʲô��y,x�Ĵ򿪶���
	//2���������(ƬԪ��ǰ�ڵƹ��zλ��) - ƫ���� ?> ����ж���Ӱ��������ڣ���˵���ô�Ϊ��Ӱ
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

//��shadowmap��ͼ��mvp�任
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

		//��mvp�任,�任���ƹ�����ϵ��
		v->Position = this->shadow_vertex.m.MultiplyVector3(v->Position);
		v->Position = this->shadow_vertex.lightV_View.MultiplyVector3(v->Position);
		v->Position = this->shadow_vertex.light_Ortho.MultiplyVector3(v->Position);
		v->Position.standardization();                //���������ת��Ϊ��ά����

	}
	//���򣬱�֤v2�ڸߴ�(y���)
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
	return false;                                //ʼ�շ���false��ԭ���ǲ���Ҫ������ɫ��ֻҪ��Ȼ������õ��������Ϣ����
}


PhongShader::~PhongShader()
{

}

Shadow::~Shadow()
{

}
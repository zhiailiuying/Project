#include"global.h"
#include"ObjfileReader.h"
#include"Buffer.h"
#include"Vertex.h"
#include"Mesh.h"
#include"Windows.h"
#include"Input.h"
#pragma comment(lib,"winmm.lib")              //Ҫ��timeGetTime
#include"Texture.h"
#include"Camera.h"
#include"Light.h"
#include"Shader.h"
#include"Renderer.h"
//using namespace std;

void CreatCude();                    //����������
void CreatPlane();                    //����ģ��
void upDateInput(unique_ptr<Mesh> &m);                         //����ģ��
void upDatewindow(unique_ptr<Windows>& w);                     //�ƶ����ڡ���������Ϣ
void doRender(unique_ptr<Windows> &w);                         //�����������Ⱦ
void ShowFPS(unique_ptr<Windows> &w);                 //����phoneShader
void DeleteData();

//��ʼ����Ȼ�����
static const int windowWidth = 500;
static const int windowHeight = 500;

auto currentMesh = make_unique<Mesh>();     //������ǰ����
auto planeMesh = make_unique<Mesh>();        //����ģ������
auto m = make_unique<Mesh>();                //��ģ�Ͷ�����в���
Vector3f translateVector;                 //��¼��ģ�͵�ƽ�ƺ���ת
Vector3f rotateVector;
auto depthBuffer = make_unique<DepthBuffer>(windowWidth * 2, windowHeight * 2);  //����shadowmap
auto zBuffer = make_unique<DepthBuffer>(windowWidth, windowHeight);             //zbuffer��depthbuffer��һ����
auto camera = make_unique<Camera>(Transform(Vector3f(0, 0, 100), Vector3f(0, 0, 0), Vector3f(0, 0, 0)));     //��ʼ�����λ��(0��0��100��,��ת��0��0��0),���ţ�0��0��0��
Shadow* depthShadow = new Shadow();
PhongShader* phongShader;
PhongShader* planeShader;
auto window = make_unique<Windows>(windowWidth, windowHeight, "Test");
//Renderer* device = new Renderer(window->hdc, windowWidth, windowHeight, camera);
auto device = make_unique<Renderer>(window->ScreenHdc, windowWidth, windowHeight, camera);

void CreatCude()
{
	currentMesh->AddVertexData(-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));     //����������Ϊ��������������
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	//ǰ
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	//��
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(-1, 0, 0),Color(1,1,1,1));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	//��
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	//��
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	//��
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 1, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 1, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 1, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 1, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 1, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 1, 0), Color(1, 1, 1, 1));
}

void CreatPlane()
{
	planeMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	planeMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	planeMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	planeMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	planeMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));
	planeMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(1, 1, 1, 1));

	//��ģ�ͱ任Ϊ(5,5,5),ƽ����Դ�һ��
	Matrix transFormMatrix;
	Matrix s = m->transform.ScaleMatrix(Vector3f(5, 5, 5));
	Matrix rot = m->transform.Rotate(Vector3f(0, 0, 0));
	Matrix t = m->transform.Translate(Vector3f(0, 0, -5));     //����Ļ�ռ�����5�񣬶��뵽��������
	transFormMatrix = t * rot * s;
	planeShader->phong_vertex.m = transFormMatrix;
	planeMesh->SetObjectToWorld(transFormMatrix);                      //��ģ�Ͷ�����������
}

void upDateInput(unique_ptr<Mesh> &m)
{
	if (IS_KEY_DOWN('A'))
	{
		translateVector.x -= 0.01f;
	}
	if (IS_KEY_DOWN('D'))
	{
		translateVector.x += 0.01f;
	}
	if (IS_KEY_DOWN('W'))
	{
		translateVector.y += 0.01f;
	}
	if (IS_KEY_DOWN('S'))
	{
		translateVector.y -= 0.01f;
	}
	if (IS_KEY_DOWN('E'))
	{
		translateVector.z -= 0.1f;
	}
	if (IS_KEY_DOWN('Q'))
	{
		translateVector.z += 0.1f;
	}
	if (IS_KEY_DOWN('K'))
	{
		rotateVector.y -= 1.0f;
	}
	if (IS_KEY_DOWN('I'))
	{
		rotateVector.y += 0.1f;
	}
	if (IS_KEY_DOWN('L'))
	{
		rotateVector.x += 0.5f;
	}
	if (IS_KEY_DOWN('J'))
	{
		rotateVector.x -= 0.5f;
	}
	if (IS_KEY_DOWN('U'))
	{
		rotateVector.z -= 0.5f;
	}
	if (IS_KEY_DOWN('O'))
	{
		rotateVector.z += 0.1f;
	}


	Matrix scale = m->transform.ScaleMatrix(Vector3f(1, 1, 1));              //��ģ������Ϊ(1,1,1),������ͶӰ
	Matrix rot = m->transform.Rotate(rotateVector);                        //����ģ����ת 
	Matrix t = m->transform.Translate(translateVector);                    //����ģ��ƽ��
	Matrix transFormMatrix = t * rot * scale;   
	phongShader->phong_vertex.m = transFormMatrix;
	depthShadow->shadow_vertex.m = transFormMatrix;
}

void upDatewindow(unique_ptr<Windows> &w)
{

	MSG msg = { 0 };

	//1��PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)����ϵͳ�����ϵ���Ϣ
	//2��TranslateMessage(&msg)��ϵͳ����Ϣת��ΪASCII�ַ�
	//3��DispatchMessage(&msg)���͵�ָ������

	while (msg.message != WM_QUIT)
	{
		upDateInput(currentMesh);            //�����񴫽������ж������,�����õ���GetAsyncKeyState����ʵʱ��ⰴ����������Ϣǰ��λ��Ӱ�첻��
		//Peekmessage:������Ϣ
		//min��max��Ϊ0����ɸѡ��Ϣ
		//PM_REMOVE��������Ϣ������
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))       
		{
			TranslateMessage(&msg);              //�������ֵת��ΪASCII�ַ���ԭ��ÿ�μ��̰���ʱϵͳ����WM_KEYDOWN��WM_KEYUP����Ϣ
			                              //���������Ӳ���wParam�� lParam����������������������ɨ���룬֮����Ҫת��ΪASCII�ַ���IS_KEY_DOWN('D')ʹ��
			DispatchMessage(&msg);        //������ʽ����MsgProW
		}                                        
		else                              //�����б���Ϣ��PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)����ֵΪ0,ִ����Ⱦ
		{
			doRender(w);                  //�����ִ����Ⱦ����
			ShowFPS(w);
		}
	}

}

//֮������Render֮��ʵ��
void doRender(unique_ptr<Windows> &w)
{
	device->Zbuffer_Clear(depthBuffer);
	device->Zbuffer_Clear(zBuffer);

	//t->DrawTexture(w->hdc, 0, 0);
	device->DrawMesh(currentMesh, depthShadow, depthBuffer );      //��ȡ��ǰ��������ͼ
	device->DrawMesh(planeMesh, depthShadow, depthBuffer);          //��ȡƽ������ͼ
	//Matrix M;

	device->DrawMesh(currentMesh, phongShader, zBuffer);            //��phongShader
	device->DrawMesh(planeMesh, planeShader, zBuffer);              

	//BitBlt������Renderer.cpp������ϸ˵��
	//SRCCOPYΪ��Դ���θ��Ƶ�Ŀ�����
	BitBlt(w->hdc, 0, 0, windowWidth,windowHeight, w->ScreenHdc, 0, 0, SRCCOPY);
}

void ShowFPS(unique_ptr<Windows>& w)
{
	static float fps = 0.0f;
	static float currentTime = 0.0f;
	static float lastTime = 0.0f;
	static int frameCount = 0;             //����Ϊ��̬����������ֻ��ʼ��һ�Σ�֮������ȷ�ۼӣ���Ȼÿ�ζ��ᱻ��Ϊ0
	currentTime = timeGetTime() * 0.001f;
	float DeltaTime = currentTime - lastTime;

	frameCount++;
	if (DeltaTime > 1.0f)
	{
		fps = static_cast<float>(frameCount) / DeltaTime;
		lastTime = currentTime;
		frameCount = 0;
	}
	//cout << frameCount << endl;
	//cout << "fps: " << fps << endl;
	wchar_t strBuffer[20];
	swprintf_s(strBuffer,L"%s %0.3f", L"fps: ",fps);       //��swprintf�汾�ĺ���д����ַ�
	TextOut(w->hdc, 0, 0, strBuffer, wcslen(strBuffer));          //wcslen������ַ��Ĵ�С
}

int main()
{
	//�ӽ�λ�ã��ӽ���Զ����z(0,0,-1)��������Ϊ(0,1,0)
	Matrix cameraV = camera->LookAt(camera->transform.position, Vector3f(0, 0, -1), Vector3f(0, 1, 0)); 
	//��;�ȴ���
	//��ȡ�������������
	Matrix cameraP = camera->Orthographic(-10, 10, 0, -120, 10, -10);
	//��ȡ�ƹ�������е���������
	Matrix LightP = camera->Orthographic(-10, 10, 0, -120, 10, -10);

	//��ʼ���չ�(����ֵ��չ�λ��)
	DirectionLight light(Vector3f(0.2, 0.2, -1), Vector3f(0,0,100), 1, true);
	//��ʼ�����
	PointLight pointLight(Vector3f(5, 5, -5), 2);

	//��������
	Texture* texture = new Texture();
	texture->LoadTexture(L"gezi.bmp");

	//������ɫ
	//1���������������ʼ��ʹ��Phong_Vertex()
	phongShader = new PhongShader(Phong_Vertex(),Phong_fragment(texture,depthBuffer));
	phongShader->phong_vertex.cameraPosition = camera->transform.position;
	//��ȡ�ƹ��ӽǷ���,�ƹ�Ĭ�ϳ�(0,0,1)
	phongShader->phong_vertex.Light_View = light.LookAt(Vector3f(0,1,0));
	//��ȡ�ƹ��ӽ��¿ռ�
	phongShader->phong_vertex.Light_Ortho = LightP;
	phongShader->phong_vertex.v = cameraV;
	phongShader->phong_vertex.p = cameraP;
	//����ƹ�
	phongShader->phong_vertex.v_directionlight.push_back(light);
	phongShader->phong_vertex.v_pointlight.push_back(pointLight);

	//��Ӱ��ɫ
	depthShadow->shadow_vertex.lightV_View =light.LookAt(Vector3f(0,1,0));
	depthShadow->shadow_vertex.light_Ortho = LightP;
	//depthShadow->shadow_vertex.light_Ortho.Print();
	//system("pause");

	//ƽ����ɫ
	planeShader = new PhongShader(Phong_Vertex(), Phong_fragment(texture,depthBuffer));
	planeShader->phong_vertex.cameraPosition = camera->transform.position;
	planeShader->phong_vertex.Light_View = light.LookAt(Vector3f(0, 1, 0));
	planeShader->phong_vertex.Light_Ortho = LightP;
	planeShader->phong_vertex.v = cameraV;
	planeShader->phong_vertex.p = LightP;
	planeShader->phong_vertex.v_directionlight.push_back(light);
	planeShader->phong_vertex.v_pointlight.push_back(pointLight);

	device->v_DirectionLight.push_back(light);
	device->v_PointLight.push_back(pointLight);

	ObjfileReader obj;
	obj.ReadObj(MODEL,m);

	CreatCude();
	CreatPlane();

	upDatewindow(window);

	system("pause");
	DeleteData();
	return 0;
}

void DeleteData()
{
	if (depthShadow != nullptr)
	{
		delete depthShadow;
	}
	if (phongShader != nullptr)
	{
		delete phongShader;
	}
	if (planeShader != nullptr)
	{
		delete planeShader;
	}

}

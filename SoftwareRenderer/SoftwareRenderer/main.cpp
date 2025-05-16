#include"global.h"
#include"ObjfileReader.h"
#include"Buffer.h"
#include"Vertex.h"
#include"Mesh.h"
#include"Windows.h"
#include"Input.h"
#pragma comment(lib,"winmm.lib")              //要用timeGetTime
#include"Texture.h"
#include"Camera.h"
#include"Light.h"
#include"Shader.h"
#include"Renderer.h"
//using namespace std;

void CreatCude();                    //创建立方体
void CreatPlane();                    //创建模型
void upDateInput(unique_ptr<Mesh> &m);                         //控制模型
void upDatewindow(unique_ptr<Windows>& w);                     //移动窗口、处理窗口信息
void doRender(unique_ptr<Windows> &w);                         //对网格进行渲染
void ShowFPS(unique_ptr<Windows> &w);                 //进行phoneShader
void DeleteData();

//初始化深度缓冲区
static const int windowWidth = 500;
static const int windowHeight = 500;

auto currentMesh = make_unique<Mesh>();     //创建当前网格
auto planeMesh = make_unique<Mesh>();        //创建模型网格
auto m = make_unique<Mesh>();                //对模型顶点进行操作
Vector3f translateVector;                 //记录对模型的平移和旋转
Vector3f rotateVector;
auto depthBuffer = make_unique<DepthBuffer>(windowWidth * 2, windowHeight * 2);  //用于shadowmap
auto zBuffer = make_unique<DepthBuffer>(windowWidth, windowHeight);             //zbuffer和depthbuffer是一样的
auto camera = make_unique<Camera>(Transform(Vector3f(0, 0, 100), Vector3f(0, 0, 0), Vector3f(0, 0, 0)));     //初始化相机位置(0，0，100）,旋转（0，0，0),缩放（0，0，0）
Shadow* depthShadow = new Shadow();
PhongShader* phongShader;
PhongShader* planeShader;
auto window = make_unique<Windows>(windowWidth, windowHeight, "Test");
//Renderer* device = new Renderer(window->hdc, windowWidth, windowHeight, camera);
auto device = make_unique<Renderer>(window->ScreenHdc, windowWidth, windowHeight, camera);

void CreatCude()
{
	currentMesh->AddVertexData(-1.0f,-1.0f,-1.0f, 0.0f, 0.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));     //六个顶点因为创建的是三角面
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, Vector3f(0, 0, -1), Color(1, 0, 0, 1));
	//前
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, 0, 1), Color(0, 1, 0, 1));
	//左
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(-1, 0, 0),Color(1,1,1,1));
	currentMesh->AddVertexData(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	currentMesh->AddVertexData(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(-1, 0, 0), Color(1, 1, 1, 1));
	//右
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	currentMesh->AddVertexData(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Vector3f(1, 0, 0), Color(0, 0, 1, 1));
	//下
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	currentMesh->AddVertexData(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, Vector3f(0, -1, 0), Color(1, 1, 0, 1));
	//上
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

	//将模型变换为(5,5,5),平面可以大一点
	Matrix transFormMatrix;
	Matrix s = m->transform.ScaleMatrix(Vector3f(5, 5, 5));
	Matrix rot = m->transform.Rotate(Vector3f(0, 0, 0));
	Matrix t = m->transform.Translate(Vector3f(0, 0, -5));     //往屏幕空间内移5格，对齐到世界坐标
	transFormMatrix = t * rot * s;
	planeShader->phong_vertex.m = transFormMatrix;
	planeMesh->SetObjectToWorld(transFormMatrix);                      //将模型对齐世界坐标
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


	Matrix scale = m->transform.ScaleMatrix(Vector3f(1, 1, 1));              //将模型缩放为(1,1,1),即正交投影
	Matrix rot = m->transform.Rotate(rotateVector);                        //控制模型旋转 
	Matrix t = m->transform.Translate(translateVector);                    //控制模型平移
	Matrix transFormMatrix = t * rot * scale;   
	phongShader->phong_vertex.m = transFormMatrix;
	depthShadow->shadow_vertex.m = transFormMatrix;
}

void upDatewindow(unique_ptr<Windows> &w)
{

	MSG msg = { 0 };

	//1、PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)接收系统队列上的消息
	//2、TranslateMessage(&msg)将系统的消息转化为ASCII字符
	//3、DispatchMessage(&msg)发送到指定窗口

	while (msg.message != WM_QUIT)
	{
		upDateInput(currentMesh);            //将网格传进来进行顶点操作,这里用的是GetAsyncKeyState可以实时检测按键，处理信息前后位置影响不大
		//Peekmessage:检索消息
		//min和max都为0即不筛选信息
		//PM_REMOVE检索完消息后销毁
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))       
		{
			TranslateMessage(&msg);              //将虚拟键值转化为ASCII字符，原因：每次键盘按下时系统产生WM_KEYDOWN、WM_KEYUP的消息
			                              //这两个附加参数wParam和 lParam往往包含的是虚拟键代码和扫描码，之后需要转化为ASCII字符供IS_KEY_DOWN('D')使用
			DispatchMessage(&msg);        //里面隐式调用MsgProW
		}                                        
		else                              //读完列表消息后PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)返回值为0,执行渲染
		{
			doRender(w);                  //读完后执行渲染操作
			ShowFPS(w);
		}
	}

}

//之后做完Render之后实现
void doRender(unique_ptr<Windows> &w)
{
	device->Zbuffer_Clear(depthBuffer);
	device->Zbuffer_Clear(zBuffer);

	//t->DrawTexture(w->hdc, 0, 0);
	device->DrawMesh(currentMesh, depthShadow, depthBuffer );      //获取当前网格的深度图
	device->DrawMesh(planeMesh, depthShadow, depthBuffer);          //获取平面的深度图
	//Matrix M;

	device->DrawMesh(currentMesh, phongShader, zBuffer);            //做phongShader
	device->DrawMesh(planeMesh, planeShader, zBuffer);              

	//BitBlt具体在Renderer.cpp中有详细说明
	//SRCCOPY为从源矩形复制到目标矩形
	BitBlt(w->hdc, 0, 0, windowWidth,windowHeight, w->ScreenHdc, 0, 0, SRCCOPY);
}

void ShowFPS(unique_ptr<Windows>& w)
{
	static float fps = 0.0f;
	static float currentTime = 0.0f;
	static float lastTime = 0.0f;
	static int frameCount = 0;             //声明为静态变量，这样只初始化一次，之后能正确累加，不然每次都会被置为0
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
	swprintf_s(strBuffer,L"%s %0.3f", L"fps: ",fps);       //用swprintf版本的函数写入宽字符
	TextOut(w->hdc, 0, 0, strBuffer, wcslen(strBuffer));          //wcslen计算宽字符的大小
}

int main()
{
	//视角位置，视角永远看向负z(0,0,-1)，上向量为(0,1,0)
	Matrix cameraV = camera->LookAt(camera->transform.position, Vector3f(0, 0, -1), Vector3f(0, 1, 0)); 
	//用途先存疑
	//获取相机的正交矩阵
	Matrix cameraP = camera->Orthographic(-10, 10, 0, -120, 10, -10);
	//获取灯光在相机中的正交矩阵
	Matrix LightP = camera->Orthographic(-10, 10, 0, -120, 10, -10);

	//初始化日光(好奇怪的日光位置)
	DirectionLight light(Vector3f(0.2, 0.2, -1), Vector3f(0,0,100), 1, true);
	//初始化点光
	PointLight pointLight(Vector3f(5, 5, -5), 2);

	//加载纹理
	Texture* texture = new Texture();
	texture->LoadTexture(L"gezi.bmp");

	//顶点着色
	//1、匿名对象仅作初始化使用Phong_Vertex()
	phongShader = new PhongShader(Phong_Vertex(),Phong_fragment(texture,depthBuffer));
	phongShader->phong_vertex.cameraPosition = camera->transform.position;
	//获取灯光视角方向,灯光默认朝(0,0,1)
	phongShader->phong_vertex.Light_View = light.LookAt(Vector3f(0,1,0));
	//获取灯光视角下空间
	phongShader->phong_vertex.Light_Ortho = LightP;
	phongShader->phong_vertex.v = cameraV;
	phongShader->phong_vertex.p = cameraP;
	//放入灯光
	phongShader->phong_vertex.v_directionlight.push_back(light);
	phongShader->phong_vertex.v_pointlight.push_back(pointLight);

	//阴影着色
	depthShadow->shadow_vertex.lightV_View =light.LookAt(Vector3f(0,1,0));
	depthShadow->shadow_vertex.light_Ortho = LightP;
	//depthShadow->shadow_vertex.light_Ortho.Print();
	//system("pause");

	//平面着色
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

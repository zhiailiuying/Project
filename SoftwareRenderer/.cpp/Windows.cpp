#include"Windows.h"

Windows::Windows()
{
}

//UNIT:消息类型，如WM_PAINT为重绘，WM_CLOSE为关闭窗口
//WPARAM为接收键击 ,如KEY_DOWN
//LPARAM为接收鼠标点击
LRESULT WINAPI MsgProcW(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Windows* window = static_cast<Windows*>(GetPropW(hWnd, L"MyAppWindowPtr"));   //GetPropA(hwnd, "MyAppWindowPtr")返回值为类型HANDLE，强转为Windows *
	if (!window)         //如果没有关联的windows对象，则调用默认的信息处理函数、以保证窗口正常关闭、移动
	{
		return DefWindowProcW(hWnd, msg, wparam, lparam);
	}
	switch (msg)            //消息处理分支,增加一个最小化和关闭
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hWnd, msg, wparam, lparam);        //最终信息处理函数
}


//初始化窗口
Windows::Windows(int w, int h, const char *name)
{
	this->WindowsWidth = w;
	this->WindowsHeight = h;

	//CS_BYTEALIGNCLIENT:每次移动窗口窗口往左位移1字节，即8bit
	//0，0，0分别为给窗口设置为缺省(默认)类型窗口类无扩展(直接使用基类）、窗口实例无扩展,即不分配字节
	//无扩展：普通标题栏、系统菜单、关闭按钮、无透明按钮、高级拖放
	//NULL：叉号使用缺省类型、默认箭头光标、不自动擦除、无菜单、
	//窗口类名称为：Test
	WNDCLASS wndClass = { CS_BYTEALIGNCLIENT,(WNDPROC)MsgProcW,0,0,0,NULL,NULL,NULL,NULL,L"Test"};

	wndClass.hInstance = GetModuleHandle(NULL);           //初始化、创建窗口句柄

	if (!RegisterClassW(&wndClass))                       //注册一个窗口类
	{
		return;                          //若注册失败、则返回
	}

	//创建窗口
	//窗口类名称：Test
	//窗口标题：Test
	//窗口风格：1、产生一个层叠的窗口，一个层叠的窗口有一个标题条和一个边框
	//2、创建一个有标题框的窗口
	//3、创建一个在标题条上带有窗口菜单的窗口
	//4、创建一个具有最小化按钮的窗口
	//初始x、y坐标为0，初始窗口尺寸x,y为0
	//父窗口句柄:NULL,窗口菜单句柄:NULL，程序实例句柄：wnd.Class.hInstance
	//创建参数：NULL
	//std::cout << "Test" << std::endl;
	window = CreateWindow(L"Test", L"Test", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,0,0,0,0,NULL,NULL,wndClass.hInstance,NULL);
	if (!window)
	{
		//std::cout << "test" << std::endl;
		return;
	}

	this->hdc = GetDC(this->window);           //获取内存DC,DC:一个像素宽和一个像素高的内存
	this->ScreenHdc = CreateCompatibleDC(this->hdc);      //绘制与屏幕空间等大小的众多DC
    //绘制窗口
	//以下都蒙圈了，不知道干啥用了喵
	//1、BITMAPINFO所需要的内存数
	//2、屏幕宽度、屏幕高度
	//3、投放设备平面数为1
	//4、32位深的位图
	//5、BI_RGB：不压缩的32位RGB格式
	//6、图片大小为this->WindowsHeight * this->WindowsWidth * 4,因为 色深 = 4字节 / 像素（8bit) = 32 / 8，，即每一个像素的色深为4
	//7、水平分辨率为0，竖直分辨率为0
	//8、颜色索引数和重要的颜色数为0，即全部和全部都重要

	BITMAPINFO bitmapInfo = { sizeof(BITMAPINFOHEADER),this->WindowsWidth ,this->WindowsHeight,1,32,BI_RGB,this->WindowsHeight * this->WindowsWidth * 4, 0,0,0,0 };
	//创建DIB位图
	LPVOID ptr;
	HBITMAP hbitmap = CreateDIBSection(this->ScreenHdc, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
	
	if (!hbitmap)
	{
		return;
	}

	HBITMAP screenObject = (HBITMAP)SelectObject(this->ScreenHdc, hbitmap);

	//设置窗口位置
	SetWindowPos(this->window, NULL, 50, 50, this->WindowsWidth, this->WindowsHeight, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

	//显示窗口，SW_NORMAL位于z缓冲区顶部
	ShowWindow(this->window, SW_NORMAL);
	UpdateWindow(this->window);

	//std::cout << "test" << std::endl;
}


Windows::~Windows()
{
	DeleteDC(this->ScreenHdc);
	ReleaseDC(this->window, this->ScreenHdc);
	DestroyWindow(this->window);

}
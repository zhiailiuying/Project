#include"Windows.h"

Windows::Windows()
{
}

//UNIT:��Ϣ���ͣ���WM_PAINTΪ�ػ棬WM_CLOSEΪ�رմ���
//WPARAMΪ���ռ��� ,��KEY_DOWN
//LPARAMΪ���������
LRESULT WINAPI MsgProcW(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Windows* window = static_cast<Windows*>(GetPropW(hWnd, L"MyAppWindowPtr"));   //GetPropA(hwnd, "MyAppWindowPtr")����ֵΪ����HANDLE��ǿתΪWindows *
	if (!window)         //���û�й�����windows���������Ĭ�ϵ���Ϣ���������Ա�֤���������رա��ƶ�
	{
		return DefWindowProcW(hWnd, msg, wparam, lparam);
	}
	switch (msg)            //��Ϣ�����֧,����һ����С���͹ر�
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProcW(hWnd, msg, wparam, lparam);        //������Ϣ������
}


//��ʼ������
Windows::Windows(int w, int h, const char *name)
{
	this->WindowsWidth = w;
	this->WindowsHeight = h;

	//CS_BYTEALIGNCLIENT:ÿ���ƶ����ڴ�������λ��1�ֽڣ���8bit
	//0��0��0�ֱ�Ϊ����������Ϊȱʡ(Ĭ��)���ʹ���������չ(ֱ��ʹ�û��ࣩ������ʵ������չ,���������ֽ�
	//����չ����ͨ��������ϵͳ�˵����رհ�ť����͸����ť���߼��Ϸ�
	//NULL�����ʹ��ȱʡ���͡�Ĭ�ϼ�ͷ��ꡢ���Զ��������޲˵���
	//����������Ϊ��Test
	WNDCLASS wndClass = { CS_BYTEALIGNCLIENT,(WNDPROC)MsgProcW,0,0,0,NULL,NULL,NULL,NULL,L"Test"};

	wndClass.hInstance = GetModuleHandle(NULL);           //��ʼ�����������ھ��

	if (!RegisterClassW(&wndClass))                       //ע��һ��������
	{
		return;                          //��ע��ʧ�ܡ��򷵻�
	}

	//��������
	//���������ƣ�Test
	//���ڱ��⣺Test
	//���ڷ��1������һ������Ĵ��ڣ�һ������Ĵ�����һ����������һ���߿�
	//2������һ���б����Ĵ���
	//3������һ���ڱ������ϴ��д��ڲ˵��Ĵ���
	//4������һ��������С����ť�Ĵ���
	//��ʼx��y����Ϊ0����ʼ���ڳߴ�x,yΪ0
	//�����ھ��:NULL,���ڲ˵����:NULL������ʵ�������wnd.Class.hInstance
	//����������NULL
	//std::cout << "Test" << std::endl;
	window = CreateWindow(L"Test", L"Test", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,0,0,0,0,NULL,NULL,wndClass.hInstance,NULL);
	if (!window)
	{
		//std::cout << "test" << std::endl;
		return;
	}

	this->hdc = GetDC(this->window);           //��ȡ�ڴ�DC,DC:һ�����ؿ��һ�����ظߵ��ڴ�
	this->ScreenHdc = CreateCompatibleDC(this->hdc);      //��������Ļ�ռ�ȴ�С���ڶ�DC
    //���ƴ���
	//���¶���Ȧ�ˣ���֪����ɶ������
	//1��BITMAPINFO����Ҫ���ڴ���
	//2����Ļ��ȡ���Ļ�߶�
	//3��Ͷ���豸ƽ����Ϊ1
	//4��32λ���λͼ
	//5��BI_RGB����ѹ����32λRGB��ʽ
	//6��ͼƬ��СΪthis->WindowsHeight * this->WindowsWidth * 4,��Ϊ ɫ�� = 4�ֽ� / ���أ�8bit) = 32 / 8������ÿһ�����ص�ɫ��Ϊ4
	//7��ˮƽ�ֱ���Ϊ0����ֱ�ֱ���Ϊ0
	//8����ɫ����������Ҫ����ɫ��Ϊ0����ȫ����ȫ������Ҫ

	BITMAPINFO bitmapInfo = { sizeof(BITMAPINFOHEADER),this->WindowsWidth ,this->WindowsHeight,1,32,BI_RGB,this->WindowsHeight * this->WindowsWidth * 4, 0,0,0,0 };
	//����DIBλͼ
	LPVOID ptr;
	HBITMAP hbitmap = CreateDIBSection(this->ScreenHdc, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
	
	if (!hbitmap)
	{
		return;
	}

	HBITMAP screenObject = (HBITMAP)SelectObject(this->ScreenHdc, hbitmap);

	//���ô���λ��
	SetWindowPos(this->window, NULL, 50, 50, this->WindowsWidth, this->WindowsHeight, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

	//��ʾ���ڣ�SW_NORMALλ��z����������
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
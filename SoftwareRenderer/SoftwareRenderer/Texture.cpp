#include"Texture.h"


Texture::Texture()
{
	this->T_Height = 512;              //将材质固定在512 * 512
	this->T_Width = 512;
}

void Texture::LoadTexture(const wchar_t* path)       //加载材质
{
	HBITMAP hbitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, this->T_Width, this->T_Height, LR_LOADFROMFILE);

	HDC hdc = CreateCompatibleDC(NULL);              //获取窗口句柄
	SelectObject(hdc, hbitmap);                      //将位图和窗口关联

	if (!hbitmap) {
		DWORD error = GetLastError();
		LPVOID errorMsg;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, error, 0, (LPWSTR)&errorMsg, 0, NULL);
		MessageBoxW(NULL, (LPCWSTR)errorMsg, L"Error", MB_OK);
		LocalFree(errorMsg);
		return;
	}

	//BITMAP bm;
	//GetObject(hbitmap, sizeof(BITMAP), &bm);
	//this->T_Width = bm.bmWidth;
	//this->T_Height = bm.bmHeight;
	//std::wcout << L"Loaded texture size: " << T_Width << L"x" << T_Height << std::endl;

	for (int i = 0; i < this->T_Width; i++)
	{
		for (int j = 0; j < this->T_Height; j++)
		{
			COLORREF w_color = GetPixel(hdc, i, j);         //最低8位存R(最右侧，二进制）,然后中间8位存G,然后8位存B，最左侧存alpha，从右往左
			int r = w_color % 256;               //分别取r,g,b值，r:0--255.对256取模得本身
			int g = (w_color >> 8) % 256;        //偏移8位后同理
			int b = (w_color >> 16) % 256;
			Color color((float)r / 256, (float)g / 256, (float)b / 256, 1);   //alpha给为1,为完全不透明
			this->TextureData[i][j] = color;          //为每一个像素点赋予颜色
		}
	}

}

//void Texture::DrawTexture(HDC hdcDest, int x, int y) {
//	// 创建临时位图缓冲区
//	BITMAPINFO bmi = { 0 };
//	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bmi.bmiHeader.biWidth = T_Width;
//	bmi.bmiHeader.biHeight = -T_Height; // 负号表示原点在左上角
//	bmi.bmiHeader.biPlanes = 1;
//	bmi.bmiHeader.biBitCount = 32;      // 32 位 ARGB
//	bmi.bmiHeader.biCompression = BI_RGB;
//
//	// 将 TextureData 转换为 32 位 ARGB 格式的像素数组
//	uint8_t* pixelBuffer = new uint8_t[T_Width * T_Height * 4];
//	for (int y = 0; y < T_Height; y++) {
//		for (int x = 0; x < T_Width; x++) {
//			Color color = TextureData[x][y];
//			int index = (y * T_Width + x) * 4;
//			pixelBuffer[index + 0] = (uint8_t)(color.b * 255); // B
//			pixelBuffer[index + 1] = (uint8_t)(color.g * 255); // G
//			pixelBuffer[index + 2] = (uint8_t)(color.r * 255); // R
//			pixelBuffer[index + 3] = 255;                      // A
//		}
//	}
//
//	// 使用 SetDIBitsToDevice 直接绘制像素数据
//	SetDIBitsToDevice(
//		hdcDest,                // 目标 DC
//		x, y,                   // 绘制位置
//		T_Width, T_Height,      // 纹理尺寸
//		0, 0,                   // 源位图起始坐标
//		0, T_Height,            // 扫描行范围和起始扫描行
//		pixelBuffer,            // 像素数据
//		&bmi,                   // 位图信息
//		DIB_RGB_COLORS          // 颜色格式
//	);
//
//	delete[] pixelBuffer;
//}

float Texture::T_Clamp(float min,float max,float value)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}

Color Texture::T_Sample(float u,float v)
{
	//std::cout << u << "  " << v << std::endl;
	u = this->T_Clamp(0, 1.0f, u);             //保证u和v在0-1之间
	v = this->T_Clamp(0, 1.0f, v); 
	int Tex_u = u * this->T_Width;
	int Tex_v = v * this->T_Height;
	return this->TextureData[Tex_u][Tex_v];      //将uv映射到材质上,一开始uv为0-1，之后变为512 * 512;
}

Texture::~Texture()
{

}
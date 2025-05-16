#include"Texture.h"


Texture::Texture()
{
	this->T_Height = 512;              //�����ʹ̶���512 * 512
	this->T_Width = 512;
}

void Texture::LoadTexture(const wchar_t* path)       //���ز���
{
	HBITMAP hbitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, this->T_Width, this->T_Height, LR_LOADFROMFILE);

	HDC hdc = CreateCompatibleDC(NULL);              //��ȡ���ھ��
	SelectObject(hdc, hbitmap);                      //��λͼ�ʹ��ڹ���

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
			COLORREF w_color = GetPixel(hdc, i, j);         //���8λ��R(���Ҳ࣬�����ƣ�,Ȼ���м�8λ��G,Ȼ��8λ��B��������alpha����������
			int r = w_color % 256;               //�ֱ�ȡr,g,bֵ��r:0--255.��256ȡģ�ñ���
			int g = (w_color >> 8) % 256;        //ƫ��8λ��ͬ��
			int b = (w_color >> 16) % 256;
			Color color((float)r / 256, (float)g / 256, (float)b / 256, 1);   //alpha��Ϊ1,Ϊ��ȫ��͸��
			this->TextureData[i][j] = color;          //Ϊÿһ�����ص㸳����ɫ
		}
	}

}

//void Texture::DrawTexture(HDC hdcDest, int x, int y) {
//	// ������ʱλͼ������
//	BITMAPINFO bmi = { 0 };
//	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bmi.bmiHeader.biWidth = T_Width;
//	bmi.bmiHeader.biHeight = -T_Height; // ���ű�ʾԭ�������Ͻ�
//	bmi.bmiHeader.biPlanes = 1;
//	bmi.bmiHeader.biBitCount = 32;      // 32 λ ARGB
//	bmi.bmiHeader.biCompression = BI_RGB;
//
//	// �� TextureData ת��Ϊ 32 λ ARGB ��ʽ����������
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
//	// ʹ�� SetDIBitsToDevice ֱ�ӻ�����������
//	SetDIBitsToDevice(
//		hdcDest,                // Ŀ�� DC
//		x, y,                   // ����λ��
//		T_Width, T_Height,      // ����ߴ�
//		0, 0,                   // Դλͼ��ʼ����
//		0, T_Height,            // ɨ���з�Χ����ʼɨ����
//		pixelBuffer,            // ��������
//		&bmi,                   // λͼ��Ϣ
//		DIB_RGB_COLORS          // ��ɫ��ʽ
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
	u = this->T_Clamp(0, 1.0f, u);             //��֤u��v��0-1֮��
	v = this->T_Clamp(0, 1.0f, v); 
	int Tex_u = u * this->T_Width;
	int Tex_v = v * this->T_Height;
	return this->TextureData[Tex_u][Tex_v];      //��uvӳ�䵽������,һ��ʼuvΪ0-1��֮���Ϊ512 * 512;
}

Texture::~Texture()
{

}
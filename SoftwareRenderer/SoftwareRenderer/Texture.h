#pragma once
#include"global.h"
#include"Color.h"
#include"Windows.h"

//����
class Texture
{
private:
	int T_Width;
	int T_Height;
	Color TextureData[1024][1024];

public:
	Texture();
	void LoadTexture(const wchar_t *path);
	Color T_Sample(float u, float v);                    //ӳ�������ɫ
	float T_Clamp(float min, float max, float value);     //ǯ��
	//void DrawTexture(HDC hdc,int x,int y);
	~Texture();
};
#pragma once
#include"global.h"
#include<windows.h>
#include<string>

//������
class Windows
{
public:

	//HWND:���ھ��
	//HWND���ã�׼ȷ��λ�����崰�ڣ���Ȼ������������Ӧ�������ڡ����п��ܸ��Ӵ��ڹ�ϵ����
	HWND window;                   //Ψһ��ʾһ������,HWND���������֤
	                               
	int WindowsHeight;                //���ڸ߶�
	int WindowsWidth;                 //���ڿ��

	//HDC:�豸�����ľ��
	HDC hdc;                         //HDCΪ���ƹ��ߣ��ܹ����ߡ���ɫ��ͼ�����Ⱦ����Ļ��
	HDC ScreenHdc;                   //��Ļ����HDC,�����崦��

	Windows();

	//��ʼ������
	Windows(int w, int h, const char *name);

	~Windows();
};
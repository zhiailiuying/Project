#pragma once
#include"global.h"
#include<windows.h>
#include<string>

//窗口类
class Windows
{
public:

	//HWND:窗口句柄
	//HWND作用：准确定位到具体窗口，不然点击、键击会对应不到窗口、还有可能父子窗口关系混乱
	HWND window;                   //唯一表示一个窗口,HWND类似于身份证
	                               
	int WindowsHeight;                //窗口高度
	int WindowsWidth;                 //窗口宽度

	//HDC:设备上下文句柄
	HDC hdc;                         //HDC为绘制工具，能够将线、颜色、图像等渲染到屏幕上
	HDC ScreenHdc;                   //屏幕绘制HDC,做缓冲处理

	Windows();

	//初始化窗口
	Windows(int w, int h, const char *name);

	~Windows();
};
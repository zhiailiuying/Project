#pragma once
#include"global.h"
#include"Matrix.h"
#include"Transform.h"

//相机
class Camera
{
public:

	Transform transform;                //变换矩阵
	Matrix CameraPosition;              //存放相机位置的矩阵
	Matrix projection;                  //投影变换矩阵

	Camera();
	Camera(Transform t);
	Matrix LookAt(const Vector3f& position,const Vector3f &Lookat ,const Vector3f &upAixs);
	Matrix CameraProjectionTransform(float fov,float aspect,float n,float f);           //基于fov角和宽高比的投影变换
	Matrix CameraProjectionTransform(float l, float r, float n, float f, float t, float b);        //课上的投影变换
	Matrix Orthographic(float i, float r, float n, float f, float t, float b);                     //正交变换
	~Camera();
};
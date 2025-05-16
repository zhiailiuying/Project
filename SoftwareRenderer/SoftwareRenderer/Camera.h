#pragma once
#include"global.h"
#include"Matrix.h"
#include"Transform.h"

//���
class Camera
{
public:

	Transform transform;                //�任����
	Matrix CameraPosition;              //������λ�õľ���
	Matrix projection;                  //ͶӰ�任����

	Camera();
	Camera(Transform t);
	Matrix LookAt(const Vector3f& position,const Vector3f &Lookat ,const Vector3f &upAixs);
	Matrix CameraProjectionTransform(float fov,float aspect,float n,float f);           //����fov�ǺͿ�߱ȵ�ͶӰ�任
	Matrix CameraProjectionTransform(float l, float r, float n, float f, float t, float b);        //���ϵ�ͶӰ�任
	Matrix Orthographic(float i, float r, float n, float f, float t, float b);                     //�����任
	~Camera();
};
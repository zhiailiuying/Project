#include"Camera.h"

Camera::Camera()
{

}

Camera::Camera(Transform t)
{
	this->transform = t;
}

Matrix Camera::LookAt(const Vector3f &position,const Vector3f &LookAt, const Vector3f &upAxis)
{
	Vector3f lookAtVector = LookAt - position;
	lookAtVector.Normalize();

	Vector3f rightVector = rightVector.Cross(upAxis, lookAtVector);
	rightVector.Normalize();

	Vector3f upAxisVector = upAxisVector.Cross(lookAtVector, rightVector);
	upAxisVector.Normalize();

	Matrix m;
	m.value[0][0] = rightVector.x;  m.value[0][1] = rightVector.y;  m.value[0][2] = rightVector.z;  m.value[0][3] = -position.x;
	m.value[1][0] = upAxisVector.x; m.value[1][1] = upAxisVector.y; m.value[1][2] = upAxisVector.z; m.value[1][3] = -position.y;
	m.value[2][0] = lookAtVector.x; m.value[2][1] = lookAtVector.y; m.value[2][2] = lookAtVector.z; m.value[2][3] = -position.z;
	m.value[3][0] = 0;              m.value[3][1] = 0;              m.value[3][2] = 0;              m.value[3][3] = 1;
	this->CameraPosition = m;
	return m;
}

//投影变换,基于fov
Matrix Camera::CameraProjectionTransform(float fov,float aspect,float n,float f)
{
	float alpha = fov * MY_PI / 180.0f;

	this->projection.value[0][0] = 1.0f / (tan(alpha / 2.0f) * aspect);
	this->projection.value[1][1] = tan(alpha / 2.0f);
	this->projection.value[2][2] = (n + f) / (n - f);
	this->projection.value[2][3] = 2 * n * f / (n - f);
	this->projection.value[3][2] = -1;

	return this->projection;
}

//投影变换，games101课上的那个
//Matrix Camera::CameraProjectionTransform(float l,float r,float n,float f,float t,float b)
//{
//	Matrix Project_Ortho, Ortho,Projection;
//	Project_Ortho.value[0][0] = n;
//	Project_Ortho.value[1][1] = n;
//	Project_Ortho.value[2][2] = n + f;
//	Project_Ortho.value[2][3] = -n * f;
//	Project_Ortho.value[3][2] = 1;
//
//	Ortho = this->Orthographic(l,r,n,f,t,b);        //返回正交变换
//
//	Projection = Ortho * Project_Ortho;                //注意矩阵相乘的顺序
//	this->projection = Projection;
//	return this->projection;
//}

//正交投影
Matrix Camera::Orthographic(float l, float r, float n, float f, float t, float b)
{
	Matrix translate, scale,Ortho;                   //平移、缩放、正交

	translate.value[0][0] = 1;
	translate.value[1][1] = 1;
	translate.value[2][2] = 1;
	translate.value[0][3] = -(r + l) / 2;
	translate.value[1][3] = -(t + b) / 2;
	translate.value[2][3] = -(n + f) / 2;

	scale.value[0][0] = 2.0f / (r - l);
	scale.value[1][1] = 2.0f / (t - b);
	scale.value[2][2] = 2.0f / (n - f);
	scale.value[3][3] = 1;

	Ortho = scale * translate;
	return Ortho;
}

Camera::~Camera()
{

}

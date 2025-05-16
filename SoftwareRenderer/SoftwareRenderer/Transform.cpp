#include"Transform.h"

Transform::Transform()
{
	this->ObjectToWorld.Identity();                    //�����绷����ʼ��Ϊ��λ����
}

Transform::Transform(Vector3f pos,Vector3f rot,Vector3f scale)
{
	this->position = pos;
	this->Rotation = rot;
	this->Scale = scale;
}

Matrix Transform::Translate(const Vector3f &v)            //ƽ�Ʊ任
{
	this->position = v;

	Matrix t;
	t.value[0][3] = position.x;                        //ƽ�ƾ���{ {1��0��0��t_x}
	t.value[1][3] = position.y;                        //         {0, 1, 0, t_y}
	t.value[2][3] = position.z;                        //         {0, 0, 1, t_z}
	return t;                                          //         {0, 0, 0, 1}}
}

Matrix Transform::RotateX(float angle)                 //��x����ת����
{
	this->Rotation.x =  angle;                       //�Ƕȱ��浽��ת����
	Matrix rotate_X;
	float alpha_angle = angle * MY_PI / 180.0f;
	rotate_X.value[1][1] =  std::cos(alpha_angle);
	rotate_X.value[1][2] = -std::sin(alpha_angle);
	rotate_X.value[2][1] =  std::sin(alpha_angle);
	rotate_X.value[2][2] =  std::cos(alpha_angle);
	return rotate_X;
}

Matrix Transform::RotateY(float angle)                //��y����ת����
{
	this->Rotation.y = angle;
	Matrix rotate_Y;

	float alpha_angle = angle * MY_PI / 180.0f;
	rotate_Y.value[0][0] =  std::cos(alpha_angle);
	rotate_Y.value[0][2] =  std::sin(alpha_angle);
	rotate_Y.value[2][0] = -std::sin(alpha_angle);
	rotate_Y.value[2][2] =  std::cos(alpha_angle);
	return rotate_Y;
}

Matrix Transform::RotateZ(float angle)                 //��z����ת����
{
	this->Rotation.z = angle;
	Matrix rotate_Z;

	float alpha_angle = angle * MY_PI / 180.0f;
	rotate_Z.value[0][0] =  std::cos(alpha_angle);
	rotate_Z.value[0][1] = -std::sin(alpha_angle);
	rotate_Z.value[1][0] =  std::sin(alpha_angle);
	rotate_Z.value[1][1] =  std::cos(alpha_angle);
	return rotate_Z;
}

Matrix Transform::Rotate(const Vector3f& rotateVector)          //������ͬʱ��ת
{
	this->Rotation = rotateVector;
	Matrix rotate,r_X,r_Y,r_Z;                                 //����rotateVector�ֱ���x����ת�ǣ�y����ת�ǡ�z����ת��

	r_X = this->RotateX(rotateVector.x);
	r_Y = this->RotateY(rotateVector.y);
	r_Z = this->RotateZ(rotateVector.z);

	rotate = r_X * r_Y * r_Z;
	return rotate;
}

Matrix Transform::ScaleMatrix(const Vector3f& scale)             //���ž���
{
	Matrix s;

	s.value[0][0] = scale.x;
	s.value[1][1] = scale.y;
	s.value[2][2] = scale.z;

	return s;
}

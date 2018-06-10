#pragma once
#include <math.h>
#define PI 3.1415927

class G_3DView
{
public:
	G_3DView();
	~G_3DView();

	void setPosition(COLORREF color);   //����ɫֵӳ��Ϊ����
	void Transform3Dto2D(float &x, float &y, float z);   //����ά����Ͷ�䵽��άƽ��
	void setPixel(CDC *pDC, COLORREF color);   //��ת������������ʾ����

	void drawCoordinateSystem3D(CDC *pDC);  //������ά����ϵ

public:
	float x, y, z, r;
	float O_x, O_y, slant, scale;
};


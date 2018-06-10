#pragma once
#include <math.h>
#define PI 3.1415927

class G_3DView
{
public:
	G_3DView();
	~G_3DView();

	void setPosition(COLORREF color);   //将颜色值映射为坐标
	void Transform3Dto2D(float &x, float &y, float z);   //将三维坐标投射到二维平面
	void setPixel(CDC *pDC, COLORREF color);   //将转换后的坐标点显示出来

	void drawCoordinateSystem3D(CDC *pDC);  //绘制三维坐标系

public:
	float x, y, z, r;
	float O_x, O_y, slant, scale;
};


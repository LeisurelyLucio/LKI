/**
* @文件名：G_3DView.h
* @简要描述：包含G_3DView类，功能为投影变换以及坐标系、坐标点的绘制
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/9
*
*/

#pragma once
#include <math.h>
#define PI 3.1415927


/*
* 公共成员：
* float x, y, z：分别储存像素点RGB坐标，用于投影变换
* float r：储存绘制像素点
* float O_x, O_y：储存坐标系原点
* float slant：储存旋转角
* float scale：储存缩放比例尺
* =================================================
* 公共方法：
* setPosition, 将颜色值映射为坐标
* Transform3Dto2D, 将三维坐标投射到二维平面
* setPixel, 将转换后的坐标点显示出来
* drawCoordinateSystem3D, 绘制三维坐标系
*/
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


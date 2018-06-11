/**
* @�ļ�����G_3DView.h
* @��Ҫ����������G_3DView�࣬����ΪͶӰ�任�Լ�����ϵ�������Ļ���
*
* @�汾��Ϣ�� 1.1
* @���ߣ� Lucio
* @���䣺 Wangyang3434@gmail.com
* @���ڣ�2018/5/10
* @�޸��ߣ�Lucio
* @�������ڣ�2018/6/9
*
*/

#pragma once
#include <math.h>
#define PI 3.1415927


/*
* ������Ա��
* float x, y, z���ֱ𴢴����ص�RGB���꣬����ͶӰ�任
* float r������������ص�
* float O_x, O_y����������ϵԭ��
* float slant��������ת��
* float scale���������ű�����
* =================================================
* ����������
* setPosition, ����ɫֵӳ��Ϊ����
* Transform3Dto2D, ����ά����Ͷ�䵽��άƽ��
* setPixel, ��ת������������ʾ����
* drawCoordinateSystem3D, ������ά����ϵ
*/
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


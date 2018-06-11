#include "stdafx.h"
#include "G_3DView.h"


G_3DView::G_3DView()
{
	slant = PI / 4;
	O_x = 180;
	O_y = 480;
	x = -1;
	y = -1;
	z = -1;
	r = 1.4; //半径具体数值
	scale = 1.6;
}


G_3DView::~G_3DView()
{

}

void G_3DView::setPosition(COLORREF color)
{
	x = GetRValue(color);
	y = GetGValue(color);
	z = GetBValue(color);
}

void G_3DView::setPixel(CDC *pDC, COLORREF color)
{
	Transform3Dto2D(x, y, z);

//	pDC->MoveTo(O_x + x * scale, O_y - y * scale);
	
	CBrush brush(color);
	CBrush* oldBr = pDC->SelectObject(&brush);
	CPen Pen(PS_SOLID, 1, color);
	CPen	*pOldPen = pDC->SelectObject(&Pen);
	pDC->Ellipse(O_x + (x - r) * scale, O_y - (y - r) * scale, O_x + (x + r) * scale, O_y - (y + r) * scale);
	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();
	pDC->SelectObject(oldBr);
	brush.DeleteObject();
}

void G_3DView::Transform3Dto2D(float &x, float &y, float z)
{
	x = x - (z * cos(slant)) / 2;
	y = y - (z * sin(slant)) / 2;
}

void G_3DView::drawCoordinateSystem3D(CDC * pDC)
{
	float x, y, z;

	// -------------------- 绘制坐标系 -------------------------

	// 坐标x轴
	pDC->MoveTo(O_x, O_y);
	pDC->LineTo(O_x + 255 * scale + 2, O_y);

	// 坐标y轴
	pDC->MoveTo(O_x, O_y);
	pDC->LineTo(O_x, O_y - 255 * scale + 2);

	// 坐标z轴
	x = 0, y = 0;
	Transform3Dto2D(x, y, 255+2);
	pDC->MoveTo(O_x, O_y);
	pDC->LineTo( x * scale + O_x, -y *scale + O_y);

	// 坐标x轴的箭头
	pDC->MoveTo(O_x + (255) * scale + 2, O_y);
	pDC->LineTo(O_x + (255 - 4) * scale, O_y + 3 * scale);
	pDC->MoveTo(O_x + (255) * scale + 2, O_y);
	pDC->LineTo(O_x + (255 - 4) * scale, O_y - 3 * scale);

	// 坐标y轴的箭头
	pDC->MoveTo(O_x, O_y - (255) *scale + 2);
	pDC->LineTo(O_x - (3) * scale, O_y - (255 - 4) * scale);
	pDC->MoveTo(O_x, O_y - (255) *scale + 2);
	pDC->LineTo(O_x + (3) * scale, O_y - (255 - 4) *scale);

	// 坐标z轴的箭头
	x = 0, y = 0;
	Transform3Dto2D(x, y, 255 + 2);
	pDC->MoveTo(x * scale + O_x, -y * scale + O_y);	
	pDC->LineTo((x+4) * scale + O_x, -y * scale + O_y);
	pDC->MoveTo(x * scale + O_x, -y * scale + O_y);
	pDC->LineTo(x * scale + O_x, (-4 - y) * scale + O_y);

	// -------------------- 绘制文字 -------------------------

	// 绘制x轴的R
	pDC->TextOutW(O_x + scale * (255-1), O_y - scale * (14), CString("R"));
	// 绘制y轴的G
	pDC->TextOutW(O_x + scale * (3), O_y - scale * (255 + 4), CString("G"));
	// 绘制z轴的B
	x = 0, y = 0;
	Transform3Dto2D(x, y, 255+5);
	pDC->TextOutW(O_x + scale * (x - 8), O_y - scale * (y + 15), CString("B"));
	CString s;
	// 绘制x轴刻度文字
	for (int ScaleTextX = 25; ScaleTextX < 255 + 1; ScaleTextX += 25)
	{
		s.Format(_T("%d"), ScaleTextX);
		pDC->TextOutW(O_x + scale * (ScaleTextX - 8), O_y + scale * (3), s);
		pDC->MoveTo(O_x + (ScaleTextX) * scale, O_y + 2*scale);
		pDC->LineTo(O_x + (ScaleTextX) * scale, O_y - 2*scale);
	}

	// 绘制y轴刻度文字
	for (int ScaleTextY = 25; ScaleTextY <= 255 + 1; ScaleTextY += 25)
	{
		s.Format(_T("%d"), ScaleTextY);
		pDC->TextOutW(O_x + scale * (-20), O_y - scale * (ScaleTextY + 1), s);
		pDC->MoveTo(O_x + 2 * scale, O_y - (ScaleTextY)* scale);
		pDC->LineTo(O_x - 2 * scale, O_y - (ScaleTextY)* scale);
	}

	// 绘制z轴刻度文字
	for (int ScaleTextZ = 50; ScaleTextZ <= 255 + 4; ScaleTextZ += 50)
	{
		s.Format(_T("%d"), ScaleTextZ);
		x = 0, y = 0;
		Transform3Dto2D(x, y, ScaleTextZ);
		pDC->TextOutW(O_x + scale * (x + 1), O_y - scale * (y), s);
		pDC->MoveTo(O_x + (x-1.5)* scale, O_y - (y+1.5) * scale);
		pDC->LineTo(O_x + (x+1.5)* scale, O_y - (y-1.5) * scale);
	}

	// 绘制函数图的Title
	x = 0, y = 0;
	Transform3Dto2D(x, y, 255 + 5);
	pDC->TextOutW(O_x + (x + 215) * scale, O_y - y * scale, CString("色彩分布图"));

}

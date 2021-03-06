/**
* @文件名：LKIView.cpp
* @简要描述：包含CLKIView类，功能为显示三维坐标系以及颜色值处理结果并可进行旋转、缩放、平移以及选择像素点等操作
*
* @版本信息： 1.1
* @作者： Lucio
* @邮箱： Wangyang3434@gmail.com
* @日期：2018/5/10
* @修改者：Lucio
* @更新日期：2018/6/11
*
* @使用了MFC Microsoft Office Fluent 用户界面，此部分版权所有(C) Microsoft Corporation，保留所有权利
*/

// LKIView.cpp: CLKIView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LKI.h"
#endif

#include "LKIDoc.h"
#include "LKIView.h"
#include "resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLKIView

IMPLEMENT_DYNCREATE(CLKIView, CView)

BEGIN_MESSAGE_MAP(CLKIView, CView)
	ON_WM_STYLECHANGED()
	/*ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()*/
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CLKIView 构造/析构

/**
* 功能：构造类，并初始化类成员
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
CLKIView::CLKIView()
{
	mouse_L_down = false;
}

CLKIView::~CLKIView()
{
}

/**
* 功能：实现坐标系自适应参数的初始化
* 参数及返回值说明：CRect rect：窗口大小
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIView::SetInitAdaptive(CRect rect)
{
	float delta = 14 / 19.0;
	if (rect.Height()/(float)rect.Width() < (1+abs(sin(m_set.coordination.slant)))/(1+abs(cos(m_set.coordination.slant)))) {
		delta = sin(m_set.coordination.slant);
		m_set.coordination.scale = (rect.Width() * 1 / (1 + delta)) / 255.0;
	}
	else {
		delta = cos(m_set.coordination.slant);
		m_set.coordination.scale = (rect.Height() * 1 / (1 + delta)) / 255.0;
	}
	m_set.coordination.O_x = rect.Width() * abs(cos(m_set.coordination.slant)) / (1 + abs(cos(m_set.coordination.slant))) - 18 * m_set.coordination.scale* m_set.coordination.scale;
	m_set.coordination.O_y = rect.Height() * 1 / (1 + abs(sin(m_set.coordination.slant))) + 18 * m_set.coordination.scale* m_set.coordination.scale;
}

/**
* 功能：实现坐标系的不同情况的绘制
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIView::OnDraw(CDC * pDC)
{
	CLKIDoc* pDoc = GetDocument();

	switch (pDoc->show_level)
	{
	case ON_3D_COORDINATION: {
		 
		if (pDoc->image_now.IsNull()) return;
		// 更改光标形状
		BeginWaitCursor();

		CMFCRibbonProgressBar* progress = ((CMainFrame*)AfxGetMainWnd())->progress;
		progress->SetPos(0, true);

		//自适应显示
		GetClientRect(pDoc->GraphClient);
		SetInitAdaptive(pDoc->GraphClient);
		
		m_set.coordination.drawCoordinateSystem3D(pDC);
		
		/*原去重算法。。。大幅减慢程序运行速度
		int step = 0;
		for (auto i = pDoc->Cset.begin(); i != pDoc->Cset.end(); i++) {
			color = i->color;
			coordination.setPosition(color);
			coordination.setPixel(pDC, color);
			step++;
			progress->SetPos(100 * step / (float)pDoc->Cset.size(), true);
		}*/

		m_set.deletetree();

		//获得图像数据
		int height = pDoc->image_now.GetHeight(), width = pDoc->image_now.GetWidth();
		//行距
		int pitch = pDoc->image_now.GetPitch();
		//首像素地址
		BYTE * src = (BYTE*)pDoc->image_now.GetBits();
		BYTE * src2 = (BYTE*)pDoc->image_show.GetBits();

		for (int line = 0; line < height; line++) {
			for (int row = 0; row < width; row++) {
				CPoint p(row, line);
				m_set.insert(RGB(*(src + pitch * line + row * 3+2), *(src + pitch * line + row * 3+1), *(src + pitch * line + row * 3)), p);
			}
			progress->SetPos(100 * line / (float)height, true);
		}

		m_set.Rdraw(pDC);

		// 恢复光标形状
		EndWaitCursor();
		progress->SetPos(0, true);
		break;
	}
	case ON_MOVE:
	case ON_ZOOM_INOUT: {
		if (pDoc->image_now.IsNull()) return;
		// 更改光标形状
		BeginWaitCursor();
		
		m_set.coordination.drawCoordinateSystem3D(pDC);
		m_set.Rdraw(pDC);

		// 恢复光标形状
		EndWaitCursor();
		
		break;
	}
	case ON_MOVING: {
		if (pDoc->image_now.IsNull()) return;
		m_set.coordination.drawCoordinateSystem3D(pDC);
		break;
	}
	case ON_CHOOSING: {
		m_set.coordination.drawCoordinateSystem3D(pDC);

		CClientDC dc(this); //获取设备句柄  
		dc.SelectStockObject(NULL_BRUSH);//不使用画刷 
		if (pDoc->choose_tool == CHOOSE_RECTANGLE) {
			CPoint tmp1, tmp2;
			if (begin_move.x < end_move.x) { tmp1.x = begin_move.x;  tmp2.x = end_move.x; }
			else { tmp2.x = begin_move.x;  tmp1.x = end_move.x; }
			if (begin_move.y < end_move.y) { tmp1.y = begin_move.y;  tmp2.y = end_move.y; }
			else { tmp2.y = begin_move.y;  tmp1.y = end_move.y; }
			dc.Rectangle(CRect(tmp1, tmp2));
		}
		else if (pDoc->choose_tool == CHOOSE_CIRCLE) {
			CPoint tmp1, tmp2;
			if (begin_move.x < end_move.x) { tmp1.x = begin_move.x;  tmp2.x = end_move.x; }
			else { tmp2.x = begin_move.x;  tmp1.x = end_move.x; }
			if (begin_move.y < end_move.y) { tmp1.y = begin_move.y;  tmp2.y = end_move.y; }
			else { tmp2.y = begin_move.y;  tmp1.y = end_move.y; }
			if (tmp2.x - tmp1.x != tmp2.y - tmp1.y) {
				tmp2.y = tmp1.y + tmp2.x - tmp1.x;
			}
			dc.Ellipse(CRect(tmp1, tmp2));
		}
		
		break;
	}
	case ON_CHOOSED: {
		if (pDoc->image_now.IsNull()) return;
		// 更改光标形状
		BeginWaitCursor();

		m_set.coordination.drawCoordinateSystem3D(pDC);
		m_set.Rdraw(pDC);

		// 恢复光标形状
		EndWaitCursor();
		break;
	}
	default:
		break;
	}

}

BOOL CLKIView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

//void CLKIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}
//
//void CLKIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// CLKIView 诊断

#ifdef _DEBUG
void CLKIView::AssertValid() const
{
	CView::AssertValid();
}

void CLKIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLKIDoc* CLKIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLKIDoc)));
	return (CLKIDoc*)m_pDocument;
}
#endif //_DEBUG


// CLKIView 消息处理程序
void CLKIView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	CView::OnStyleChanged(nStyleType,lpStyleStruct);
}


/**
* 功能：鼠标左键按下事件的响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	end_move = point;
	mouse_L_down = true;
}

/**
* 功能：鼠标左键抬起事件的响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CLKIDoc* pDoc = GetDocument();
	mouse_L_down = false;
	if (pDoc->image_now.IsNull()) {
		return;
	}
	switch (pDoc->mouse_level)
	{
	case MOVING: {
		pDoc->show_level = ON_MOVE;
		end_move = point;
		m_set.coordination.O_x += (end_move.x - begin_move.x)/m_set.coordination.scale;
		m_set.coordination.O_y += (end_move.y - begin_move.y)/m_set.coordination.scale;
		//pDoc->mouse_level = VIEWING;
		Invalidate();
		break;
	}
	case CHOOSING: {
		begin_move = point;
		CPoint tmp1, tmp2;
		begin_move.x = (begin_move.x - m_set.coordination.O_x) / m_set.coordination.scale;
		begin_move.y = (-begin_move.y + m_set.coordination.O_y) / m_set.coordination.scale;
		end_move.x = (end_move.x - m_set.coordination.O_x) / m_set.coordination.scale;
		end_move.y = (-end_move.y + m_set.coordination.O_y) / m_set.coordination.scale;
		if (pDoc->choose_tool == CHOOSE_PIXEL) {
			tmp1 = begin_move;
			tmp2 = begin_move;
		}
		else {
			if (begin_move.x < end_move.x) { tmp1.x = begin_move.x;  tmp2.x = end_move.x; }
			else { tmp2.x = begin_move.x;  tmp1.x = end_move.x; }
			if (begin_move.y < end_move.y) { tmp1.y = begin_move.y;  tmp2.y = end_move.y; }
			else { tmp2.y = begin_move.y;  tmp1.y = end_move.y; }
		}
		if (pDoc->choose_tool == CHOOSE_CIRCLE && tmp2.x - tmp1.x != tmp2.y - tmp1.y) {
			tmp2.y = tmp1.y + tmp2.x - tmp1.x;
		}

		CRect chooseRect(tmp1, tmp2);
		//设置选择集
		pDoc->Set_choosesetByCoordination(chooseRect);

		pDoc->show_level = ON_CHOOSED;
		pDoc->UpdateAllViews(NULL);

		break;
	}
	default:
		break;
	}
		
	
}

/**
* 功能：鼠标移动事件的响应
* 参数及返回值说明：无
*
* 作者：Lucio
* 日期：2018/6/9
* 版本信息： 1.1
*/
void CLKIView::OnMouseMove(UINT nFlags, CPoint point)
{
	CLKIDoc* pDoc = GetDocument();
	
	if (pDoc->image_now.IsNull()) {
		return;
	}
	switch (pDoc->mouse_level)
	{
	case MOVING: {
		if (mouse_L_down) {
			begin_move = end_move;
			pDoc->show_level = ON_MOVING;
			end_move = point;
			m_set.coordination.O_x += (end_move.x - begin_move.x) / m_set.coordination.scale;
			m_set.coordination.O_y += (end_move.y - begin_move.y) / m_set.coordination.scale;
			Invalidate();
		}
		break;
	}
	case CHOOSING: {
		if (mouse_L_down && pDoc->choose_tool != CHOOSE_PIXEL) {
			begin_move = point;
			pDoc->show_level = ON_CHOOSING;
			Invalidate();
		}
		break;
	}
	default:
		break;
	}
	
}
